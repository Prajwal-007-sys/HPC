#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// 3x3 Sobel Templates (Gx and Gy)
const int Gx[3][3] = {
    {-1, 0, 1},
    {-2, 0, 2},
    {-1, 0, 1}
};

const int Gy[3][3] = {
    {-1, -2, -1},
     {0,  0,  0},
     {1,  2,  1}
};

int main() {
    int n = 6;              // Matrix dimension (n x n)
    double t_c = 1e-9;      // Time per multiply-add operation (e.g., 1 nanosecond)

    // 1. Allocate n x n image and output matrices
    int **image = (int **)malloc(n * sizeof(int *));
    int **edges = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        image[i] = (int *)malloc(n * sizeof(int));
        edges[i] = (int *)calloc(n, sizeof(int));
    }

    // 2. Initialize matrix with a bright central box
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            if (r >= 2 && r <= 3 && c >= 2 && c <= 3) {
                image[r][c] = 200; // Bright object
            } else {
                image[r][c] = 10;  // Dark background
            }
        }
    }

    // 3. Apply 3x3 templates (Edge Detection)
    for (int r = 1; r < n - 1; r++) {
        for (int c = 1; c < n - 1; c++) {
            int val_x = 0;
            int val_y = 0;

            // Apply 3x3 Gx and Gy templates (9 multiply-add ops each = 18 total)
            for (int kr = -1; kr <= 1; kr++) {
                for (int kc = -1; kc <= 1; kc++) {
                    int pixel = image[r + kr][c + kc];
                    val_x += pixel * Gx[kr + 1][kc + 1];
                    val_y += pixel * Gy[kr + 1][kc + 1];
                }
            }

            int magnitude = (int)sqrt(val_x * val_x + val_y * val_y);
            edges[r][c] = magnitude > 255 ? 255 : magnitude;
        }
    }

    // 4. Calculate Serial Execution Time
    double total_pixels = (double)n * n;
    double Ts_single = 9.0 * total_pixels * t_c;  // 1 Template
    double Ts_both   = 18.0 * total_pixels * t_c; // Both Gx and Gy Templates

    // Display Input Matrix
    printf("--- Input Image Matrix (%dx%d) ---\n", n, n);
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            printf("%4d ", image[r][c]);
        }
        printf("\n");
    }

    // Display Output Matrix
    printf("\n--- Detected Edges Matrix ---\n");
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            printf("%4d ", edges[r][c]);
        }
        printf("\n");
    }

    // Display Theoretical Time Results
    printf("\n--- Serial Execution Time Metrics ---\n");
    printf("Total Pixels (n^2)             : %.0f\n", total_pixels);
    printf("Single Template Time (9*n^2*tc): %e seconds\n", Ts_single);
    printf("Both Templates Time (18*n^2*tc): %e seconds\n", Ts_both);

    // Clean up memory
    for (int i = 0; i < n; i++) {
        free(image[i]);
        free(edges[i]);
    }
    free(image);
    free(edges);

    return 0;
}