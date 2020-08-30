''' Given an array nums. 
We define a running sum of an array as runningSum[i] = sum(nums[0]...nums[i]).  Return the running sum of nums '''
import sys


def run(nums):
    if len(nums) <= 1:
        return nums
    running_total = 0
    ans = []
    for num in nums:
        running_total += int(num)
        ans.append(running_total)
    return ans



if __name__ == '__main__':
    print(run(sys.argv[1:]))
