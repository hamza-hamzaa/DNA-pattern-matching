/*
We made this file to write the actual implementation of the DynamicCharArray functions.
The header file tells us what each function does, while this file explains how each function works.

We also have some rules that we need to keep true throughout the program:
(1) The length can never be greater than the capacity because we cannot store more characters than the space we allocated.
(2) If the capacity is greater than 0, our data pointer must point to a valid memory space.
(3) If the capacity is 0, our data pointer must be nullptr because we have not allocated any memory.
*/
#include "dynamicchararray.h"
#include <new>   // I am using this to include std::nothrow, which allows new to return nullptr if memory allocation fails instead of throwing an exception.

// The Private helper functions
bool DynamicCharArray::grow(size_t minCapacity) {
    if (minCapacity <= capacity_) return true; // I am checking if our current capacity is already enough. If it is, we don't need to allocate more memory.
    size_t newCapacity; // I am using this variable to keep track of the new capacity we want to allocate.
    if (capacity_ == 0) {newCapacity = DEFAULT_CAPACITY;} // If our current capacity is 0, I start with the default capacity of 16.
    else {newCapacity = capacity_;} // Otherwise, I start with our current capacity.
    while (newCapacity < minCapacity) {
        // I keep doubling the capacity until we have enough space for the characters we need.
        // This helps us avoid allocating new memory every time we add a character.
        // I also check for size_t overflow before doubling so the number does not become too large.
        if (newCapacity > ((size_t)-1) / 2) {newCapacity = minCapacity; break;}
        newCapacity *= 2; // I double the capacity to increase the available space.
    }
    char* newData = new (std::nothrow) char[newCapacity]; // I am creating a new memory space with the new capacity. I use std::nothrow so the pointer becomes nullptr if the allocation fails.
    if (newData == nullptr) return false; // If memory allocation fails, I return false without changing our original array.
    for (size_t i = 0; i < length_; ++i) newData[i] = data[i]; // I am copying all the characters from our old array into the new memory space so we don't lose any data.
    delete[] data; // I am freeing the old memory because we now have a new memory space containing our characters.
    data = newData; // I am making data point to our newly allocated memory.
    capacity_ = newCapacity; // I am updating the capacity so it matches the amount of memory we just allocated.
    return true; // I return true because we successfully increased the capacity of our array.
}

// The Constructors, Destructor, and Copying Functions
// I am using this constructor to create an empty array with the default capacity of 16 characters.
DynamicCharArray::DynamicCharArray() : data(nullptr), length_(0), capacity_(0) {grow(DEFAULT_CAPACITY);} // I am calling grow() to allocate memory for 16 characters. If allocation fails, the array stays empty.
// I am using this constructor to create an empty array with a capacity that we choose.
DynamicCharArray::DynamicCharArray(size_t initialCapacity) : data(nullptr), length_(0), capacity_(0) {
    if (initialCapacity > 0) { // I am checking if the requested capacity is greater than 0 before allocating memory.
        data = new (std::nothrow) char[initialCapacity]; // I am allocating enough memory to store the requested number of characters.
        if (data == nullptr) {capacity_ = 0;} // If memory allocation fails, I keep the capacity at 0.
        else {capacity_ = initialCapacity;} // If allocation succeeds, I update the capacity to the requested amount.
    }
}
// I am using this constructor to create an array from an existing C string, such as "ACGT".
DynamicCharArray::DynamicCharArray(const char* cstr) : data(nullptr), length_(0), capacity_(0) {append(cstr);} // I am using append() to copy the characters from the C string into our array.
// I am using this copy constructor to create a new array using another DynamicCharArray object.
DynamicCharArray::DynamicCharArray(const DynamicCharArray& other) : data(nullptr), length_(0), capacity_(0) {append(other);} // I am using append() to copy all the characters into our new array.
//This creates a deep copy.
// I am using the destructor to free the memory allocated for our array.
DynamicCharArray::~DynamicCharArray() {
    delete[] data; // I am freeing the memory because we used new[] to allocate it.
    data = nullptr; // I am setting the pointer to nullptr because it no longer points to allocated memory.
}

// I am using this operator to copy the contents of one existing array into another.
DynamicCharArray& DynamicCharArray::operator=(const DynamicCharArray& other) {// I am checking if we are copying the array into itself, such as arr = arr.
    // If both objects are the same, there is no need to copy anything.
    if (this == &other) {return *this;}
    char* newData = nullptr; // I am creating a pointer for the new memory. It starts as nullptr because we haven't allocated memory yet.
    if (other.length_ > 0) { // I am checking if the other array actually contains characters.
        newData = new (std::nothrow) char[other.length_]; // I am allocating enough memory to store all the characters from the other array.
        if (newData == nullptr) {return *this;} // If memory allocation fails, I return the current object without changing its original data.
        for (size_t i = 0; i < other.length_; ++i) {newData[i] = other.data[i];} // I am copying each character from the other array into our new memory.
    }
    delete[] data; // I am freeing the old memory after successfully copying the new characters.
    data = newData; // I am making data point to the new memory.
    length_ = other.length_; // I am updating the length to match the number of characters in the other array.
    if (newData == nullptr) {capacity_ = 0;} // I am updating the capacity. If no memory was allocated, it stays at 0.
    else {capacity_ = other.length_;}
    return *this; // I am returning the current object after the assignment is complete.
}

// Modification Functions
// I am using this function to make sure our array has enough memory for a certain number of characters.
bool DynamicCharArray::reserve(size_t newCapacity) {return grow(newCapacity);} // I am calling grow() to increase the capacity if we need more space.
//If the current capacity is already enough, nothing changes.
// I am using this function to add one character to the end of our array.
bool DynamicCharArray::pushBack(char c) {
    if (grow(length_ + 1) == false) {return false;} // First make sure we have enough space for one more character.
    data[length_] = c; // the add the character at the end of the array.
    length_++; // After that increase the length because we added one character.
    return true; // I return true because we successfully added the character.
}
bool DynamicCharArray::append(const char* cstr) {
    if (cstr == nullptr) {return true;} // I am checking if the string exists. If it is nullptr, there is nothing to add, so I return true.
    size_t n = 0; // I am using this variable to count how many characters are in the C string.
    while (cstr[n] != '\0') {n++;} // I keep counting until I reach '\0', which marks the end of the C string.
    if (n == 0) {return true;} // If the string is empty, there is nothing to add, so I return true.
    if (grow(length_ + n) == false) {return false;} // I am making sure our array has enough space for its current characters and all the new characters.
    //If we cannot allocate enough memory, I return false.
    for (size_t i = 0; i < n; i++) {data[length_ + i] = cstr[i];} // I am copying each character from the C string into the free spaces at the end of our array.
    length_ = length_ + n; // I am updating the length by adding the number of characters we just copied.
    return true; // I return true because we successfully added the characters to our array.
}

bool DynamicCharArray::append(const DynamicCharArray& other) {
    size_t n = other.length_; // I am storing the number of characters in the other array so we know how many characters we need to copy.
    //We do this before calling grow() so appending an array to itself, like arr.append(arr), works correctly.
    if (n == 0) {return true;} // I am checking if the other array is empty. If it has no characters, there is nothing to add, so I return true.
    if (grow(length_ + n) == false) {return false;} // I am making sure our array has enough space for its current characters and the new characters. If memory allocation fails, I return false.
    for (size_t i = 0; i < n; i++) {data[length_ + i] = other.data[i];} // I am copying each character from the other array into the free spaces at the end of our array.
    length_ = length_ + n; // I am updating the length by adding the number of characters we just copied.
    return true; // I return true because we successfully added the characters to our array.
}

bool DynamicCharArray::popBack(char& removed) {
    if (length_ == 0) {return false;} // I am checking if our array is empty. If there are no characters to remove, I return false.
    removed = data[length_ - 1]; // I am getting the last character in our array and storing it in removed.
    //I use length_ - 1 because array indexes start at 0.
    length_--; // I am decreasing the length by 1 because we removed the last character. The allocated memory is still there, but the array now contains one less character.
    return true; // I return true because we successfully removed the last character.
}

bool DynamicCharArray::set(size_t index, char c) {
    if (index >= length_) {return false;} // I am checking if the index is outside our array's current length.
    //If it is, we cannot change that position, so I return false.
    data[index] = c; // I am replacing the character at the given index with the new character c.
    return true; // I return true because we successfully updated the character.
}

void DynamicCharArray::clear() {length_ = 0;} // I am setting the length to 0 because we are removing all the characters from our array.
//I am keeping the allocated memory so we can reuse it later without allocating memory again.
// Access / information
bool DynamicCharArray::get(size_t index, char& out) const {
    if (index >= length_) {return false;} // I am checking if the requested index is outside our array's current length.
    //If it is, there is no character to get, so I return false.
    out = data[index]; // I am getting the character at the requested index and storing it in out. This allows us to access the character without changing the original array.
    return true; // I return true because we successfully retrieved the character.
}

char& DynamicCharArray::operator[](size_t index) {return data[index];} // I am returning the character at the requested index.
//Since I am returning char&, we can also use this function to change the character.
const char& DynamicCharArray::operator[](size_t index) const {return data[index];} // I am returning the character at the requested index, but as const char&, meaning we can read it but cannot change it.
const char* DynamicCharArray::rawData() const {return data;} // I am returning a pointer to the actual character array so other functions can access its data without copying it.
//The returned data is not necessarily null-terminated.
size_t DynamicCharArray::size() const {return length_;} // I am returning the number of characters currently stored in our array.
size_t DynamicCharArray::capacity() const {return capacity_;} // I am returning the total number of characters our allocated memory can hold.
bool DynamicCharArray::isEmpty() const {return length_ == 0;} // I am checking if our array has zero characters. If it does, I return true; otherwise, I return false.


// Helper functions for the algorithms
bool DynamicCharArray::equals(const DynamicCharArray& other) const {
    if (length_ != other.length_) {return false;} // I am checking if both arrays have the same number of characters. If their lengths are different, they cannot be equal.
    for (size_t i = 0; i < length_; ++i) { // I am going through each character in both arrays to compare them.
        if (data[i] != other.data[i]) {return false;} // I am checking if the characters at the same index are different. If I find a difference, the arrays are not equal.
    }
    return true; // I return true because all the characters matched, meaning both arrays are equal.
}
bool DynamicCharArray::matchesAt(size_t pos, const DynamicCharArray& pattern) const {
    if (pos > length_) {return false;} // I am checking if the starting position is outside our array. If it is, the pattern cannot match there.
    if (pattern.length_ > length_ - pos) {return false;} // I am checking if the pattern is longer than the remaining characters from this position. If it is, the pattern would go past the end of our array.
    for (size_t j = 0; j < pattern.length_; ++j) { // I am going through each character in the pattern to compare it with our array.
        if (data[pos + j] != pattern.data[j]) {return false;} // I am comparing the current character in our array with the corresponding character in the pattern. If they are different, the pattern does not match at this position.
    }
    return true; // I return true because all the characters matched. An empty pattern also matches at any valid position.
}
DynamicCharArray DynamicCharArray::substr(size_t start, size_t count) const {
    DynamicCharArray result(count); // I am creating a new array that will store the substring. I use count as its initial capacity because that is the maximum number of characters we might copy.
    if (start >= length_) {return result;} // I am checking if the starting position is outside our array or exactly at the end. If it is, there are no characters to copy, so I return an empty array.
    if (count > length_ - start) {count = length_ - start;} // I am checking if the requested number of characters goes past the end of our array. If it does, I reduce count so we only copy the characters that are available.
    for (size_t i = 0; i < count; ++i) {result.pushBack(data[start + i]);} // I am going through the characters that we want to include in our substring.
 // I am copying each character from our original array into the new result array.
    return result; // I am returning the new array containing the substring.
}