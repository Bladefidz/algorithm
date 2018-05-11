/**
 * Stack with max.
 * Create a data structure that efficiently supports the stack operations
 * (push and pop) and also a return-the-maximum operation.
 * Assume the elements are reals numbers so that you can compare them.
 *
 * Solution:
 * Implement binary search to find max.
 */
 import edu.princeton.cs.algs4.In;
 import edu.princeton.cs.algs4.StdOut;

 public class StackDbl<Double>
 {
     private Double[] storage;
     private SortedNodes sorted = null;
     private int count = 0;

     public StackDbl(int capacity)
     {
         this.storage = (Double[]) new Object[capacity];
     }

     private class SortedNodes
     {
         Double item;
         SortedNodes next;
     }

     public boolean isEmpty()
     {
         return this.count == 0;
     }

     private void resize(int capacity)
     {
         Double[] copy = (Double[]) new Object[capacity];
         for (int i = 0; i < this.count; i++)
         {
             copy[i] = this.storage[i];
         }
         this.storage = copy;
     }

     private void insertinSortedNode(Double item)
     {
         if (this.sorted == null)
         {
             this.sorted = new SortedNodes();
             this.sorted.item = item;
             this.sorted.next = null;
         }
         else
         {
             SortedNodes current = this.sorted;
             double i = item.doubleValue();
             double c;
             while(current != null)
             {
                 c = current.item.doubleValue();
                 if (c < i)
                 { // current item is less than new item
                     SortedNodes newNode = new SortedNodes();
                     newNode.item = item;
                     newNode.next = current;
                     current = newNode;
                     break;
                 }
                 else if (c == i)
                 { // current item equal to new item
                     break;
                 }
                 else
                 {
                    current = current.next;
                 }
             }
         }
     }

     private void removeNode(Double item)
     {
         SortedNodes head = this.sorted;
         while (head != null)
         {
             if (head.item == item)
             {
                 head = head.next;
                 break;
             }
         }
     }

     private void removeLastNode()
     {
         this.sorted = this.sorted.next;
     }

     /**
      * Return maximum value in array
      * @return Double
      */
     public Double max()
     {
         if (this.sorted != null)
         {
             return this.sorted.item;
         }
     }

     /**
      * Double array size if full
      * @param Double item Input
      */
     public void push(Double item)
     {
         if (this.count == this.storage.length)
         {
             resize(this.storage.length * 2);
         }
         this.storage[this.count++] = item;
         insertinSortedNode(item);
     }

     /**
      * Halve array size if one-quarter fuStringll
      * @return int
      */
     public Double pop()
     {
         Double item = this.storage[--this.count];
         this.storage[this.count] = null;
         if (item == max())
         {
             removeLastNode();
         }
         else
         {
             removeNode(item);
         }
         if (this.count > 0 && this.count == this.storage.length / 4)
         {
             resize(this.storage.length / 2);
         }
         return item;
     }

     /**
      * Test client
      * @param String[] args
      */
     public static void main(String[] args)
     {

     }
 }
