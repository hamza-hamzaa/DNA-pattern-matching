/*
We made this file to declare the brute force search algorithm.
Brute force is the simplest way to search for a pattern inside a text.
We try every possible starting position in the text and compare the pattern with the text character by character.
We keep it in our project as the baseline because it is easy to understand and easy to trust.
This file only declares the function. The next file explains how the function works.
*/

#ifndef BRUTEFORCE_H
#define BRUTEFORCE_H

#include <cstddef>   // I am using this so I can be able to use size_t. This will help me store the number of matches.
#include <fstream>   // I am using this so I can be able to use ifstream. The text we search in is stored in a file.
#include <string>    // I am using this so I can be able to use string. The pattern we search for is a string.

size_t bruteForceSearch(std::ifstream& file, const std::string& pattern); // I am using this function to count how many times the pattern appears in the text file.
// It has two parameters. The first one is the ifstream which is the file that contains the text. The second one is the pattern we are looking for.
// It returns the number of times the pattern appears. Matches that overlap are also counted. For example "AA" appears 3 times in "AAAA".
// The file is passed by reference because reading from a file changes the file stream. The pattern is passed as a const reference so we do not copy it and we cannot change it.
// The function reads the text from the beginning of the file every time it is called so we can call it again on the same file stream.
// If the pattern is empty or longer than the text or the file could not be opened the function returns 0 because nothing can match.
// Time complexity is O(n*m) in the worst case where n is the text length and m is the pattern length.

#endif