" Given a string s, find the longest palindromic substring in s. You may assume that the maximum length of s is 1000."
"it works but not fast enough for leetcode FeelsBadMan"

import sys
from collections import defaultdict
def isPalindrome(s):
    half = len(s)/2
    for i in range(half):
        if s[i] != s[len(s) - (i+1)]:
            return False
    return True
 
def run(s):
    """
    :type s: str
    :rtype: str
    """
    hmap = defaultdict(list)
    max_string = "" if len(s) < 0 else s[0]
    for current_index, char in enumerate(s):
        if char in hmap: 
            for index in hmap[char]:
                potential_max = s[index:current_index+1]
                if isPalindrome(potential_max) and len(max_string) < len(potential_max):
                    max_string = potential_max
        hmap[char].append(current_index)
    return max_string
    




if __name__ == '__main__':
    print(run(sys.argv[1]))
