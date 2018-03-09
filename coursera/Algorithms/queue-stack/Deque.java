import java.lang.Iterable;
import java.lang.IllegalArgumentException;
import java.lang.UnsupportedOperationException;
import java.util.Iterator;
import java.util.NoSuchElementException;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

/**
 * Dequeue
 * A double-ended queue or deque (pronounced “deck”)
 * is a generalization of a stack and a queue that supports adding
 * and removing items from either the front or the back of the data
 * structure
 *
 * Requirements
 * Non-iterator operations  : Constant worst-case time
 * Iterator constructor	    : Constant worst-case time
 * Other iterator operations: Constant worst-case time
 * Non-iterator memory use  : 48n + 192 bytes
 * Memory per iterator      : Constant
 */
public class Deque<Item> implements Iterable<Item> {
    private Node first;
    private Node last;
    private int count;

    /**
     * Helper Linked list class
     */
    private class Node
    {
        private Node prev;
        private Item item;
        private Node next;
    }

    /**
     * Construct an empty deque
     */
    public Deque()
    {
        this.first = null;
        this.last = null;
        this.count = 0;
    }

    /**
     * Is the deque empty?
     * @return boolean
     */
    public boolean isEmpty()
    {
        return this.last == null;
    }

    /**
     * Return the number of items on the deque
     * @return integer
     */
    public int size()
    {
        return this.count;
    }

    /**
     * Add the item to the front
     * @param Item item [description]
     */
    public void addFirst(Item item)
    {
        if (item != null)
        {
            Node old = this.first;
            this.first = new Node();
            this.first.prev = null;
            this.first.item = item;
            this.first.next = old;
            if (old == null)
            {
                this.last = this.first;
            }
            else
            {
                old.prev = this.first;
            }
            this.count++;
        }
        else
        {
            throw new IllegalArgumentException();
        }
    }

    /**
     * Add the item to the end
     * @param Item item To add
     */
    public void addLast(Item item)
    {
        if (item != null)
        {
            Node old = this.last;
            this.last = new Node();
            this.last.item = item;
            this.last.next = null;
            this.last.prev = old;
            if (old == null)
            {
                this.first = this.last;
            }
            else
            {
                old.next = this.last;
            }
            this.count++;
        }
        else
        {
            throw new IllegalArgumentException();
        }
    }

    /**
     * Remove and return the item from the front
     * @return Item
     */
    public Item removeFirst()
    {
        if (!isEmpty())
        {
            Item it = this.first.item;
            if (this.first.next != null)
            {
                this.first = this.first.next;
                this.first.prev = null;
            }
            else
            {
                this.last = null;
                this.first = null;
            }
            this.count--;
            return it;
        }
        else
        {
            throw new NoSuchElementException();
        }
    }

    /**
     * Remove and return the item from the end
     * @return Item
     */
    public Item removeLast()
    {
        if (!isEmpty())
        {
            Item it = this.last.item;
            if (this.last.prev != null)
            {
                this.last = this.last.prev;
                this.last.next = null;
            }
            else
            {
                this.last = null;
                this.first = null;
            }
            this.count--;
            return it;
        }
        else
        {
            throw new NoSuchElementException();
        }
    }

    /**
     * Return an iterator over items in order from front to end
     * @return Iterator
     */
    public Iterator<Item> iterator()
    {
        return new ListIterator();
    }

    /**
     * List Iterator class
     */
    private class ListIterator implements Iterator<Item>
    {
        private Node current = first;

        public boolean hasNext()
        {
            return current != null;
        }

        public void remove()
        {
            throw new UnsupportedOperationException();
        }

        public Item next()
        {
            if (hasNext())
            {
                Item item = current.item;
                current = current.next;
                return item;
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
            Deque<Integer> dq = new Deque<>();

            // Testing queue operations
            StdOut.print("addFirst() then removeLast(): ");
            for (int i = 1; i <= num; i++)
            {
                Integer item = i;
                dq.addFirst(i);
            }
            for (int i = 1; i <= num; i++)
            {
                StdOut.print(dq.removeLast() + " ");
            }
            StdOut.print('\n');
            StdOut.println("empty? " + dq.isEmpty());
            StdOut.println("size? " + dq.size());
            StdOut.print("addLast() then removeFirst(): ");
            for (int i = 1; i <= num; i++)
            {
                Integer item = i;
                dq.addLast(i);
            }
            for (int i = 1; i <= num; i++)
            {
                StdOut.print(dq.removeFirst() + " ");
            }
            StdOut.print('\n');
            StdOut.println("empty? " + dq.isEmpty());
            StdOut.println("size? " + dq.size());
            StdOut.print("addFirst() then removeFirst(): ");
            for (int i = 1; i <= num; i++)
            {
                Integer item = i;
                dq.addFirst(i);
            }
            for (int i = 1; i <= num; i++)
            {
                StdOut.print(dq.removeFirst() + " ");
            }
            StdOut.print('\n');
            StdOut.println("empty? " + dq.isEmpty());
            StdOut.println("size? " + dq.size());
            StdOut.print("addLast() then removeLast(): ");
            for (int i = 1; i <= num; i++)
            {
                Integer item = i;
                dq.addLast(i);
            }
            for (int i = 1; i <= num; i++)
            {
                StdOut.print(dq.removeLast() + " ");
            }
            StdOut.print('\n');
            StdOut.println("empty? " + dq.isEmpty());
            StdOut.println("size? " + dq.size());

            // Testing iterator
            StdOut.print("addFirst() then foreach: ");
            for (int i = 1; i <= num; i++)
            {
                Integer item = i;
                dq.addFirst(i);
            }
            for (Integer i : dq) {
                StdOut.print(i + " ");
            }
            StdOut.print('\n');
            StdOut.print("addLast() then foreach: ");
            for (int i = 1; i <= num; i++)
            {
                Integer item = i;
                dq.addLast(i);
            }
            for (Integer i : dq) {
                StdOut.print(i + " ");
            }
            StdOut.print('\n');
        }
    }
}
