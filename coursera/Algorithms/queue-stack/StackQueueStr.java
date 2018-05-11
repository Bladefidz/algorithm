/**
 * Queue with two stacks.
 * Implement a queue with two stacks so that each queue operations
 * takes a constant amortized number of stack operations.
 *
 * Solution:
 * Create private class of Stack inside Queue.
 * Initialize two stack instance each for enqueue and dequeue operation.
 */
 import edu.princeton.cs.algs4.In;
 import edu.princeton.cs.algs4.StdOut;

public class StackQueueStr
{
    private StackStr enqStack;
    private StackStr deqStack;

    public StackQueueStr(int capacity)
    {
        this.enqStack = new StackStr(capacity);
        this.deqStack = new StackStr(capacity);
    }

    private class StackStr
    {
        private String[] storage;
        private int count = 0;

        public StackStr(int capacity)
        {
            this.storage = new String[capacity];
        }

        public boolean isEmpty()
        {
            return this.count == 0;
        }

        private void resize(int capacity)
        {
            String[] copy = new String[capacity];
            for (int i = 0; i < this.count; i++)
            {
                copy[i] = this.storage[i];
            }
            this.storage = copy;
        }

        /**
         * Double array size if full
         * @param String item Input
         */
        public void push(String item)
        {
            if (this.count == this.storage.length)
            {
                resize(this.storage.length * 2);
            }
            this.storage[this.count++] = item;
        }

        /**
         * Halve array size if one-quarter full
         * @return String
         */
        public String pop()
        {
            String item = this.storage[--this.count];
            this.storage[this.count] = null;
            if (this.count > 0 && this.count == this.storage.length / 4)
            {
                resize(this.storage.length / 2);
            }
            return item;
        }
    }

    public void enqueue(String item)
    {
        this.enqStack.push(item);
    }

    public String dequeue()
    {
        while(!this.enqStack.isEmpty())
        {
            this.deqStack.push(this.enqStack.pop());
        }
        return this.deqStack.pop();
    }

    /**
     * Test client
     * @param String[] args
     */
    public static void main(String[] args)
    {
        StackQueueStr sqs = new StackQueueStr(5);
        sqs.enqueue("i");
        sqs.enqueue("you");
        sqs.enqueue("we");
        sqs.enqueue("they");
        sqs.enqueue("it");
        for (int i = 0; i < 5; i++)
        {
            StdOut.println(sqs.dequeue());
        }
    }
}
