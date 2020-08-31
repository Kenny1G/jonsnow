''' Given the array candies and the integer extraCandies, where candies[i] represents the number of candies that the ith kid has.

For each kid check if there is a way to distribute extraCandies among the kids such that he or she can have the greatest number of candies among them. Notice that multiple kids can have the greatest number of candies.'''
import sys


def run(candies, extraCandies):
    ans = []
    max_candies = int(max(candies))
    for candy in candies:
        if int(candy) + int(extraCandies) >= max_candies:
            ans.append(True)
        else:
            ans.append(False)

    return ans



if __name__ == '__main__':
    print(run(sys.argv[2:], sys.argv[1]))
