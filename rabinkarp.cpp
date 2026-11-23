/*
We made this file to write the actual implementation of the Rabin-Karp search.
The header file tells us what the function does while this file explains how it works.
The hash of a string s of length m is calculated like a number written in base 256:
hash = s[0]*256^(m-1) + s[1]*256^(m-2) + ... + s[m-1] and then we take the remainder after dividing by a big number called MOD.
We use 256 because a char can have 256 different values so every character is one digit of the number.
We take the remainder so the hash always stays small enough to store.
*/
#include "rabinkarp.h"
using namespace std;

static const long long BASE = 256; // I am using 256 as the base of the hash because a char can have 256 different values.
static const long long MOD = 1000000007; // I am using this big prime number to keep the hash small. A big prime makes it rare for two different strings to have the same hash.

size_t rabinKarpSearch(ifstream& file, const string& pattern) {
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
    size_t count = 0; // I am using this variable to count how many real matches we found so far.

    if (textpattern == 0 || textpattern > textsize) {return 0;} // I am checking if the pattern is empty or longer than the text. If it is nothing can match so I return 0.

    long long power = 1; // I am using this variable to store 256 to the power of (m - 1). It is the weight of the first character of a window.
    for (size_t k = 1; k < textpattern; k++) {power = (power * BASE) % MOD;} // I am multiplying by the base m - 1 times and I take the remainder each time so the number stays small.

    long long patternHash = 0; // I am using this variable to store the hash of the pattern.
    long long windowHash = 0; // I am using this variable to store the hash of the current window of the text. It starts as the first m characters.
    for (size_t k = 0; k < textpattern; k++) { // I am going through the first m characters of the pattern and of the text at the same time.
        patternHash = (patternHash * BASE + (unsigned char)pattern[k]) % MOD; // I am moving the old digits one place to the left and adding the next pattern character.
        windowHash = (windowHash * BASE + (unsigned char)text[k]) % MOD; // I am doing the same for the first window of the text.
        // I convert the characters to unsigned char first so a negative char value cannot break the calculation.
    }

    for (size_t i = 0; i + textpattern <= textsize; i++) { // I am moving the window across the text one position at a time.
        if (windowHash == patternHash) { // I am checking if the hash of this window is the same as the hash of the pattern. If it is the window is only a candidate.
            size_t j = 0; // I am using this variable to count how many characters matched while we verify the candidate.
            while (j < textpattern && text[i + j] == pattern[j]) {j++;} // I am comparing the window with the pattern character by character because equal hashes do not guarantee equal strings.
            if (j == textpattern) {count++;} // I am checking if all the characters matched. If they did this is a real match so I increase the count.
        }
        if (i + textpattern < textsize) { // I am checking if there is another window after this one. For the last window there is nothing left to roll to.
            long long outgoing = ((unsigned char)text[i] * power) % MOD; // I am calculating the part of the hash that belongs to the first character of the window which is about to leave.
            windowHash = (windowHash - outgoing + MOD) % MOD; // I am removing the leaving character from the hash. I add MOD first so the subtraction can never become negative.
            windowHash = (windowHash * BASE + (unsigned char)text[i + textpattern]) % MOD; // I am moving the remaining digits one place to the left and adding the new character that enters the window.
        }
    }
    return count; // I return the total number of real matches we found. It is 0 if the pattern never appeared.
}