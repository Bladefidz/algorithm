import java.lang.Iterable;
import java.lang.IllegalArgumentException;
import java.lang.UnsupportedOperationException;
import java.util.Iterator;
import java.util.NoSuchElementException;
import edu.princeton.cs.algs4.StdRandom;

/**
 * Randomized queue.
 * A randomized queue is similar to a stack or queue,
 * except that the item removed is chosen uniformly
 * at random from items in the data structure
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
    private Item first;
    private int count;

    /**
     * Construct an empty randomized queue
     * @return void
     */
    public RandomizedQueue()
    {
        this.first = null;
        this.count = 0;
    }

    /**
     * Is the randomized queue empty?
     * @return boolean
     */
    public boolean isEmpty()
    {
        return this.first == null;
    }

    /**
     * Return the number of items on the randomized queue
     * @return Integer
     */
    public int size()
    {
        return this.size;
    }

    /**
     * Add the item
     * @param Item item Item to add
     */
    public void enqueue(Item item)
    {

    }

    /**
     * Remove and return a random item
     * @return Item
     */
    public Item dequeue()
    {

    }

    /**
     * Return a random item (but do not remove it)
     * @return Item
     */
    public Item sample()
    {

    }

    /**
     * Return an independent iterator over items in random order
     * @return Iterator
     */
    public Iterator<Item> iterator()
    {

    }

    /**
     * Unit testing (optional)
     * @param String[] args StdIn arguments
     */
    public static void main(String[] args)
    {

    }
}
