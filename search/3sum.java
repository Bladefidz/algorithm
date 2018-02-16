import java.util.Arrays;

class Solution {
    public List<List<Integer>> threeSum(int[] nums) {
        List<List<Integer>> result = new LinkedList<>();

        // Defensive technique to check zero
        if (nums.length > 2) {
            List<Integer> zeros = new LinkedList<>();
            for (int i = 0; i < 3; i++) {
                if (nums[i] != 0) break;
                zeros.add(0);
                if (i == nums.length-1) {
                    result.add(zeros);
                    return result;
                }
            }
        }

        // HashSet<Integer> set = new HashSet<Integer>();
        Arrays.sort(nums);

        // Show nums array
        for (int i = 0; i < nums.length; i++)
            System.out.print(nums[i] + " ");
        System.out.println();

        // Do binary search for each pairs i and j
        int lasti = Integer.MAX_VALUE;
        int lastj = Integer.MAX_VALUE;
        for (int i = 0; i < nums.length; i++) {
            if (nums[i] == lasti)
                continue;
            for (int j = i+1; j < nums.length; j++) {
                if (nums[j] == lastj)
                    continue;
                int complements = -(nums[i] + nums[j]);
                int lo = 0;
                int hi = nums.length - 1;
                int k = -1;
                while (lo <= hi) {
                    int mid = lo + (hi - lo) / 2;
                    if      (complements < nums[mid]) hi = mid - 1;
                    else if (complements > nums[mid]) lo = mid + 1;
                    else {
                        k = mid;
                        break;
                    }
                }
                if (k == i && k+1 < nums.length)
                    if (nums[k+1] == nums[i])
                        k++;
                if (k == j && k+1 < nums.length)
                    if (nums[k+1] == nums[j])
                        k++;
                if (k > j) {
                    List<Integer> newSum = new LinkedList<>();
                    newSum.add(nums[i]);
                    newSum.add(nums[j]);
                    newSum.add(nums[k]);
                    result.add(newSum);
                }
                lastj = nums[j];
            }
            lasti = nums[i];
        }

        return result;
    }
}