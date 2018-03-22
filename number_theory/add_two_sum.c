#include <stdio.h>
#include <stdlib.h>

/**
 * You are given two non-empty linked lists representing two non-negative integers.
 * The digits are stored in reverse order and each of their nodes contain a single digit.
 * Add the two numbers and return it as a linked list.
 * 
 * You may assume the two numbers do not contain any leading zero, except the number 0 itself.
 * Example
 *  Input: (2 -> 4 -> 3) + (5 -> 6 -> 4)
 *  Output: 7 -> 0 -> 8
 *  Explanation: 342 + 465 = 807.
 */

struct ListNode
{
    int val;
    struct ListNode *next;
};

#include <stdio.h>

struct ListNode *addTwoNumbers(struct ListNode *l1, struct ListNode *l2)
{
    struct ListNode *l1Tmp = l1;
    struct ListNode *l2Tmp = l2;
    struct ListNode *sum = (struct ListNode *)malloc(sizeof(struct ListNode));
    struct ListNode *sumTmp;
    int reminder = 0;

    sumTmp = sum;

    while (true)
    {
        sumTmp->val = l1Tmp->val + l2Tmp->val + reminder;
        //printf("%d + %d + %d = %d\n", l1Tmp->val, l2Tmp->val, reminder, sumTmp->val);

        reminder = 0;
        if (sumTmp->val > 9)
        {
            // calculate reminder and add only modulo of 10
            reminder = 1;
            sumTmp->val = sumTmp->val % 10;
        }

        struct ListNode *new = (struct ListNode *)malloc(sizeof(struct ListNode));

        if (l1Tmp->next != NULL && l2Tmp->next != NULL)
        {
            sumTmp->next = new;
            l1Tmp = l1Tmp->next;
            l2Tmp = l2Tmp->next;
        }
        else
        {
            if (l1Tmp->next != NULL)
            {
                //printf("l1Tmp not null\n");
                l1Tmp = l1Tmp->next;
                sumTmp->next = new;
                sumTmp = sumTmp->next;
                while (true)
                {
                    //printf("%d\n", l1Tmp->val);
                    sumTmp->val = l1Tmp->val + reminder;
                    reminder = 0;
                    if (sumTmp->val > 9)
                    {
                        // calculate reminder and add only modulo of 10
                        reminder = 1;
                        sumTmp->val = sumTmp->val % 10;
                    }
                    if (l1Tmp->next != NULL)
                    {
                        struct ListNode *new1 = (struct ListNode *)malloc(sizeof(struct ListNode));
                        sumTmp->next = new1;
                        sumTmp = sumTmp->next;
                        l1Tmp = l1Tmp->next;
                    }
                    else
                    {
                        if (reminder > 0)
                        {
                            struct ListNode *new1 = (struct ListNode *)malloc(sizeof(struct ListNode));
                            new1->val = 1;
                            new1->next = NULL;
                            sumTmp->next = new1;
                            sumTmp = sumTmp->next;
                        }
                        break;
                    }
                }
                sumTmp->next = NULL;
                break;
            }
            else if (l2Tmp->next != NULL)
            {
                //printf("l2Tmp not null\n");
                l2Tmp = l2Tmp->next;
                sumTmp->next = new;
                sumTmp = sumTmp->next;
                while (true)
                {
                    //printf("%d\n", l2Tmp->val);
                    sumTmp->val = l2Tmp->val + reminder;
                    reminder = 0;
                    if (sumTmp->val > 9)
                    {
                        // calculate reminder and add only modulo of 10
                        reminder = 1;
                        sumTmp->val = sumTmp->val % 10;
                    }
                    if (l2Tmp->next != NULL)
                    {
                        struct ListNode *new1 = (struct ListNode *)malloc(sizeof(struct ListNode));
                        sumTmp->next = new1;
                        sumTmp = sumTmp->next;
                        l2Tmp = l2Tmp->next;
                    }
                    else
                    {
                        if (reminder > 0)
                        {
                            struct ListNode *new1 = (struct ListNode *)malloc(sizeof(struct ListNode));
                            new1->val = 1;
                            new1->next = NULL;
                            sumTmp->next = new1;
                            sumTmp = sumTmp->next;
                        }
                        break;
                    }
                }
                sumTmp->next = NULL;
                break;
            }
            else
            {
                if (reminder > 0)
                {
                    new->val = 1;
                    new->next = NULL;
                    sumTmp->next = new;
                }
                else
                {
                    sumTmp->next = NULL;
                }
                break;
            }
        }

        sumTmp = sumTmp->next;
    }

    return sum;
}

/**
 * Main program
 */
int main(int argc, char const *argv[])
{
    return 0;
}