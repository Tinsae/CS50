#include <stdio.h>
#include <cs50.h>
/* 
 A program to express our usage of water in terms of bottles of water
 The more you stay the more you waste.
*/
int main(void)
{
    printf("How many minutes do you need to shower ? \n");
    int mins=get_int();
    printf("damn, you are wasting %i bottles of water \n",(192 * mins) / 16);
}   