#include <stdio.h>
#include <cs50.h>
#include <math.h>

long long digits(long long card);
bool checksum(long long card);

/*
  A program to validate credit card numbers for 
  American Express, Master Card and Visa
  It does it's work using the famous Lune Algorithm.
  It identifies one company's credit card from the other using
  the beginning numbers.
*/
int main(void)
{
    printf("%s\n","Welcome to Tinsae's Credit Card Validator");
    printf("%s\n","Enter a credit card number");
    long long card=get_long_long();
    // Test the checksum using a function
    if (checksum(card) == true)
    {
        long long digits_count=digits(card);
        long long start1=card / pow(10,digits_count - 1);
        long long start2=card / pow(10,digits_count - 2) ;

        if (start2 == 34 || start2 == 37)
        {
            printf("AMEX\n");
        }
        else if(start2 >= 51 && start2 <= 55)
        {
            printf("MASTERCARD\n");
        }
        else if(start1 == 4)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID 2\n");
        }
    }
    else
    {
        printf("INVALID 1\n");
    }
}   

/* A function which calculates the number of digits of _
    any given credit card number
*/
long long digits(long long card)
{
    int digits_count = 0;
    while(card != 0)
    {
        card = card / 10;
        digits_count = digits_count + 1;
    }
    return digits_count;
}
/* A function which checks the validity of any credit card number 
    using the famous Luhn’s algorithm
*/
bool checksum(long long card)
{
    long long digits_count = 0, quotient = 0, first = 0, second = 0, flag = 0;
    digits_count = digits(card);
    /* When number of digits is even, we set flag to 1 so that we pick 
       every other number starting from the first digit
       otherwise, we pick every other number beginning from the second digit
    */
    if ( digits_count % 2 == 0)
    {
        flag = 1;
    }
    else
    {
        flag = 0;
    }
    /* In the following loop we maintain two sums
       the first is just the sum of the unselected numbers
       the second is the sum of digits of the products of the selected numbers
    */
    for(long long i = digits_count; i > 0; i--)
    {   
        long long div=pow(10,i - 1);
        quotient = card / div;
        if (flag == 0)
        {
            first = first + quotient;
            flag = 1;
        }
        else
        {
            long long t_quotient = quotient * 2;
            long long d_count2 = digits(t_quotient);
            for (long long p = d_count2; p > 0; p--)
            {
                long long div2 = pow(10,p - 1);
                long long new_quotient = t_quotient / div2;
                second = second + new_quotient;
                t_quotient = t_quotient - (new_quotient * div2);
            }
            flag = 0;
        }
        card = card - (quotient * div);
    }
    /* If the whole sum is divisible by 10 then it is valid */
    if ((first + second) % 10 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

