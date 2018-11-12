/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    //Done using binary search
    int start = 0;
    int end = n - 1;
    
    // The start is usually smaller than the end; but at the last element
    // The start will be the same as the end
    // If it is not found at that point, the loop will try either to 
    // only decrease end or only increase start.
    // In either way start will be larger than end; that is a sign to stop
    while (start<=end) 
    {
        // Find the mid-range
        int middle = (start + end) / 2;
        
        // Go to the left
        if (value < values[middle]) 
        {
            end = middle - 1;
        }
        // Go to the right
        else if (value > values[middle]) 
        {
            start = middle + 1;
        }
        else
        {
           // printf("Number is found @ index %i",middle);
            return true;
        }
    }

    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
     // I am using counting sort
     // find the maximum first, and use it as the size of counter array 
     // This algorithm is bad for very very large numbers because
     // the storage space requirement will be high
    int max = 0;    
    for (int i = 0; i < n; i++)
    {
        if (max < values[i])
        {
            max = values[i];
        }
    }
    // initialize frequency array to all be all 0s
    // It is max + 1 because to make the last index equal to max
    // we have to make the size max + 1; last index = size-1
    int frequency[max + 1];
    for (int i = 0; i <= max; i++)
    {
        frequency[i] = 0;
    }
    
    // the index of frequency[] will be equivalent to a number
    // in values[] and the value stored at an index will contain
    // frequency of the number in values[]
    for (int i = 0; i < n; i++)
    {
        frequency[values[i]] += 1;
    }
    int v_count = 0;
    for(int f_count = 0; f_count <= max; f_count++) 
    {
        int curr = frequency[f_count];
        // overwrite i in values according to the number of times f_count
        // occured in frequency[]
        while(curr > 0) 
        {
            values[v_count] = f_count;
            v_count++;
            curr--;
        }
        
    }
}
