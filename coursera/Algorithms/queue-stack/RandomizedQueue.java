import java.lang.Iterable;
import java.lang.IllegalArgumentException;
import java.lang.UnsupportedOperationException;
import java.util.Iterator;
import java.util.NoSuchElementException;
import edu.princeton.cs.algs4.StdRandom;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

/**
 * Randomized queue.
 * A randomized queue is similar to a stack or queue,
 * except that the item removed is chosen uniformly at random
 * from items in the data structure
 *
 * Requirements
 * Non-iterator operations  : Constant amortized time
 * Iterator constructor	    : Linear in current # of items
 * Other iterator operations: Constant amortized time
 * Non-iterator memory use  : 48n + 192 bytes
 * Memory per iterator      : 48n + 192 bytes
 */
public class RandomizedQueue<Item> implements Iterable<Item>
{
    private Item[] storage;
    private int count;

    /**
     * Construct an empty randomized queue
     * @return void
     */
    public RandomizedQueue()
    {
        this.storage = (Item[]) new Object[2];
        this.count = 0;
    }

    /**
     * Is the randomized queue empty?
     * @return boolean
     */
    public boolean isEmpty()
    {
        return this.count == 0;
    }

    /**
     * Return the number of items on the randomized queue
     * @return Integer
     */
    public int size()
    {
        return this.count;
    }

    private void resize(int capacity)
    {
        Item[] copy = (Item[]) new Object[capacity];
        for (int i = 0; i < this.count; i++)
        {
            copy[i] = this.storage[i];
        }
        this.storage = copy;
    }

    /**
     * Add the item
     * @param Item item Item to add
     */
    public void enqueue(Item item)
    {
        if (item != null)
        {
            if (this.count == this.storage.length)
            {
                resize(2 * this.storage.length);
            }
            this.storage[this.count++] = item;
        }
        else
        {
            throw new IllegalArgumentException();
        }
    }

    /**
     * Remove and return a random item
     * @return Item
     */
    public Item dequeue()
    {
        if (!isEmpty())
        {
            int r = StdRandom.uniform(this.count);
            Item item = this.storage[r];
            this.storage[r] = this.storage[this.count - 1];
            this.storage[this.count - 1] = null;
            this.count--;
            if (this.count > 0 && this.count == this.storage.length / 4)
            {
                resize(this.storage.length / 2);
            }
            return item;
        }
        else
        {
            throw new NoSuchElementException();
        }
    }

    /**
     * Return a random item (but do not remove it)
     * @return Item
     */
    public Item sample()
    {
        if (!isEmpty())
        {
            return this.storage[StdRandom.uniform(0, this.count)];
        }
        else
        {
            throw new NoSuchElementException();
        }
    }

    /**
     * Return an independent iterator over items in random order
     * @return Iterator
     */
    public Iterator<Item> iterator()
    {
        return new ShuffledArrayIterator(this.storage, this.count);
    }

    /**
     * Random Array Iterator class
     */
    private class ShuffledArrayIterator implements Iterator<Item>
    {
        private Item[] suflStorage;
        private int suflCount;
        private int out;

        public ShuffledArrayIterator(Item[] rq, int size)
        {
            this.suflStorage = (Item[]) new Object[size];
            this.suflCount = size;
            for (int i = 0; i < size; i++)
            {
                this.suflStorage[i] = rq[i];
            }
            StdRandom.shuffle(this.suflStorage, 0, count);
            this.out = 0;
        }

        public boolean hasNext()
        {
            return this.out < this.suflCount;
        }

        public void remove()
        {
            throw new UnsupportedOperationException();
        }

        public Item next()
        {
            if (hasNext())
            {
                Item it = this.suflStorage[this.out];
                this.out++;
                return it;
            }
            else
            {
                throw new NoSuchElementException();
            }
        }
    }

    /**
     * Unit testing (optional)
     * @param String[] args StdIn arguments
     */
    public static void main(String[] args)
    {
        int num = Integer.parseInt(args[0]);
        if (num > 0)
        {
            RandomizedQueue<Integer> rq = new RandomizedQueue<>();

            // Testing enqueue and dequeue
            StdOut.println("enqueue() then dequeue()");
            for (int i = 1; i <= num; i++)
            {
                Integer item = i;
                rq.enqueue(i);
            }
            while(!rq.isEmpty())
            {
                StdOut.print(rq.dequeue() + " ");
            }
            StdOut.print('\n');

            // Testing iterator
            StdOut.println("enqueue() then random iterator");
            for (int i = 1; i <= num; i++)
            {
                Integer item = i;
                rq.enqueue(i);
            }
            for (Integer i : rq)
            {
                StdOut.print(i + " ");
            }
            StdOut.print('\n');

            // Testing nested iterator
            StdOut.println("nested random iterator");
            Iterator<Integer> it1 = rq.iterator();
            Iterator<Integer> it2 = rq.iterator();
            while(it1.hasNext())
            {
                Integer i = it1.next();
                StdOut.print(i + " ");
            }
            StdOut.print('\n');
            while(it2.hasNext())
            {
                Integer i = it2.next();
                StdOut.print(i + " ");
            }
            StdOut.print('\n');
        }
    }
}
