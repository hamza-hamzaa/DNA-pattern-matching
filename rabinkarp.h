/*
We made this file to declare the Rabin-Karp search algorithm.
Rabin-Karp does not compare the pattern with every window of the text character by character like brute force.
Instead it turns the pattern into one number called a hash and it turns each window of the text into a hash too.
It only compares characters when the two hashes are equal.
The hash of the next window is calculated from the hash of the current window. This is called a rolling hash.
This file only declares the function. The next file explains how the function works.
*/

#ifndef RABINKARP_H
#define RABINKARP_H

#include <cstddef>   // I am using this so I can be able to use size_t. This will help me store the number of matches.
#include <fstream>   // I am using this so I can be able to use ifstream. The text we search in is stored in a file.
#include <string>    // I am using this so I can be able to use string. The pattern we search for is a string.

size_t rabinKarpSearch(std::ifstream& file, const std::string& pattern); // I am using this function to count how many times the pattern appears in the text file.
// It has two parameters. The first one is the ifstream which is the file that contains the text. The second one is the pattern we are looking for.
// It returns the number of times the pattern appears. Matches that overlap are also counted just like in brute force.
// The file is passed by reference because reading from a file changes the file stream. The pattern is passed as a const reference so we do not copy it and we cannot change it.
// The function reads the text from the beginning of the file every time it is called so we can call it again on the same file stream.
// If the pattern is empty or longer than the text or the file could not be opened the function returns 0 because nothing can match.
// Time complexity is O(n+m) on average because we only compare characters when the hashes are equal.
// In the worst case many windows have the same hash as the pattern so it becomes O(n*m).

#endif