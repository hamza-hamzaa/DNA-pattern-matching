/*
We made this array to store the DNA text and the patterns we want to search for in our project.
Since we can't use things like vector or string we have to make our own array that can grow when we need more space.
We manage memory ourselves using new[] and delete[].
This is a linear data structure because the characters are stored one after another.
Some functions return false if something goes wrong like trying to access an invalid index or failing to allocate memory.
This lets us handle errors without throwing exceptions.
Each object has its own memory so copying one array into another creates a separate copy instead of making both objects point to the same memory.
For the header file I will be explaining why we use each function.
 */

#ifndef DYNAMICCHARARRAY_H
#define DYNAMICCHARARRAY_H

#include <cstddef>   // I am using this so I can be able to use size_t. This will help me track how many characters we store and how much memory we have in the array.

class DynamicCharArray {
private:
    char*  data;       // I am using this pointer to store the characters in the memory we allocate for the array.
    // This allows us to manage the memory ourselves and add more space when we need it. This is our heap buffer.
    size_t length_;    // I am using this to keep track of how many characters we currently have stored in the array. This helps us know how much of the array we are actually using.
    size_t capacity_;  // I am using this to keep track of how many characters the array can hold with its current memory. This helps us know when we need to allocate more memory to fit more characters.

    static const size_t DEFAULT_CAPACITY = 16; // I am using this to set the default amount of space our array starts with which is 16 characters.
    // This means that when we create an empty array without specifying its capacity it will have enough memory to store 16 characters before needing to grow.
    // The static means this value belongs to the class itself and const means we cannot change its value.
    // We picked 16 as a starting capacity so our array has space for 16 characters before it needs to grow. It is a good starting number before it grows.


    bool grow(size_t minCapacity); // I am using this function to increase the array's capacity when we run out of space.
    // It doubles the current capacity until we have enough room for the characters we need.
    // If we cannot allocate more memory it returns false and keeps the array unchanged.

public:
    // Constructor, destructor, and copying
    DynamicCharArray(); // I am using this constructor to create an empty array with the default capacity of 16 characters.
    // The array starts with length 0 because we haven't stored anything yet.
    // We call grow() to allocate space for 16 characters. If memory allocation fails the array stays empty with capacity 0.

    DynamicCharArray(size_t initialCapacity); // I am using this constructor when I want to create an empty array with a specific starting capacity.
    // Instead of always starting with 16 characters we can decide how much memory we want to allocate when creating the array.
    // If the requested capacity is 0 no memory is allocated. If allocation fails the array stays empty with capacity 0.

    DynamicCharArray(const char* text); // I am using this constructor to create an array from an existing C string such as "ACGT".
    // It takes the characters from the given string and copies them into our own allocated memory using append().
    // This allows us to store DNA sequences or search patterns directly when creating the object.

    DynamicCharArray(const DynamicCharArray& other); // I am using this copy constructor when I want to create a new array using another DynamicCharArray object.
    // We make a deep copy which means we allocate new memory and copy all the characters from the original array.
    // It uses append() to copy the characters into the new array's own memory.
    // This is important because we don't want both objects pointing to the same memory.
    // That way changing or deleting one array will not affect the other.

    ~DynamicCharArray(); // I am using the destructor to free the memory that was allocated for the array.
    // Since we use new[] to allocate memory we must use delete[] when the object is destroyed.
    // This prevents memory leaks and makes sure the allocated memory is properly released.

    DynamicCharArray& operator=(const DynamicCharArray& other); // I am using this operator when I want to assign one existing DynamicCharArray object to another.
    // It also performs a deep copy by allocating separate memory and copying the characters.
    // It allocates memory based on the number of characters in the other array not its original capacity.
    // It also checks if we are assigning an array to itself so we don't cause memory problems.
    // If memory allocation fails the original array stays unchanged.
    // It returns a reference to the current object so we can chain assignments if needed.

    // The main difference between these functions is that each one is used at a different stage of the object's lifetime.
    // The first constructor creates an empty array with the default capacity of 16 while the second allows us to choose how much memory we want the array to start with.
    // The third constructor creates an array and copies characters from an existing C string such as "ACGT".
    // The copy constructor is used when we want to create a new array using another existing array so it makes a deep copy and gives the new object its own separate memory.
    // The destructor does the opposite of the constructors because it frees the memory when the object is destroyed.
    // Finally the copy assignment operator is used when we already have two existing arrays and want to copy the contents of one into the other.
    // Both the copy constructor and copy assignment perform a deep copy but the main difference is that the copy constructor creates a new object while the assignment operator updates an object that already exists.


    // Array Modifications Functions
    bool pushBack(char c); // I am using this function to add one character to the end of our array.
    // If there is no more available space it calls grow() to increase the capacity so we have enough room for the new character.
    // It returns false if something goes wrong such as failing to allocate more memory.

    bool append(const char* text); // I am using this function to add characters from an existing C string to the end of our array.
    // It copies the characters one by one after the characters we already have stored.
    // This allows us to add DNA sequences or search patterns without creating a new array.
    // If the C string is nullptr or empty nothing is added and the function returns true.

    bool append(const DynamicCharArray& other); // I am using this function to add the contents of another DynamicCharArray object to the end of our current array.
    // It copies the characters from the other array without changing or affecting the original one.
    // We use a reference to avoid making an unnecessary copy of the entire object.
    // It also allows us to append an array to itself such as arr.append(arr) without losing the original characters.

    bool popBack(char& removed); // I am using this function to remove the last character from our array.
    // Before removing it, we store the removed character in the variable 'removed' so we can still access it.
    // It returns false if the array is empty because there is nothing to remove.

    bool set(size_t index, char c); // I am using this function to replace a character at a specific position in our array.
    // The index tells us which character we want to change and the new character replaces the old one.
    // It returns false if the index is outside the current length of the array.

    void clear(); // I am using this function to remove all the characters currently stored in our array by setting the length to 0.
    // We don't delete the allocated memory or reset the capacity because we might want to reuse the same array later.
    // This saves us from allocating memory again when we start adding new characters.

    bool reserve(size_t newCapacity); // I am using this function to make sure our array has enough memory for a certain number of characters before we start adding them.
    // If the requested capacity is larger than our current capacity it calls grow() to allocate more memory.
    // The grow() function doubles the capacity when needed so we don't have to allocate memory every time we add characters.
    // If the requested capacity is smaller than or equal to the current capacity nothing changes.
    // It returns false if memory allocation fails.

    // Access Functions
    bool get(size_t index, char& out) const; // I am using this function to get the character stored at a specific position in our array.
    // The index tells us which character we want to access and the function copies that character into the "out" variable.
    // It returns false if the index is invalid so we don't accidentally access a position outside the array.
    // The const at the end means this function will not change anything inside our array.

    char& operator[](size_t index); // I am using this operator to access or modify a character directly using its index just like a normal array.
    // Unlike get() this operator does not check if the index is valid which makes it faster.
    // We mainly use it in our searching algorithms when we already know that the index is within the array's limits.
    // It returns a reference to the actual character so we can also change its value.

    const char& operator[](size_t index) const; // I am using this version when we want to access a character from an array that should not be modified.
    // It works like the previous operator but returns a const reference meaning we can read the character but cannot change it.
    // The const at the end also makes sure that the function does not modify the array.

    // Information
    size_t size() const; // I am using this function to get the number of characters currently stored in our array.
    // It returns the length not the capacity because we only want to know how many characters we are actually using.

    size_t capacity() const; // I am using this function to get the total amount of memory our array currently has available.
    // This helps us know how many characters we can store before needing to allocate more memory.

    bool isEmpty() const; // I am using this function to check if our array is empty or not.
    // It checks if the length is 0 and returns true if there are no characters stored otherwise it returns false.
};

#endif