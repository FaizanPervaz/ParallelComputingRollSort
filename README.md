# ParallelComputingRollSort
Parallel and Serial implementations of a roll number sorting algorithm using linked lists in C, showcasing the use of pthreads and CPU affinity for performance optimization.

## Features
- **Serial Implementation:** Read numbers from a file, store them in an array, and sort them using merge sort implemented on a linked list.
- **Parallel Implementation:** Utilize multiple pthreads to handle different portions of data concurrently, incorporating CPU affinity to enhance performance.

## Serial Functions
- `void readRollNumbers(FILE* inputFile, int* Numbers, int num);`
- `void addRollNumbersToList(Node** head, int* Numbers, int num);`
- `Node* mergeSort(Node* head);`

## Parallel Functions with CPU Affinity
- `void* addRollNumbersToListParallel(void* arg);`
- `void* mergeSortParallel(void* arg);`
- `void setAffinity(pthread_t thread, int coreId);`

## Setup and Compilation
Ensure you have a C compiler and pthreads library installed on your system. Clone this repository and compile the programs using the following commands:

## Testing and Analysis
Measure and analyze the execution time for both serial and parallel implementations under various conditions to evaluate the effectiveness of threading and CPU affinity:

## Execution Time Measurement: 
Compare the performance with varying input sizes and thread counts.

## CPU Affinity Impact Analysis: 
Discuss how specific CPU-core bindings influence performance, considering aspects like cache locality and context switching.

```bash
git clone https://github.com/yourusername/ParallelComputingRollSort.git
cd ParallelComputingRollSort
gcc -o program Q1.c -lpthread  // For serial implementation
gcc -o program Q2.c -lpthread  // For parallel implementation
Execute the compiled programs as follows:
./program


