#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define s1 10
#define s2 100
#define s3 10000
#define s4 1000000
#define s5 10000000
#define s6 100000000
#define s7 1000000000

int main()
{
    int samples[] = {s1, s2, s3, s4, s5, s6, s7};
    int num_samples = 7;

    srand(time(NULL));

    for (int j = 0; j < num_samples; j++)
    {
        int s = samples[j];
        int count = 0;

        for (int i = 0; i < s; i++)
        {
            double x = -1.0 + 2.0 * rand() / RAND_MAX;
            double y = -1.0 + 2.0 * rand() / RAND_MAX;

            if (x * x + y * y <= 1.0)
            {
                count++;
            }
        }

        double p = (double)count / s;
        double pi = 4.0 * p;

        printf("s = %d, pi = %f\n", s, pi);
    }

    return 0;
}