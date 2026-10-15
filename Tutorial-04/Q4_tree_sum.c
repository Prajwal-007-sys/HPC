// #include <stdio.h>
// #include <stdlib.h>
// #include <omp.h>

// typedef struct Node {
//     int data;
//     struct Node *left;
//     struct Node *right;
// } Node;

// Node* createNode(int value) {
//     Node* newNode = (Node*)malloc(sizeof(Node));
//     newNode->data = value;
//     newNode->left = NULL;
//     newNode->right = NULL;
//     return newNode;
// }

// // Parallel Recursive Sum Function
// int sumTree(Node* root) {
//     if (root == NULL) return 0;

//     int left_sum = 0, right_sum = 0;

//     #pragma omp task shared(left_sum) firstprivate(root)
//     left_sum = sumTree(root->left);

//     #pragma omp task shared(right_sum) firstprivate(root)
//     right_sum = sumTree(root->right);

//     // Synchronize: Wait until both left and right tasks finish
//     #pragma omp taskwait

//     // Combine results (bottom-up sum return)
//     return root->data + left_sum + right_sum;
// }

// int main() {
//     Node* root = createNode(10);
//     root->left = createNode(5);
//     root->right = createNode(15);
//     root->left->left = createNode(2);
//     root->left->right = createNode(7);
//     root->right->right = createNode(20);

//     int total_sum = 0;

//     // Start the OpenMP parallel region with a single thread
//     #pragma omp parallel
//     {
//         #pragma omp single
//         {
//             total_sum = sumTree(root);
//         }
//     }

//     printf("Total Tree Sum: %d\n", total_sum);
//     return 0;
// }

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

typedef struct Node {
    int data;
    struct Node *left;
    struct Node *right;
} Node;

// Create node with a random integer value between 1 and 100
Node* createRandomNode() {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = (rand() % 100) + 1;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Build a complete binary tree of given depth recursively
Node* buildTree(int current_depth, int max_depth) {
    if (current_depth > max_depth) return NULL;

    Node* root = createRandomNode();
    root->left = buildTree(current_depth + 1, max_depth);
    root->right = buildTree(current_depth + 1, max_depth);
    return root;
}

// OpenMP Parallel Task-based Tree Sum
long long sumTreeParallel(Node* root) {
    if (root == NULL) return 0;

    long long left_sum = 0, right_sum = 0;

    #pragma omp task shared(left_sum) firstprivate(root)
    left_sum = sumTreeParallel(root->left);

    #pragma omp task shared(right_sum) firstprivate(root)
    right_sum = sumTreeParallel(root->right);

    #pragma omp taskwait

    return root->data + left_sum + right_sum;
}

// Free memory to prevent leaks
void freeTree(Node* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    srand((unsigned int)time(NULL));

    // Test different tree depths
    int depths[] = {10, 15, 20, 22};
    int num_tests = sizeof(depths) / sizeof(depths[0]);

    printf("===============================================================\n");
    printf("%-10s | %-15s | %-15s | %-15s\n", "Depth", "Total Nodes", "Tree Sum", "Execution Time");
    printf("===============================================================\n");

    for (int i = 0; i < num_tests; i++) {
        int depth = depths[i];
        long long total_nodes = (1LL << depth) - 1; // 2^depth - 1 nodes

        // Build the tree
        Node* root = buildTree(1, depth);

        // Benchmark Parallel Traversal
        double start_time = omp_get_wtime();
        
        long long total_sum = 0;
        #pragma omp parallel
        {
            #pragma omp single
            {
                total_sum = sumTreeParallel(root);
            }
        }

        double elapsed_time = omp_get_wtime() - start_time;

        printf("%-10d | %-15lld | %-15lld | %-15.6f s\n", 
               depth, total_nodes, total_sum, elapsed_time);

        freeTree(root);
    }
    return 0;
}