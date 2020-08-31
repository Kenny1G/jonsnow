''' Given an array of integers nums.

A pair (i,j) is called good if nums[i] == nums[j] and i < j.

Return the number of good pairs.'''
import sys
from collections import defaultdict


def bruteforce(nums):
    ''' bruteforce solution with O(n^2) speed '''
    ans = 0
    for i in range(len(nums)):
        for j in range(len(nums)):
            if nums[i] == nums[j] and i < j:
                ans += 1

    return ans

def optimized(nums):
    hmap = defaultdict(int)
    ans = 0
    for num in nums:
        ans += hmap[num]
        hmap[num] += 1
    print(hmap)
    return ans




if __name__ == '__main__':
    nums = []
    for char in sys.argv[1:]:
        nums.append(int(char))

    print(bruteforce(nums))
    print(optimized(nums))

