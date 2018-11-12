#!/usr/bin/env python3

import cs50
import sys

# A function which checks the validity of any credit card number 
# using the famous Luhn’s algorithm
def checksum(card):
    sel = 0
    nsel = 0
    card_len = len(card)
    # last digit
    q1 = card_len - 1
    # second-to-last digit
    q2 = card_len - 2
    while (q1 >= 0 or q2 >= 0):
        if (q1 >= 0):
            nsel = nsel + card[q1]
        if (q2 >= 0):
            sel = sel + sum([int(i) for i in str(card[q2]*2)])
        q1 -= 2
        q2 -= 2
        
    # If the whole sum is divisible by 10 then it is valid 
    if ((sel + nsel) % 10 == 0):
        return True
    else:
        return False

print("Welcome to Tinsae's Credit Card Validator")
sys.stdout.writelines("Enter a credit card number: ")
card = input()
# Test the checksum using a function
if (checksum([int(i) for i in card]) == True):
    card_len = int(len(str(card)))
    start1 = int(card)//(10 ** (card_len-1))
    start2 = int(card)//(10 ** (card_len-2))
    if (start2 == 34 or start2 == 37):
        print("AMEX")
    elif (start2 >= 51 and start2 <= 55):
        print("MASTERCARD")
    elif(start1 == 4):
        print("VISA")
    else:
        print("INVALID 2")
else:
    print("INVALID 1")