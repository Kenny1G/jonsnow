import sys

def run(s): 
        slider = []
        bSize =  0
        for  char in s:
            print(char, slider)
            if char not in slider:
                slider.append(char)
                bSize = max(len(slider), bSize)
            else:
                slider.pop(0)
        return bSize

if __name__ == '__main__':
    print(run(sys.argv[1]))


