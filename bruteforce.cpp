/*
We made this file to write the actual implementation of the brute force search.
The header file tells us what the function does while this file explains how it works.
*/
#include "bruteforce.h"
using namespace std;

size_t bruteForceSearch(ifstream& file, const string& pattern) {
    if (!file.is_open()) {return 0;} // I am checking if the file was opened. If it was not there is no text to search in so I return 0.
    file.clear(); // I am clearing any error flags so the stream can be read again.
    file.seekg(0); // I am moving back to the beginning of the file so we always read the whole text.

    string text; // I am using this string to store the text we read from the file. We read the whole file once so we can look at any position of the text directly.
    char c; // I am using this variable to store the character we just read.
    bool atLineStart = true; // I am using this variable to know if we are at the beginning of a line.
    bool inHeader = false; // I am using this variable to know if we are inside a header line that starts with the > symbol.
    while (file.get(c)) { // I am reading the file one character at a time until there is nothing left to read.
        if (c == '\n') {atLineStart = true; inHeader = false; continue;} // A new line starts here so the header line is over. I skip the new line character itself.
        if (atLineStart && c == '>') {inHeader = true;} // A line that starts with > is a header in a FASTA file so I mark it.
        atLineStart = false; // We already read a character of this line so we are no longer at its beginning.
        if (inHeader || c == ' ') {continue;} // I am skipping header lines and spaces because they are not part of the DNA text.
        text += c; // I am adding the character to the end of the text.
    }

    size_t textsize = text.size(); // I am using this variable to store the number of characters in the text.
    size_t textpattern = pattern.size(); // I am using this variable to store the number of characters in the pattern.
    size_t count = 0; // I am using this variable to count how many matches we found so far.

    if (textpattern == 0 || textpattern > textsize) {return 0;} // I am checking if the pattern is empty or longer than the text. If it is nothing can match so I return 0.

    for (size_t i = 0; i + textpattern <= textsize; i++) { // I am trying every starting index where the whole pattern still fits inside the text.
        size_t j = 0; // I am using this variable to count how many characters of the pattern matched at this starting index.
        while (j < textpattern && text[i + j] == pattern[j]) {j++;} // I keep comparing the next character of the text with the next character of the pattern.
        // I stop as soon as I find a different character or when I reach the end of the pattern.
        if (j == textpattern) {count++;} // I am checking if all the characters of the pattern matched. If they did I increase the count because we found one more match.
    }
    return count; // I return the total number of matches we found. It is 0 if the pattern never appeared.
}