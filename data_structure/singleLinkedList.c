#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Global object of linked list structure
 */
struct Node
{
    int value;
    struct Node *next;
};

/* Prototypes declarations */
struct Node* createNewNode(int value);
void printList(struct Node *head);
void push(struct Node** head, struct Node** newNode);
void append(struct Node** prev, struct Node** newNode);
void removeHead(struct Node** head);
void removeAfter(struct Node** prev);


/* Main program */
/**
 * Main program call
 * @param  argc argument count
 * @param  argv argument variables
 * @return      int
 */
int main(int argc, char const *argv[])
{
    char inputChar; // ['char', EOF, NULL]
    int inputInt;
    struct Node* head = NULL;
    struct Node *prev = NULL;
    struct Node *next = NULL;

    // Loop until user decided to terminate
    do {
        printf("Single Linked List\n");
        printf("Store only an integer value in ascending order.\n\n");
        printf("Available operation options:\n");
        printf("1: Insertion operation.\n");
        printf("2: Deletion operation.\n");
        printf("3: Print operation.\n");

        printf("\nChoose an option: ");
        scanf(" %c", &inputChar);
        printf("\n");

        if (inputChar == '1') {
            // Insertion
            printf("Input a new node value: ");
            scanf("%d", &inputInt);
        } else if (inputChar == '2') {
            // Deletion
            printf("Input a node value will be deleted: ");
            scanf("%d", &inputInt);
        } else if (inputChar == '3') {
            // Print
            printList(head);
            printf("\n");
        } else {
            printf("Error: Option not valid.\n");
        }

        if (inputChar == '1' || inputChar == '2') {
            if (head == NULL) {
                if (inputChar == '1') {
                    struct Node* newNode = createNewNode(inputInt);

                    push(&head, &newNode);
                } else if (inputChar == '2') {
                    printf("Head is empty.\n");
                }
            } else {
                if (inputChar == '1') {
                    prev = NULL;
                    next = head;

                    while (next != NULL) {
                        prev = next;
                        next = (*prev).next;

                        if ((*prev).value == inputInt) {
                            printf("Value %d has been saved in list.\n",
                                inputInt);
                            break;
                        }

                        if ((*prev).value < inputInt) {
                            struct Node *newNode = createNewNode(inputInt);

                            if (next == NULL || ((*next).value > inputInt)) {
                                // Insert as tail if next==NULL.
                                // Else, insert in between
                                append(&prev, &newNode);
                                break;
                            }
                        }
                    }
                } else if (inputChar == '2') {
                    if ((*head).value == inputInt) {
                        removeHead(&head);
                    } else {
                        prev = head;
                        next = (*head).next;

                        while (next != NULL) {
                            if ((*next).value == inputInt) {
                                removeAfter(&prev);
                                break;
                            }

                            prev = next;
                            next = (*next).next;
                        }
                    }
                }
            }
        }

        printf("Continue to run? (Y/n): ");
        scanf(" %c", &inputChar);
        printf("\n");
    } while ((inputChar == 'y') || (inputChar == 'Y'));

    free(head);

    return 0;
}


/* Function declarations */

/**
 * Create a new list
 * Because function has its own scope, so any created variable inside it
 * stored in head and disappear as long as function return.
 * So, we create a function which pointing into new variable and will return
 * new created address in memory.
 *
 * @param  value Integer value of list
 * @return       Structure
 */
struct Node* createNewNode(int value) {
    struct Node* newNode = (struct Node *) malloc(sizeof(struct Node));

    if (newNode == NULL) {
        return NULL;
    } else {
        (*newNode).value = value;
    }

    return newNode;
}

/**
 * Print all list item
 * @param head Head node of linked list
 */
void printList(struct Node *head) {
    if (head != NULL) {
        do {
            printf("%d->", (*head).value);
            head = (*head).next;
        } while (head != NULL);
    } else {
        printf("Error: List is empty\n");
    }
}

/**
 * Insert new value as head in linked list.
 * @param head Head of linked list
 */
void push(struct Node** head, struct Node** newNode) {
    if ((*head) != NULL) {
        (**newNode).next = (*head);
        (*head) = (*newNode);
    } else {
        (*head) = (*newNode);
        (**head).next = NULL;
    }
}

/**
 * Remove head of linked list and update the head with its next node
 * @param head Head of linked list
 */
void removeHead(struct Node** head) {
    if (head != NULL) {
        struct Node *tmp = (*head);
        (*head) = (*tmp).next;
        free(tmp);
    } else {
        printf("Error: Head is empty.\n");
    }
}

/**
 * Remove node after specific node
 * @param prev         Previous node
 */
void removeAfter(struct Node** prev) {
    if (prev != NULL) {
        struct Node *tmp = ((**prev).next);

        (**prev).next = (*(**prev).next).next;

        free(tmp);
    } else {
        printf("Error: Previous node is empty.\n");
    }
}

/**
 * Insert new node after specific node
 * @param prev       [description]
 * @param newNode    [description]
 */
void append(struct Node** prev, struct Node** newNode) {
    if (prev != NULL) {
        (**newNode).next = (**prev).next;
        (**prev).next = (*newNode);
    } else {
        printf("Error: Previous node is empty.\n");
    }
}