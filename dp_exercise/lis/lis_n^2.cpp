/**
 * Leetcode 300, N^2 implementation
 * Here we use 'ending at' to identify the optimal solution to each subproblem
 */
#include <vector>

class Solution {
public:
    int lengthOfLIS(std::vector<int>& nums) {
        std::vector<int> length(nums.size());
        
        int ret = 0;
        for(int i = 0; i < nums.size(); i++){
            int maxLength = 0; // max length of preceding subsequence
            for (int j = 0; j < i; j++)
                if (nums[j] < nums[i] && length[j] > maxLength)
                    maxLength = length[j];
            length[i] = maxLength + 1; // count this element in
            if (length[i] > ret)
                ret = length[i];
        }

        return ret;
    }
};