// #include<iostream>
// #include <vector>
// #include <cmath>
// #include <algorithm>
// #include <random>
// #include <iomanip>

// using namespace std;
// using Matrix = vector<vector<int>>;

// const int ROWS = 6;
// const int COLS = 6;

// // Sobel 3x3 convolution kernels
// const int Gx[3][3] = {
//     {-1, 0, 1},
//     {-2, 0, 2},
//     {-1, 0, 1}
// };

// const int Gy[3][3] = {
//     {-1, -2, -1},
//     { 0,  0,  0},
//     { 1,  2,  1}
// };

// int main() {
//     //std::mt19937 rng(42);  //Mersenne Twister pseudo-random number generator //But, this gives uniform distribution
//     Matrix gray(ROWS, vector<int>(COLS, 10)); // 10 is intial value of the elements
//     gray[2][2] = 200; gray[2][3] = 200;
//     gray[3][2] = 200; gray[3][3] = 200;

//     // 
//     Matrix edges(ROWS, vector<int>(COLS, 0)); // 0 is the initial value of the elements

//     for (int r = 1; r < ROWS - 1 ; ++r) {
//         for (int c = 1;c < COLS - 1; ++c) {
//             int val_x = 0;
//             int val_y = 0;

//             for (int kr = -1; kr <= 1; ++kr) {
//                 for
//             }
//         }
//     }

// }


#include<iostream>
#include<vector>

using namespace std;

using Matrix = vector<vector<int>>;

const int ROWS = 6;
const int COLS = 6;

int main() {
    Matrix gray(ROWS, vector<int>(COLS, 10));
    gray[2][2] = 200; gray[2][3] = 200;
    gray[3][2] = 200; gray[3][3] = 200;

    Matrix conv(3, vector<int>(3, ))
}