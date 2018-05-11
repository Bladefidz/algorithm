/*
typedef struct ListNode {
      int val;
      struct ListNode *next;
} node;

*/

typedef struct ListNode node;

node *
addTwoNumbers(node *l1, node *l2)
{
    int carry = 0;
    int sum;

    node *l1_tmp = l1;
    node *l2_tmp = l2;
    node *ret_head = l2;

    if (l1 == NULL)
    {
        return l2;
    }
    else if (l2 == NULL)
    {
        return l1;
    }

    while (l1_tmp->next != NULL && l2_tmp->next != NULL)
    {
        sum = l1_tmp->val + l2_tmp->val + carry;
        carry = sum / 10;
        sum = sum % 10;
        l2_tmp->val = sum;
        l2_tmp = l2_tmp->next;
        l1_tmp = l1_tmp->next;
    }

    if (l1_tmp->next == NULL && l2_tmp->next != NULL)
    {
        sum = l1_tmp->val + l2_tmp->val + carry;
        carry = sum / 10;
        sum = sum % 10;
        l2_tmp->val = sum;
        l2_tmp = l2_tmp->next;

        while (l2_tmp->next != NULL && carry > 0)
        {
            sum = l2_tmp->val + carry;
            carry = sum / 10;
            sum = sum % 10;
            l2_tmp->val = sum;
            l2_tmp = l2_tmp->next;
        }

        if (carry == 0)
        {
            goto out;
        }

        sum = l2_tmp->val + carry;
        carry = sum / 10;
        l2_tmp->val = sum % 10;

        if (carry > 0)
        {
            node *t_node = (node *)malloc(sizeof(node));
            t_node->val = 1;
            t_node->next = NULL;
            l2_tmp->next = t_node;
        }
    }
    else if (l1_tmp->next != NULL && l2_tmp->next == NULL)
    {
        sum = l1_tmp->val + l2_tmp->val + carry;
        carry = sum / 10;
        sum = sum % 10;
        l2_tmp->val = sum;
        l1_tmp = l1_tmp->next;
        l2_tmp->next = l1_tmp;

        while (l1_tmp->next != NULL && carry > 0)
        {
            sum = l1_tmp->val + carry;
            carry = sum / 10;
            sum = sum % 10;
            l1_tmp->val = sum;
            l1_tmp = l1_tmp->next;
        }

        if (carry == 0)
        {
            goto out;
        }

        sum = l1_tmp->val + carry;
        carry = sum / 10;
        l1_tmp->val = sum % 10;

        if (carry > 0)
        {
            node *t_node = (node *)malloc(sizeof(node));
            t_node->val = 1;
            t_node->next = NULL;
            l1_tmp->next = t_node;
        }
    }
    else
    {
        sum = l1_tmp->val + l2_tmp->val + carry;
        carry = sum / 10;
        sum = sum % 10;
        l2_tmp->val = sum;

        if (carry > 0)
        {
            node *t_node = (node *)malloc(sizeof(node));
            t_node->val = 1;
            t_node->next = NULL;
            l2_tmp->next = t_node;
        }
    }

out:
    return ret_head;
}