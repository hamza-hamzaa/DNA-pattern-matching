#include "SuffixArray.h"

#include <iostream>
#include <string>

using namespace std;

// We assign the DNA characters to integers. $ represents end of string
int getBaseRank(char character) { 
    if (character == '$') return 0;
    if (character == 'A') return 1;
    if (character == 'C') return 2;
    if (character == 'G') return 3;
    if (character == 'T') return 4;
    return -1; 
}


// Merges two sorted subarrays of SuffixTuples into a single sorted array.
void merge(SuffixTuple* leftArray, int leftSize, SuffixTuple* rightArray, int rightSize, SuffixTuple* array) {
    int i = 0, l = 0, r = 0; 
    
    while (l < leftSize && r < rightSize) {
       // we compare left ranks of tuples
       // if lefts are equal then we compare right ranks
        if ((leftArray[l].left_rank < rightArray[r].left_rank) or 
           ((leftArray[l].left_rank == rightArray[r].left_rank && leftArray[l].right_rank <= rightArray[r].right_rank))) 
           {
            array[i] = leftArray[l];
            l++;
        } else {
            array[i] = rightArray[r];
            r++;
        }
        i++;
    }
    
   // puts remaining subarrays in array after one side is fully merged
    while (l < leftSize) {
        array[i] = leftArray[l];
        i++;
        l++;
    }
    

    while (r < rightSize) {
        array[i] = rightArray[r];
        i++;
        r++;
    }
}

// Recursively divides the array, sorts the halves, and merges them.
void mergeSort(SuffixTuple* array, int length) 
{
    // base case
    if (length <= 1) return;
    
    int middle = length / 2;
    
   // creates subarrays 
    SuffixTuple* leftArray = new SuffixTuple[middle];
    SuffixTuple* rightArray = new SuffixTuple[length - middle];
    
    // Populate the left and right subarrays with array elements
    for (int i = 0; i < length; i++) {
        if (i < middle) {
            leftArray[i] = array[i];
        } else {
            rightArray[i - middle] = array[i]; 
        }
    }
    
    // Recursively sort both halves
    mergeSort(leftArray, middle);
    mergeSort(rightArray, length - middle);
    
    // Merge the sorted halves back together
    merge(leftArray, middle, rightArray, length - middle, array);
    
   // prevents memory leak
    delete[] leftArray;
    delete[] rightArray;
}


int main() {
    string word = "ABA$";
    int length = word.length();
    
   
    SuffixTuple* tuples = new SuffixTuple[length];
    
   
    // For length 1, we only look at the very first letter of each suffix 
    //to get the left rank.
    for (int i = 0; i < length; i++) {
        tuples[i].original_index = i;
        tuples[i].left_rank = getBaseRank(word[i]); // Dictionary score of current letter
        
        // letter exactly 1 step ahead of left rank used to calculate right rank.
        // If we are at the end of the string so we assign -1.
        if (i + 1 < length) {
            tuples[i].right_rank = getBaseRank(word[i + 1]);
        } else {
            tuples[i].right_rank = -1;
        }
    }
    

    mergeSort(tuples, length);
    
    // array used to hold newly calculated ranks during the doubling part
    int* new_ranks = new int[length];
    

    // To improve efficency instead of reading letters, we multiply our 
    // prefix length L by 2 every round.  A length 2L prefix means
    // two length L prefixes glued together.
    for (int L = 2; L < length; L *= 2) {
        

        // We evaluate our newly sorted tuples. If a tuple is identical to the one 
        // before it, they share a rank. If it's different, we increment the rank.
        int current_rank = 0;
        new_ranks[tuples[0].original_index] = current_rank; // The top of the sorted list gets rank 0
        
        for (int i = 1; i < length; i++) {
            if (tuples[i].left_rank != tuples[i - 1].left_rank || 
                tuples[i].right_rank != tuples[i - 1].right_rank)
             {
                // Unique prefix found, increase the rank score
                current_rank++; 
            }
            // Save new rank at the suffix's original starting index
            new_ranks[tuples[i].original_index] = current_rank;
        }
        
        // We build prefixes of length 2L by combining two halves of length L.
        for (int i = 0; i < length; i++)
         {
            tuples[i].original_index = i;
            
            // First half: The new rank of the chunk starting exactly at index i
            tuples[i].left_rank = new_ranks[i];
            
            // Second half: The new rank of the chunk located exactly L steps ahead!
            if (i + L < length) {
                tuples[i].right_rank = new_ranks[i + L];
            } else
             {
                // out of bound
                tuples[i].right_rank = -1; 
            }
        }
        
     
        // Sort the tuples again based on their updated L-step leapfrog ranks.
        mergeSort(tuples, length);
    }
    

    // every suffix has a completely unique rank and is perfectly sorted.
    cout << "Sorted Tuples (Original Index | Left Rank | Right Rank):" << endl;
    for (int i = 0; i < length; i++) {
        cout << "Index: " << tuples[i].original_index 
             << " | Left: " << tuples[i].left_rank 
             << " | Right: " << tuples[i].right_rank << endl;
    }
  

    delete[] new_ranks;
    delete[] tuples;
    
    return 0;
}