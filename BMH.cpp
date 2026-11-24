#include "BMH.h"
#include "HashTable.h"
#include<string>
using namespace std;

// Check that the DNA string contains only A, C, G, and T.
static bool isValidDNABMH(const string& dna)
{
    for (int i = 0; i < (int)dna.length(); i++)
    {
        if (dna[i] != 'A' && dna[i] != 'C' &&
            dna[i] != 'G' && dna[i] != 'T')
        {
            return false;
        }
    }

    return true;
}

int searchBMH(const string& dna, const string& pattern)
{
    int textLength = (int)dna.length();
    int patternLength = (int)pattern.length();

    // An empty pattern or invalid DNA input is not accepted.
    if (patternLength == 0 || !isValidDNABMH(dna) || !isValidDNABMH(pattern))
        return -1;

    // The pattern cannot fit inside the DNA string.
    if (patternLength > textLength)
        return 0;

    // Build the BMH shift table using the existing HashTable class.
    HashTable shifts(53);

    // The last character is not added to the table.
    for (int i = 0; i < patternLength - 1; i++)
    {
        int shift = patternLength - 1 - i;
        shifts.insertOrUpdate(pattern[i], shift);
    }

    int matches = 0;
    int position = 0;

    // Keep checking while the pattern still fits in the DNA string.
    while (position <= textLength - patternLength)
    {
        int j = patternLength - 1;

        // Compare the pattern with the DNA string from right to left.
        while (j >= 0 && pattern[j] == dna[position + j])
        {
            j--;
        }

        // If j reached -1, every character matched.
        if (j < 0)
        {
            matches++;
        }

        // Use the character aligned with the pattern's last position
        // to decide how far the pattern can move.
        int shift = patternLength;
        int storedShift;

        if (shifts.get(dna[position + patternLength - 1], storedShift))
        {
            shift = storedShift;
        }

        // Move the pattern forward. This also allows overlapping matches.
        position += shift;
    }

    return matches;
}





