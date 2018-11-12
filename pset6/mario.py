#!/usr/bin/env python3

import cs50
import sys

def main():
    # validate the height
    while (True):
        print("Enter the height of the pyramid: ")
        height = cs50.get_int()
        if (height > 23 or height < 0):
            print ("Invalid input try again\n")
        else:
            break
    
    space = height - 1
    hashs = 1
    i=0
    # An outer loop to manage the every row of the pyramid
    while(i < height):
        # display spaces on the left side
        j=0
        while(j < space):
            sys.stdout.write(" ")
            j+=1 
        # display hashes on the left side
        k=0
        while (k < hashs):
            sys.stdout.write("#")
            k+=1
        # create gap
        sys.stdout.write("  ")
        # display hashes on the right side
        k=0
        while (k < hashs):
            sys.stdout.write("#")
            k+=1
        print("\n");
        i+=1
        space-=1
        hashs+=1
if __name__ == "__main__":
    main()