#define CPU_ZERO
#define CPU_SET

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>

#define NUM_THREADS 4 // Number of threads to use

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct {
    int tid;
    Node** head;
    int* Numbers;
    int num;
} ThreadArgs;

void readRollNumbers(FILE* inputFile, int* Numbers, int num) {
    for (int i = 0; i < num; i++) {
        fscanf(inputFile, "%d", &Numbers[i]);
    }
}

void addRollNumbersToList(Node** head, int* Numbers, int num) {
    for (int i = 0; i < num; i++) {
        Node* newNode = (Node*)malloc(sizeof(Node));
        newNode->data = Numbers[i];
        newNode->next = *head;
        *head = newNode;
    }
}

void* addRollNumbersToListParallel(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    int num_per_thread = args->num / NUM_THREADS;
    int start_index = num_per_thread * args->tid;
    int end_index = (args->tid == NUM_THREADS - 1) ? args->num : start_index + num_per_thread;

    for (int i = start_index; i < end_index; i++) {
        Node* newNode = (Node*)malloc(sizeof(Node));
        newNode->data = args->Numbers[i];
        newNode->next = *(args->head);
        *(args->head) = newNode;
    }

    pthread_exit(NULL);
}

Node* merge(Node* left, Node* right) {
    Node* result = NULL;
    if (left == NULL)
        return right;
    else if (right == NULL)
        return left;

    if (left->data <= right->data) {
        result = left;
        result->next = merge(left->next, right);
    } else {
        result = right;
        result->next = merge(left, right->next);
    }
    return result;
}

void split(Node* source, Node** frontRef, Node** backRef) {
    Node* fast;
    Node* slow;
    slow = source;
    fast = source->next;

    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
}

Node* mergeSort(Node* head) {
    Node* front;
    Node* back;
    if (head == NULL || head->next == NULL) {
        return head;
    }

    split(head, &front, &back);

    front = mergeSort(front);
    back = mergeSort(back);

    return merge(front, back);
}

void* mergeSortParallel(void* arg) {
    Node** head = (Node**)arg;
    *head = mergeSort(*head);
    pthread_exit(NULL);
}

void setAffinity(pthread_t thread, int coreId) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(coreId, &cpuset);
    pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
}

void printList(Node* head) {
    while (head != NULL) {
        printf("%d ", head->data);
        head = head->next;
    }
    printf("\n");
}

int main() {
    FILE* inputFile = fopen("/home/onebit/Documents/Numbers.txt", "r");
    if (inputFile == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    int num = 10;
    int Numbers[num];
    readRollNumbers(inputFile, Numbers, num);
    fclose(inputFile);

    Node* head = NULL;

    pthread_t threads[NUM_THREADS];
    ThreadArgs args[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        args[i].tid = i;
        args[i].head = &head;
        args[i].Numbers = Numbers;
        args[i].num = num;
        pthread_create(&threads[i], NULL, addRollNumbersToListParallel, (void*)&args[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Unsorted List: ");
    printList(head);

    for (int i = 0; i < NUM_THREADS; i++) {
        setAffinity(threads[i], i % NUM_THREADS);
    }

    pthread_t merge_thread;
    pthread_create(&merge_thread, NULL, mergeSortParallel, (void*)&head);
    pthread_join(merge_thread, NULL);

    printf("Sorted List: ");
    printList(head);

    return 0;
}