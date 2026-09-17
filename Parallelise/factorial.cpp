#include <iostream>
#include <iomanip>
#include <chrono>
#include <omp.h>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
using namespace boost::multiprecision;

// ============================================================
// Sequential Factorial
// ============================================================
cpp_int factorial_sequential(int n)
{
    cpp_int result = 1;

    for (int i = 1; i <= n; i++)
    {
        result *= i;
    }

    return result;
}

// ============================================================
// Parallel Factorial
// ============================================================
cpp_int factorial_parallel(int n)
{
    int num_threads = omp_get_max_threads();

    // Each thread gets its own partial result
    vector<cpp_int> partial(num_threads, 1);

    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();

        #pragma omp for
        for (int i = 1; i <= n; i++)
        {
            partial[thread_id] *= i;
        }
    }

    // Combine partial results
    cpp_int result = 1;

    for (int i = 0; i < num_threads; i++)
    {
        result *= partial[i];
    }

    return result;
}

// ============================================================
// Main
// ============================================================
int main()
{
    int n;

    cout << "Enter n: ";
    cin >> n;

    cout << "\nCalculating " << n << "!\n";

    // --------------------------------------------------------
    // Sequential
    // --------------------------------------------------------
    auto start_seq = chrono::high_resolution_clock::now();

    cpp_int seq_result = factorial_sequential(n);

    auto end_seq = chrono::high_resolution_clock::now();

    double seq_time =
        chrono::duration<double, milli>(end_seq - start_seq).count();


    // --------------------------------------------------------
    // Parallel
    // --------------------------------------------------------
    auto start_par = chrono::high_resolution_clock::now();

    cpp_int par_result = factorial_parallel(n);

    auto end_par = chrono::high_resolution_clock::now();

    double par_time =
        chrono::duration<double, milli>(end_par - start_par).count();


    // --------------------------------------------------------
    // Verify results
    // --------------------------------------------------------
    bool correct = (seq_result == par_result);

    // --------------------------------------------------------
    // Statistics
    // --------------------------------------------------------
    double speedup = seq_time / par_time;

    int threads = omp_get_max_threads();

    // --------------------------------------------------------
    // Output
    // --------------------------------------------------------
    cout << "\n================ RESULTS ================\n";

    cout << "n                  : " << n << endl;
    cout << "Threads available  : " << threads << endl;

    cout << "\nSequential result   : " << seq_result << endl;
    cout << "Parallel result     : " << par_result << endl;

    cout << "\nResults match       : "
         << (correct ? "YES" : "NO") << endl;

    cout << fixed << setprecision(6);

    cout << "\nSequential time     : "
         << seq_time << " ms" << endl;

    cout << "Parallel time       : "
         << par_time << " ms" << endl;

    cout << "Speedup             : "
         << speedup << "x" << endl;

    cout << "Efficiency          : "
         << (speedup / threads) * 100
         << "%" << endl;

    cout << "==========================================\n";

    return 0;
}