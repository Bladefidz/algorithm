#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


/* Global variable declarations */

// Define type of item of node
typedef int ItemType;

// Define type of node
typedef struct node Node;

// Define structure called stack
typedef struct {
    Node *TOS;
} Stack;

// Declare structure called node
struct node {
    ItemType item;
    Node *next;
};


/* Prototypes declarations */
Stack *init();
int isEmpty(Stack *stack);
void push(Stack **stack, ItemType newValue);
struct Node *pop(Stack **stack);
void printStack(Stack *stack);
int removeStack(Stack *stack);


/* Main program */
/**
 * Main program call
 * @param  argc argument count
 * @param  argv argument variables
 * @return      int
 */
int main(int argc, char const *argv[])
{
    char inputChar;
    int inputInt;
    int nodeCount;
    Stack *stack = init();
    Node *tmp = NULL;

    printf("#################################################\n"
        "STACK OF LINKED LIST\n\nAvailable options:\n"
        "1. Push\n"
        "2. Pop\n"
        "3. Print\n");

    do
    {
        printf("\nChoose an option: ");
        scanf(" %c", &inputChar);
        printf("\n");

        if (inputChar == '1') {
            // Do push

            // Get new node value from stdin
            printf("Input new node value: ");
            scanf("%d", &inputInt);

            push(&stack, inputInt);

            nodeCount = nodeCount + 1;
        } else if (inputChar == '2') {
            // Do pop
            tmp = pop(&stack);

            printf("Popped value of node: %d\n", (*tmp).item);

            free(tmp);

            nodeCount = nodeCount - 1;
        } else if (inputChar == '3') {
            // Do print

            printStack(stack);
        } else {
            printf("Warning: Wrong input option!\n");
        }

        printf("\nContinue to run? (Y/n): ");
        scanf(" %c", &inputChar);
        printf("\n");
    } while (inputChar == 'y' || inputChar == 'Y');

    printf("Freeing stack.....\n");
    if (removeStack(stack)) {
        printf("Stack was freed.\n");
    } else {
        printf("Warning: Stack already empty\n");
    }

    return 0;
}

/**
 * Initialize new stack
 * @param stack Pointer into stack address
 * @return      Pointer of empty stack
 */
Stack *init() {
    Stack *newStack = (Stack *) malloc(sizeof(Stack));
    (*newStack).TOS = NULL;

    return newStack;
}

/**
 * Check either stack is empty or not
 * @param  stack Pointer of stack
 * @return       Boolean
 */
int isEmpty(Stack *stack) {
    if ((*stack).TOS == NULL) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * Push new value into stack
 * @param  stack    Type of stack
 * @param  newValue New value
 */
void push(Stack **stack, ItemType newValue) {
    // Allocate new node
    Node *newNode = (Node *) malloc (sizeof(Node));

    // Assign value into new node
    (*newNode).item = newValue;

    if (isEmpty(*stack)) {
        // Create new item
        (*newNode).next = NULL;
    } else {
        // Insert new item
        (*newNode).next = (**stack).TOS;
    }

    (**stack).TOS = newNode;
}

/**
 * Pop the head of stack
 * @param  stack Type of stack
 * @return       Node
 */
struct Node *pop(Stack **stack) {
    if (isEmpty(*stack)) {
        return NULL;
    } else {
        Node *tmp = (**stack).TOS;
        (**stack).TOS = (*(**stack).TOS).next;
        return tmp;
    }
}

/**
 * Print every node in stack
 * @param stack Pointer of stack
 */
void printStack(Stack *stack) {
    Node *tmp;

    if (isEmpty(stack)) {
        printf("Warning: Stack is empty!\n");
    } else {
        tmp = (*stack).TOS;

        do {
            printf("%d\n", (*tmp).item);

            tmp = (*tmp).next;
        } while (tmp != NULL);
    }
}

/**
 * Free or remove every item in stack
 * @param  stack Pointer of stack
 * @return       Boolean
 */
int removeStack(Stack *stack) {
    Node *tmp;
    Node *next;

    if (isEmpty(stack)) {
        return 0;
    } else {
        tmp = (*stack).TOS;
        next = (*tmp).next;

        do {
            free(tmp);

            tmp = next;
            next = (*next).next;
        } while (next != NULL);

        return 1;
    }

    free(stack);
}