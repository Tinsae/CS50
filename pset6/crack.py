#!/usr/bin/env python3


import cs50
from sys import argv
import crypt
from string import ascii_uppercase as uc
from string import ascii_lowercase as lc

# arguments should be 2

if (len(argv) == 2):
    # extract the salt from the 1st argument
    hashed = str(argv[1])
    #print (crypt.crypt("fZer", "50"))
    #length 1 passwords
    for i in uc + lc:
        if (crypt.crypt(i, hashed[0:2]) == argv[1]):
            print(i)
            break
        for j in uc + lc:
            if (crypt.crypt(i + j, hashed[0:2]) == argv[1]):
                print(i + j)
                exit(0)
            for k in uc + lc:
                if (crypt.crypt(i + j + k, hashed[0:2]) == argv[1]):
                    print(i + j + k)
                    exit(0)
                for m in uc + lc:
                    if (crypt.crypt(i + j + k + m, hashed[0:2]) == argv[1]):
                        print(i + j + k + m)
                        exit(0)

else:
    print("Not found")
    exit(1)