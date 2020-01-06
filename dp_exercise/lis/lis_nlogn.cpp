/**
 * Leetcode 300, NlogN implementation
 * 
 * Key: DP. Only the value of tail elements, and the length of those optimal subsequences are needed
 * Use the `length` as index is the keypoint.
 * 
 * If we need the sequence, I guess tails alone is not enough. We need to clone the subsequences
 * from time to time, and the complexity would be N^2
 */
#include <vector>
#include <algorithm> // binary search

class Solution {
public:
    int lengthOfLIS(std::vector<int>& nums) {
        if (nums.size() == 0)
            return 0;
        
        std::vector<int> tails;
        tails.reserve(nums.size());
        tails.push_back(nums[0]); // index = length - 1

        for (int i = 1; i < nums.size(); i++)
        {
            // the branch is not necessary, but easier for recap
            if (nums[i] < tails[0]) // smaller than all
                tails[0] = nums[i];
            else if (nums[i] > tails[tails.size() - 1]) // greater than all
                tails.push_back(nums[i]);
            else
            {
                std::vector<int>::iterator it = std::lower_bound(tails.begin(), tails.end(), nums[i]);
                *it = nums[i]; // works as expected even if *it == nums[i]
            }
        }
        return tails.size();
    }
};