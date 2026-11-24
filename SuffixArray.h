#ifndef SUFFIX_ARRAY_H
#define SUFFIX_ARRAY_H
#include <string>
// we use suffixTuple to compare the prefixes of the suffixes
// to avoid comparing character by character to save time
struct SuffixTuple {
    int original_index; 
    int left_rank;     
    int right_rank;    
};

// We assign the DNA characters to integers. $ represents end of string.
int getBaseRank(char character);

// Merges two sorted subarrays of SuffixTuples into a single sorted array.
void merge(SuffixTuple* leftArray, int leftSize, SuffixTuple* rightArray,
           int rightSize, SuffixTuple* array);

// Recursively divides the array, sorts the halves, and merges them.
void mergeSort(SuffixTuple* array, int length);

SuffixTuple* suffixArrayBuilder(std::string word);

// Counts how many times 'pattern' appears in 'word' using the built suffix array.
int countPattern(std::string word, SuffixTuple* tuples, int length, std::string pattern);

#endif 
        