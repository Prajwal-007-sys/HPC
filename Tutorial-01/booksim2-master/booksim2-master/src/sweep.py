import csv
import re
import subprocess
from pathlib import Path
from statistics import mean

BOOKSIM = Path("./booksim").resolve()
BASE_CONFIG = Path("./examples/torus88").resolve()

RESULTS_DIR = Path("./torus88_results")
RESULTS_DIR.mkdir(exist_ok=True)

TRAFFIC_PATTERNS = ["uniform", "transpose"]

START_RATE = 0.005
MAX_RATE = 1.0

# Fine search between stable and unstable points
FINE_INTERVALS = 10


def make_config(base_text, traffic, injection_rate):
    """Create a temporary BookSim configuration."""

    text = re.sub(
        r"(?m)^\s*traffic\s*=\s*[^;]+;",
        f"traffic = {traffic};",
        base_text
    )

    text = re.sub(
        r"(?m)^\s*injection_rate\s*=\s*[^;]+;",
        f"injection_rate = {injection_rate:.6f};",
        text
    )

    return text


def run_booksim(base_text, traffic, rate, run_id):

    temp_config = RESULTS_DIR / f"_temp_{traffic}_{run_id}.cfg"
    raw_output = RESULTS_DIR / (
        f"raw_{traffic}_{run_id:03d}_{rate:.6f}.txt"
    )

    config = make_config(
        base_text,
        traffic,
        rate
    )

    temp_config.write_text(config)

    try:

        result = subprocess.run(
            [
                str(BOOKSIM),
                str(temp_config)
            ],
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True
        )

        output = result.stdout

        raw_output.write_text(output)

        return output

    finally:

        if temp_config.exists():
            temp_config.unlink()


def is_unstable(output):

    markers = [
        "Simulation unstable, ending",
        "Too many sample periods needed to converge",
        "Average latency for class 0 exceeded 500 cycles"
    ]

    return any(marker in output for marker in markers)


def extract_final_latency(output):

    marker = "====== Overall Traffic Statistics ======"

    position = output.rfind(marker)

    if position == -1:
        return None

    final_block = output[position:]

    match = re.search(
        r"Network latency average\s*=\s*([0-9]+(?:\.[0-9]+)?)",
        final_block
    )

    if match:
        return float(match.group(1))

    return None


def extract_value(output, label):

    matches = re.findall(
        rf"{re.escape(label)}\s*=\s*([0-9]+(?:\.[0-9]+)?)",
        output
    )

    if matches:
        return float(matches[-1])

    return None


def run_one(data, base_text, traffic, rate, run_id):

    print(
        f"[{run_id:02d}] "
        f"traffic={traffic}, "
        f"injection_rate={rate:.6f}",
        flush=True
    )

    output = run_booksim(
        base_text,
        traffic,
        rate,
        run_id
    )

    unstable = is_unstable(output)

    latency = extract_final_latency(output)

    if unstable:

        print(
            "     BOOKSIM: UNSTABLE / SATURATION REGION"
        )

        status = "unstable"

    elif latency is not None:

        print(
            f"     Network latency = {latency:.6f}"
        )

        status = "stable"

    else:

        raise RuntimeError(
            "BookSim produced neither final statistics "
            "nor a recognized unstable termination."
        )

    row = {
        "traffic": traffic,
        "injection_rate": rate,
        "status": status,
        "network_latency": latency,
        "injected_packet_rate":
            extract_value(
                output,
                "Injected packet rate average"
            ),
        "accepted_packet_rate":
            extract_value(
                output,
                "Accepted packet rate average"
            ),
        "injected_flit_rate":
            extract_value(
                output,
                "Injected flit rate average"
            ),
        "accepted_flit_rate":
            extract_value(
                output,
                "Accepted flit rate average"
            ),
        "packet_latency":
            extract_value(
                output,
                "Packet latency average"
            ),
        "flit_latency":
            extract_value(
                output,
                "Flit latency average"
            ),
        "fragmentation":
            extract_value(
                output,
                "Fragmentation average"
            ),
        "hops":
            extract_value(
                output,
                "Hops average"
            )
    }

    data.append(row)

    return row


def next_rate(rate):

    if rate < 0.010:

        return round(rate + 0.001, 6)

    elif rate < 0.100:

        return round(rate + 0.010, 6)

    else:

        return round(rate + 0.020, 6)


def fine_search(
    data,
    base_text,
    traffic,
    stable_rate,
    unstable_rate,
    run_id
):

    print()
    print("-" * 70)

    print(
        f"Fine search between "
        f"{stable_rate:.6f} and "
        f"{unstable_rate:.6f}"
    )

    print("-" * 70)

    step = (
        unstable_rate - stable_rate
    ) / FINE_INTERVALS

    rate = stable_rate + step

    while rate < unstable_rate:

        run_id += 1

        row = run_one(
            data,
            base_text,
            traffic,
            round(rate, 6),
            run_id
        )

        if row["status"] == "unstable":

            unstable_rate = rate
            break

        stable_rate = rate

        rate += step

    return (
        stable_rate,
        unstable_rate,
        run_id
    )


def sample_points(data, count=10):

    stable = [
        x for x in data
        if x["status"] == "stable"
    ]

    if len(stable) <= count:

        return stable

    selected = []

    # Always keep first point
    selected.append(stable[0])

    # Select approximately evenly spaced points
    step = (
        len(stable) - 1
    ) / (count - 1)

    for i in range(1, count - 1):

        index = round(i * step)

        selected.append(
            stable[index]
        )

    # Always keep last stable point
    selected.append(stable[-1])

    return selected


def write_csv(path, rows):

    if not rows:
        return

    fields = [
        "traffic",
        "injection_rate",
        "status",
        "network_latency",
        "packet_latency",
        "flit_latency",
        "injected_packet_rate",
        "accepted_packet_rate",
        "injected_flit_rate",
        "accepted_flit_rate",
        "fragmentation",
        "hops"
    ]

    with open(
        path,
        "w",
        newline=""
    ) as f:

        writer = csv.DictWriter(
            f,
            fieldnames=fields
        )

        writer.writeheader()

        writer.writerows(rows)


def run_traffic(traffic, base_text):

    print()
    print("=" * 70)
    print(
        f"TRAFFIC PATTERN: {traffic}"
    )
    print("=" * 70)

    data = []

    rate = START_RATE

    run_id = 0

    last_stable_rate = None

    first_unstable_rate = None

    # -------------------------------------------------
    # COARSE SEARCH
    # -------------------------------------------------

    while rate <= MAX_RATE:

        run_id += 1

        row = run_one(
            data,
            base_text,
            traffic,
            rate,
            run_id
        )

        if row["status"] == "unstable":

            first_unstable_rate = rate

            break

        last_stable_rate = rate

        rate = next_rate(rate)

    # -------------------------------------------------
    # FINE SEARCH
    # -------------------------------------------------

    if (
        first_unstable_rate is not None
        and last_stable_rate is not None
    ):

        (
            last_stable_rate,
            first_unstable_rate,
            run_id
        ) = fine_search(
            data,
            base_text,
            traffic,
            last_stable_rate,
            first_unstable_rate,
            run_id
        )

    # -------------------------------------------------
    # SORT
    # -------------------------------------------------

    data.sort(
        key=lambda x:
        x["injection_rate"]
    )

    # -------------------------------------------------
    # SAMPLE ~10 POINTS
    # -------------------------------------------------

    sampled = sample_points(
        data,
        10
    )

    # -------------------------------------------------
    # SATURATION RESULT
    # -------------------------------------------------

    if first_unstable_rate is not None:

        print()
        print(
            "SATURATION BOUNDARY"
        )

        print(
            f"Last stable rate  = "
            f"{last_stable_rate:.6f}"
        )

        print(
            f"First unstable rate = "
            f"{first_unstable_rate:.6f}"
        )

        print(
            f"Saturation lies between "
            f"{last_stable_rate:.6f} "
            f"and "
            f"{first_unstable_rate:.6f}"
        )

    # -------------------------------------------------
    # SAVE ALL RESULTS
    # -------------------------------------------------

    write_csv(
        RESULTS_DIR /
        f"all_results_{traffic}.csv",
        data
    )

    # -------------------------------------------------
    # SAVE SAMPLED RESULTS
    # -------------------------------------------------

    write_csv(
        RESULTS_DIR /
        f"sampled_results_{traffic}.csv",
        sampled
    )

    # -------------------------------------------------
    # PRINT FINAL RESULTS
    # -------------------------------------------------

    print()
    print(
        "=" * 70
    )

    print(
        f"10 DATA POINTS - {traffic}"
    )

    print(
        "=" * 70
    )

    for x in sampled:

        print(
            f"({x['injection_rate']:.6f}, "
            f"{x['network_latency']:.6f})"
        )

    print()
    print(
        "Results saved in:"
    )

    print(
        RESULTS_DIR
    )

    return data


def main():

    if not BOOKSIM.exists():

        raise SystemExit(
            f"BookSim not found: {BOOKSIM}"
        )

    if not BASE_CONFIG.exists():

        raise SystemExit(
            f"Configuration not found: "
            f"{BASE_CONFIG}"
        )

    base_text = (
        BASE_CONFIG.read_text()
    )

    for traffic in TRAFFIC_PATTERNS:

        run_traffic(
            traffic,
            base_text
        )


if __name__ == "__main__":

    main()
