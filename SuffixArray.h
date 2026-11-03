#ifndef SUFFIX_ARRAY_H
#define SUFFIX_ARRAY_H

// we use suffixTuple to compare the prefixes of the suffixes
// to avoid comparing character by character to save time
struct SuffixTuple {
    int original_index; // stores original index of a suffix
    int left_rank;      // stores first half of prefix
    int right_rank;     // used in conflicts when elements have equal left ranks
};

// We assign the DNA characters to integers. $ represents end of string.
int getBaseRank(char character);

// Merges two sorted subarrays of SuffixTuples into a single sorted array.
void merge(SuffixTuple* leftArray, int leftSize, SuffixTuple* rightArray,
           int rightSize, SuffixTuple* array);

// Recursively divides the array, sorts the halves, and merges them.
void mergeSort(SuffixTuple* array, int length);

#endif // SUFFIX_ARRAY_H
