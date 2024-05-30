#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

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

    int num = 10; // Assuming 10 numbers in the file
    int Numbers[num];

    readRollNumbers(inputFile, Numbers, num);
    fclose(inputFile);

    Node* head = NULL;

    addRollNumbersToList(&head, Numbers, num);

    printf("Unsorted List: ");
    printList(head);

    head = mergeSort(head);

    printf("Sorted List: ");
    printList(head);

    return 0;
}