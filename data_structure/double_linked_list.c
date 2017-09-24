#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Global variable declarations */

/**
 * Global object of linked list structure
 */
struct Node
{
    struct Node *prev;
    int value;
    struct Node *next;
};

/* Prototypes declarations */
void printListForward(struct Node *ancestor);
void printListBackward(struct Node *ancestor);
struct Node *findNodeForward(struct Node *startNode, int value);
struct Node *findNodeBackward(struct Node *startNode, int value);
int prepend(struct Node **next, struct Node **newNode);
int append(struct Node **prev, struct Node **newNode);
int removeHead(struct Node **head);
int removeNode(struct Node **node);
int removeTail(struct Node **tail);
int removeList(struct Node *head);


/* Main program */
/**
 * Main program call
 * @param  argc argument count
 * @param  argv argument variables
 * @return      int
 */
int main(int argc, char const *argv[])
{
    char inputChar;  // ['char', EOF, NULL]
    int inputInt;
    int nodeCount = 0;  // Will use defensive programming using old increment
    struct Node *head = NULL;
    struct Node *tail = NULL;
    struct Node *prev = NULL;
    struct Node *next = NULL;
    struct Node *newNode = NULL;

    // Loop until user decided to terminate
    do {
        printf("################################################\n");
        printf("Single Linked List\n");
        printf("Store only an integer value in ascending order.\n\n");
        printf("Available operation options:\n");
        printf("1: Prepend as head.\n");
        printf("2: Append as tail.\n");
        printf("3: Prepend to a node.\n");
        printf("4: Append to a node.\n");
        printf("5: Remove head.\n");
        printf("6: Remove tail.\n");
        printf("7: Remove node.\n");
        printf("8: Print list forward.\n");
        printf("9: Print list backward.\n");

        printf("\nChoose an option: ");
        scanf(" %c", &inputChar);
        printf("\n");

        if (inputChar == '1' || inputChar == '2' ||
            inputChar == '3' || inputChar == '4'
        ) {
            // If prepend as head or append as tail or prepend to a node
            // or append to a node

            // Get new node value from stdin
            printf("Input new node value: ");
            scanf("%d", &inputInt);

            if (inputInt == 0) {
                printf("Error: Do not input zero!\n");
                removeList(head);
                exit(0);
            }

            // Allocate new node
            newNode = (struct Node *) malloc(sizeof(struct Node));

            if (newNode == NULL) {
                // New node allocation failed
                printf("Failed to allocate new node!\n");
            } else {
                // Modify new node properties
                (*newNode).prev = NULL;
                (*newNode).value = inputInt;
                (*newNode).next = NULL;

                // New node allocation success, then continue
                if (head == NULL) {
                    // List is empty

                    if (inputChar == '1') {
                        // prepend as new head
                        head = newNode;
                        nodeCount = nodeCount + 1;
                        printf("New head was created.\n");
                    } else {
                        // Can not execute others operations in empty list
                        printf("Warning: Your list is empty.\n");

                        // Freeing allocated memory
                        free(newNode);
                    }
                } else {
                    // At least one node existed in a list

                    if (inputChar == '1' || inputChar == '2') {
                        if ((*head).value == inputInt) {
                            printf("Warning: The value already at head.\n");

                            free(newNode);
                        } else {
                            if (inputChar == '1') {
                                // Prepend as new head
                                if ((*head).value > inputInt) {
                                    if (prepend(&head, &newNode) == 1) {
                                        head = newNode;
                                        nodeCount = nodeCount + 1;
                                        printf("Head was updated.\n");
                                    } else {
                                        // Maybe neither head or new node
                                        // lost its value
                                        printf("Warning: Neither target node "
                                            "nor node to append should not be "
                                            "empty.\n");
                                    }
                                } else {
                                    printf("Warning: The value bigger than "
                                        "head.\n");

                                    // Freeing allocated memory
                                    free(newNode);
                                }
                            } else if (inputChar == '2') {
                                // Append as new tail
                                if (tail == NULL) {
                                    // Tail is empty

                                    if (append(&head, &newNode) == 1) {
                                        // Append to head as new tail
                                        tail = newNode;
                                        nodeCount = nodeCount + 1;
                                        printf("New tail was created.\n");
                                    } else {
                                        // Maybe neither tail
                                        // or new node lost its value
                                        printf("Warning: Neither target node nor"
                                            "node to append should not be empty.\n");
                                    }
                                } else {
                                    if ((*tail).value >= inputInt) {
                                        if ((*tail).value == inputInt) {
                                            printf("Warning: The value "
                                                "already at tail.\n");
                                        } else {
                                            printf("Warning: The value "
                                                "lesser than tail.\n");
                                        }

                                        free(newNode);
                                    } else {
                                        if (append(&tail, &newNode)) {
                                            // Append to old tail as new tail
                                            tail = newNode;
                                            nodeCount = nodeCount + 1;
                                            printf("Tail was updated.\n");
                                        } else {
                                            // Maybe neither tail
                                            // or new node lost its value
                                            printf("Warning: Neither target node nor"
                                                "node to append should not be empty.\n");
                                        }
                                    }
                                }
                            } else {
                                // Nothing to do
                            }
                        }
                    } else if (inputChar == '3' || inputChar == '4') {
                        // Prepend to or append to or remove a node

                        if (inputChar == '3') {
                            printf("Try to prepend a new node in the sorted "
                                "ascending list.\n");
                        } else if (inputChar == '4') {
                            printf("Try to append a new node in the sorted "
                                "ascending list.\n");
                        } else {
                            printf("Error: A variable was corrupted!\n");
                            removeList(head);
                            exit(0);
                        }

                        if (inputChar == '3') {
                            // Prepend to a node

                            if ((*head).value > (*newNode).value) {
                                // Prepend to head

                                if (prepend(&head, &newNode) == 1) {
                                    // Prepend as new head
                                    head = newNode;
                                    nodeCount = nodeCount + 1;
                                    printf("Head was updated.\n");
                                } else {
                                    // Maybe neither head
                                    // or new node lost its value
                                    printf("Warning: Neither head nor"
                                            "node to append should not be "
                                            "empty.\n");
                                }
                            } else {
                                // Find a bigger node and prepend to it

                                // Initialize start node and its next node
                                prev = head;
                                next = (*head).next;

                                // Begin to searching forward
                                do {
                                    if ((*next).value >= (*newNode).value) {
                                        if ((*next).value > (*newNode).value) {
                                            if (prepend(&next, &newNode)) {
                                                // Prepend before bigger node
                                                nodeCount = nodeCount + 1;
                                                printf("New value was "
                                                    "prepended \n");
                                            } else {
                                                // Maybe neither target node
                                                // or new node lost its value
                                                printf("Warning: Neither "
                                                    "target node nor node to "
                                                    "append should not "
                                                    "be empty.\n");
                                            }
                                        } else {
                                            // Exactly same value has been
                                            // stored
                                            printf("Warning: Value %d has "
                                            "been saved in list.\n",
                                            inputInt);
                                        }

                                        // Stop the iteration
                                        break;
                                    } else {
                                        // Scanning to the next node.
                                        prev = next;
                                        next = (*next).next;
                                    }
                                } while(next != NULL);
                            }
                        } else if (inputChar == '4') {
                            // Append to a node

                            if (tail != NULL) {
                                // Tail is empty

                                if ((*tail).value < (*newNode).value) {
                                    // Append to tail

                                    if (append(&tail, &newNode) == 1) {
                                        // Append as new tail
                                        tail = newNode;
                                        nodeCount = nodeCount + 1;
                                        printf("Tail was updated.\n");
                                    } else {
                                        // Argh, i was bored writing comment
                                        // in this section. Its have been
                                        // clear right? :P
                                        printf("Warning: Neither tail nor "
                                            "node to append should not "
                                            "be empty.\n");
                                    }
                                } else {
                                    // Find a lesser node and append to it

                                    // Initialize start node and its next node
                                    prev = tail;
                                    next = (*tail).next;

                                    // Begin to searching backward
                                    do {
                                        if (
                                            (*prev).value <= (*newNode).value
                                        ) {
                                            if (
                                                (*prev).value <
                                                (*newNode).value
                                            ) {
                                                if (append(&prev, &newNode)) {
                                                    nodeCount = nodeCount + 1;
                                                    printf("New value was "
                                                        "appended \n");
                                                } else {
                                                    printf("Warning: Neither "
                                                        "target node nor node "
                                                        "to append should not "
                                                        "be empty.\n");
                                                }
                                            } else {
                                                printf("Warning: Value has "
                                                    "been saved in list.\n");
                                            }

                                            break;
                                        } else {
                                            // Scanning to the previous node.
                                            next = prev;
                                            prev = (*prev).prev;
                                        }
                                    } while(prev != NULL);
                                }
                            } else {
                                // Tail is empty, so check value of head

                                if ((*head).value == (*newNode).value) {
                                    // Head has same value with the new node
                                    printf("Warning: "
                                        "Value %d has been saved in list.\n",
                                        inputInt);
                                } else {
                                    // Try to append to head
                                    if ((*head).value < (*newNode).value) {
                                        if (append(&head, &newNode) == 1) {
                                            head = newNode;
                                            nodeCount = nodeCount + 1;
                                            printf("Head was updated.\n");
                                        } else {
                                            printf("Warning: Neither head nor "
                                            "node to append should not "
                                            "be empty.\n");
                                        }
                                    } else {
                                        printf("Warning: Your head bigger "
                                            "than new node, please use "
                                            "prepend instead.\n");
                                    }
                                }
                            }
                        } else {
                            // This is very rare case where we lost value of
                            // certain variable in memory, so just exit.
                            printf("Error: A variable was corrupted!\n");
                            removeList(head);
                            exit(0);
                        }
                    } else {
                        // Do nothing
                    }
                }
            }
        } else {
            if (inputChar == '5') {
                // Remove head
                if (removeHead(&head)) {
                    nodeCount = nodeCount - 1;
                    printf("Old head has been removed and the next node "
                        "become a new head\n");
                } else {
                    printf("Warning: We lost head.\n");
                }
            } else if (inputChar == '6') {
                // Remove tail
                if (removeTail(&tail)) {
                    nodeCount = nodeCount - 1;
                    printf("Old tail has been removed and "
                        "the previous node become a new tail.\n");
                } else {
                    printf("Warning: Tail is empty.\n");
                }
            } else if (inputChar == '7') {
                // Copy new node value from stdin into temporary

                printf("Input value of node to be removed: ");

                // Get value of node to find from stdin
                scanf("%d", &inputInt);

                // Remove a node
                if (inputInt >= (*head).value) {
                    // Node to find has bigger value than
                    // or equal to head

                    if (inputInt == (*head).value) {
                        // Remove head
                        if (removeHead(&head)) {
                            nodeCount = nodeCount - 1;
                            printf("Old head has been removed and "
                                "the previous node become a new "
                                "head\n");
                        } else {
                            printf("Warning: We lost head.\n");
                        }
                    } else {
                        if (inputInt <= (*tail).value) {
                            // Node to find has lesser value than
                            // or equal to tail

                            if (inputInt == (*tail).value) {
                                // Remove tail
                                if (removeTail(&tail)) {
                                    nodeCount = nodeCount - 1;
                                    printf("Old tail has been "
                                        "removed and the previous "
                                        "node become a new tail"
                                        ".\n");
                                } else {
                                    printf("Warning: We lost tail."
                                        "\n");
                                }
                            } else {
                                // Because data likely to grow,
                                // Check from backward is priority

                                if (
                                    ((*tail).value - inputInt) <=
                                    (inputInt - (*head).value)
                                ) {
                                    next = findNodeBackward(
                                        (*tail).prev,
                                        inputInt
                                    );

                                    if (removeNode(&next)) {
                                        printf("A node in between has been "
                                            "removed.\n");
                                    } else {
                                        printf("Failed to remove node.\n");
                                    }
                                } else {
                                    prev = findNodeForward(
                                        (*head).next,
                                        inputInt
                                    );

                                    if (removeNode(&prev)) {
                                        printf("A node in between has been "
                                            "removed.\n");
                                    } else {
                                        printf("Failed to remove node.\n");
                                    }
                                }
                            }
                        } else {
                            // Node to find has bigger value
                            // than tail
                            printf("Warning: Value not found in "
                                "any nodes\n");
                        }
                    }
                } else {
                    // Node to find has lesser value than head
                    printf("Warning: Value not found in any "
                        "nodes\n");
                }
            } else if (inputChar == '8') {
                // Print list in forward direction
                printListForward(head);
            } else if (inputChar == '9') {
                // Print list in backward direction
                printListBackward(tail);
            } else {
                // User has been input wrong option
                printf("Warning: Invalid option\n");
            }
        }

        printf("\n");
        printf("Continue to run? (Y/n): ");
        scanf(" %c", &inputChar);
        printf("\n");
    } while ((inputChar == 'y') || (inputChar == 'Y'));

    /* Freeing allocated variables */
    printf("Freeing all nodes in list.\n");

    if (removeList(head)) {
        printf("All nodes in list have been freed.\n");
    } else {
        printf("List already empty.\n");
    }

    return 0;
}


/* Function declarations */

/**
 * Print all list item in forward direction
 * @param head Head node of linked list
 */
void printListForward(struct Node *ancestor) {
    if (ancestor != NULL) {
        do {
            printf("%d->", (*ancestor).value);
            ancestor = (*ancestor).next;
        } while (ancestor != NULL);
    } else {
        printf("Warning: Start node is empty\n");
    }
}

/**
 * Print all list item in backward direction
 * @param head Head node of linked list
 */
void printListBackward(struct Node *ancestor) {
    if (ancestor != NULL) {
        do {
            printf("%d->", (*ancestor).value);
            ancestor = (*ancestor).prev;
        } while (ancestor != NULL);
    } else {
        printf("Warning: Last node is empty\n");
    }
}

/**
 * Searching in forward direction to find a node which
 * has property of value matched with value to find.
 * @param  startNode A node to start searching
 * @param  value     Value to find
 * @return           Object
 */
struct Node *findNodeForward(struct Node *startNode, int value) {
    struct Node *tmp = startNode;
    int found = 0;

    if (startNode != NULL) {
        do {
            if ((*tmp).value == value) {
                found = 1;
                break;
            }

            tmp = (*tmp).next;
        } while (tmp != NULL);

        if (found == 1) {
            return tmp;
        } else {
            return NULL;
        }
    } else {
        return NULL;
    }
}

/**
 * Searching in backward direction to find a node which
 * has property of value matched with value to find.
 * @param  startNode A node to start searching
 * @param  value     Value to find
 * @return           Object
 */
struct Node *findNodeBackward(struct Node *startNode, int value) {
    struct Node *tmp = startNode;
    int found = 0;

    if (startNode != NULL) {

        do {
            if ((*tmp).value == value) {
                found = 1;
                break;
            }

            tmp = (*tmp).prev;
        } while (tmp != NULL);

        if (found == 1) {
            return tmp;
        } else {
            return NULL;
        }
    } else {
        return NULL;
    }
}

/**
 * Prepend before specific node
 * @param next    Next node
 * @param newNode New node
 */
int prepend(struct Node **next, struct Node **newNode) {
    if (next != NULL && newNode != NULL) {
        if ((**next).prev != NULL) {
            (**newNode).prev = (**next).prev;
            (*(**next).prev).next = (*newNode);
        } else {
            // Just for ensuring not circular.
            (**newNode).prev = NULL;
        }

        (**next).prev = (*newNode);
        (**newNode).next = (*next);

        return 1;
    } else {
        return 0;
    }
}

/**
 * Append to specific node
 * @param prev    Previous node
 * @param newNode new node
 */
int append(struct Node **prev, struct Node **newNode) {
    if (prev != NULL && newNode != NULL) {

        if ((**prev).next != NULL) {
            (**newNode).next = (**prev).next;
            (*(**prev).next).prev = (*newNode);
        } else {
            (**newNode).next = NULL;
        }

        (**prev).next = (*newNode);
        (**newNode).prev = (*prev);

        return 1;
    } else {
        return 0;
    }
}

/**
 * Remove first node
 * @param head First node
 */
int removeHead(struct Node **head) {
    struct Node *tmp = (*head);

    if (tmp != NULL) {
        if ((*tmp).next != NULL) {
            (*head) = (*tmp).next;
            (**head).prev = NULL;
        } else {
            // List became empty
            head = NULL;
        }

        free(tmp);

        return 1;
    } else {
        return 0;
    }
}

/**
 * Remove a node
 * @param node Node to remove
 */
int removeNode(struct Node **node) {
    struct Node *tmp = (*node);

    if (node != NULL) {

        if ((**node).prev != NULL) {
            (*(**node).prev).next = ((**node).next);
        } else {
            (*(**node).next).prev = NULL;
        }

        if ((**node).next != NULL) {
            (*(**node).next).prev = ((**node).prev);
        } else {
            (*(**node).prev).next = NULL;
        }

        free(tmp);

        return 1;
    } else {
        return 0;
    }
}

/**
 * Remove tail node
 * @param tail Last node
 */
int removeTail(struct Node **tail) {
    struct Node *tmp = (*tail);
    int success = 0;

    if (tail != NULL) {
        if ((*tmp).prev != NULL) {
            (*tail) = (*tmp).prev;
            (**tail).next = NULL;

            success = 1;
        } else {
            // Programmer logic fault let head empty
            success = 0;
        }

        free(tmp);
    } else {
        success = 0;
    }

    return success;
}

/**
 * Remove all allocated nodes in list
 * @param head Head of list
 */
int removeList(struct Node *head) {
    struct Node *node = head;

    if (node != NULL) {
        do {
            free(head);

            head = node;
            node = (*head).next;
        } while (node != NULL);

        return 1;
    } else {
        return 0;
    }
}