''' Given the array nums consisting of 2n elements in the form [x1,x2,...,xn,y1,y2,...,yn].

Return the array in the form [x1,y1,x2,y2,...,xn,yn]'''
import sys


def run(nums):
    ans = []
    for i in range(len(nums)/2):
        ans.append(nums[i])
        ans.append(nums[i + len(nums)/2])

    return ans



if __name__ == '__main__':
    print(run(sys.argv[1:]))
