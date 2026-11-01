/*
 * I am using this file to test our DynamicCharArray functions and check if they work correctly.
 * I am running different tests and printing PASS if they succeed or FAIL if they do not.
 * I am using <cstdio> to print the results. Our DynamicCharArray itself does not use STL or Qt.
 */

#include <cstdio>
#include "dynamicchararray.h"
static int g_checks = 0; // I am using this variable to count the total number of checks we run.
static int g_failed = 0; // I am using this variable to count how many checks fail.
// I am using CHECK to run a check, count it, and print the line number if it fails.
#define CHECK(cond) do { ++g_checks; if (!(cond)) { ++g_failed; \
std::printf("    FAIL (line %d): %s\n", __LINE__, #cond); } } while (0)
    // I am using this function to print PASS or FAIL for each test.
    static void report(const char* name, int failedBefore) {
        if (g_failed == failedBefore) {std::printf("[PASS] %s\n", name);} // I am checking if the number of failed checks stayed the same during this test.
             // I am printing PASS because no checks failed in this test.
        else {std::printf("[FAIL] %s\n", name);} // I am printing FAIL because at least one check failed.
    }

static void testDefaultIsEmpty() {
    int f = g_failed; // I am saving the number of failed checks before this test starts.
    DynamicCharArray a; // I am creating an array using the default constructor.
    CHECK(a.isEmpty()); // I am checking if the new array is empty.
    CHECK(a.size() == 0); // I am checking if the array contains zero characters.
    CHECK(a.capacity() == 16); // I am checking if the array starts with a capacity of 16 characters.
    report("default constructor gives an empty array", f); // I am reporting whether this test passed or failed.
}

static void testPushBackGrows() {
    int f = g_failed; // I am saving the number of failed checks before this test starts.
    DynamicCharArray a; // I am creating an empty array.
    for (int i = 0; i < 1000; ++i) { // I am repeating this loop 1000 times to add 1000 characters.
        CHECK(a.pushBack('A')); // I am checking if each character is added successfully.
    }
    CHECK(a.size() == 1000); // I am checking if the array contains 1000 characters.
    CHECK(a.capacity() >= 1000); // I am checking if the array has enough capacity to store all 1000 characters.
    report("pushBack grows the buffer automatically", f); // I am reporting whether the array grew correctly.
}

static void testConstructFromCString() {
    int f = g_failed; // I am saving the number of failed checks before this test starts.
    DynamicCharArray a("ACGT"); // I am creating an array from the C string "ACGT".
    CHECK(a.size() == 4); // I am checking if the array contains four characters.
    CHECK(a[0] == 'A' && a[2] == 'G' && a[3] == 'T'); // I am checking if the characters are stored at the correct positions.
    DynamicCharArray empty(""); // I am creating an array from an empty string.
    CHECK(empty.isEmpty()); // I am checking if the array created from an empty string is empty.
    DynamicCharArray nul((const char*)nullptr); // I am creating an array using a null pointer instead of a string.
    CHECK(nul.isEmpty()); // I am checking if the array stays empty when the pointer is nullptr.
    report("construct from C string (incl. empty and null)", f); // I am reporting whether the constructors worked correctly.
}

static void testCopyIsDeep() {
    int f = g_failed; // I am saving the number of failed checks before this test starts.
    DynamicCharArray a("ACGT"); // I am creating the original array.
    DynamicCharArray b(a); // I am creating a copy of the original array.
    b.set(0, 'T'); // I am changing the first character in the copied array.
    CHECK(a[0] == 'A'); // I am checking that the original array still contains 'A' at index 0.
    CHECK(b[0] == 'T'); // I am checking that the copied array now contains 'T' at index 0.
    report("copy constructor makes a deep copy", f); // I am checking that changing the copy does not change the original.
}

static void testAssignment() {
    int f = g_failed; // I am saving the number of failed checks before this test starts.
    DynamicCharArray a("ACGT"), b; // I am creating an array with characters and another empty array.
    b = a; // I am copying the contents of a into b.
    b = b; // I am assigning b to itself to check that self-assignment is safe.
    CHECK(b.equals(a)); // I am checking if b contains the same characters as a.
    b.set(1, 'X'); // I am changing the character at index 1 in b.
    CHECK(a[1] == 'C'); // I am checking that changing b did not change the original array a.
    report("operator= is deep and self-assignment safe", f); // I am reporting whether assignment works correctly.
}

static void testAppend() {
    int f = g_failed; // I am saving the number of failed checks before this test starts.
    DynamicCharArray a("AC"); // I am creating an array containing "AC".
    CHECK(a.append("GT")); // I am adding "GT" to the end of the array and checking if it succeeds.
    CHECK(a.equals(DynamicCharArray("ACGT"))); // I am checking if the array now contains "ACGT".
    a.append(a); // I am appending the array to itself to check if self-append works correctly.
    CHECK(a.equals(DynamicCharArray("ACGTACGT"))); // I am checking if the array now contains "ACGTACGT".
    report("append (C string, other array, itself)", f); // I am reporting whether the append operations worked correctly.
}

static void testCheckedAccess() {
    int f = g_failed; // I am saving the number of failed checks before this test starts.
    DynamicCharArray a("AC"); // I am creating an array containing "AC".
    char c = 0; // I am creating a character variable to store a character retrieved from the array.
    CHECK(a.get(1, c) && c == 'C'); // I am checking if get() retrieves the character 'C' from index 1.
    CHECK(!a.get(2, c)); // I am checking that get() returns false when we request an index outside the array.
    CHECK(!a.set(5, 'X')); // I am checking that set() returns false when we try to change an invalid index.
    report("get/set reject invalid indices", f); // I am reporting whether get() and set() handle invalid indexes correctly.
}

static void testPopBackAndClear() {
    int f = g_failed; // I am saving the number of failed checks before this test starts.
    DynamicCharArray a("AC"); // I am creating an array containing "AC".
    char c = 0; // I am creating a character variable to store the character we remove.
    CHECK(a.popBack(c) && c == 'C'); // I am checking if popBack() removes the last character and stores 'C' in c.
    CHECK(a.size() == 1); // I am checking if only one character remains after removing the last character.
    a.clear(); // I am removing all the characters from the array while keeping its allocated memory.
    CHECK(a.isEmpty()); // I am checking if the array is now empty.
    CHECK(!a.popBack(c)); // I am checking that popBack() returns false when the array is empty.
    report("popBack and clear", f); // I am reporting whether popBack() and clear() work correctly.
}

static void testMatchesAt() {
    int f = g_failed; // I am saving the number of failed checks before this test starts.
    DynamicCharArray t("ACGTACGT"), p("ACG"); // I am creating the text we want to search and the pattern we want to find.
    CHECK(t.matchesAt(0, p)); // I am checking if the pattern matches the text starting at index 0.
    CHECK(t.matchesAt(4, p)); // I am checking if the pattern matches the text starting at index 4.
    CHECK(!t.matchesAt(1, p)); // I am checking that the function returns false when the pattern does not match at index 1.
    CHECK(!t.matchesAt(7, p)); // I am checking that the function returns false when the pattern would go past the end of the text.
    CHECK(!t.matchesAt(100, p)); // I am checking that the function returns false when the starting position is outside the text.
    CHECK(!DynamicCharArray("AC").matchesAt(0, DynamicCharArray("ACGT"))); // I am checking that a pattern longer than the text cannot match.
    CHECK(t.matchesAt(0, DynamicCharArray())); // I am checking that an empty pattern matches at a valid position.
    report("matchesAt edge cases", f); // I am reporting whether matchesAt() handles these different cases correctly.
}

static void testSubstr() {
    int f = g_failed; // I am saving the number of failed checks before this test starts.
    DynamicCharArray t("ACGTACGT"); // I am creating an array containing "ACGTACGT".
    CHECK(t.substr(2, 3).equals(DynamicCharArray("GTA"))); // I am checking if substr() returns three characters starting at index 2.
    CHECK(t.substr(6, 99).size() == 2); // I am checking that substr() only returns the remaining two characters when the requested count goes past the end.
    CHECK(t.substr(50, 2).isEmpty()); // I am checking that substr() returns an empty array when the starting position is outside the array.
    report("substr clamps and handles out-of-range", f); // I am reporting whether substr() handles these cases correctly.
}

static void testReserveAndZeroCapacity() {
    int f = g_failed; // I am saving the number of failed checks before this test starts.
    DynamicCharArray a((size_t)0); // I am creating an array with an initial capacity of zero.
    CHECK(a.capacity() == 0); // I am checking that the array starts with zero capacity.
    CHECK(a.pushBack('x')); // I am checking that pushBack() can add a character even when the array starts with zero capacity.
    CHECK(a.size() == 1); // I am checking that the array now contains one character.
    CHECK(a.reserve(500)); // I am checking that reserve() can increase the capacity to at least 500.
    CHECK(a.capacity() >= 500 && a.size() == 1); // I am checking that the capacity is at least 500 and the existing character is still stored.
    report("zero capacity and reserve", f); // I am reporting whether zero capacity and reserve() work correctly.
}

int main() {
    std::printf("DynamicCharArray tests \n"); // I am printing a heading to show that the tests have started.
    testDefaultIsEmpty(); // I am testing the default constructor.
    testPushBackGrows(); // I am testing whether pushBack() automatically increases capacity.
    testConstructFromCString(); // I am testing construction from C strings.
    testCopyIsDeep(); // I am testing whether the copy constructor creates an independent copy.
    testAssignment(); // I am testing assignment and self-assignment.
    testAppend(); // I am testing appending strings and arrays.
    testCheckedAccess(); // I am testing get() and set() with valid and invalid indexes.
    testPopBackAndClear(); // I am testing removing the last character and clearing the array.
    testMatchesAt(); // I am testing pattern matching at different positions.
    testSubstr(); // I am testing substring creation.
    testReserveAndZeroCapacity(); // I am testing reserve() and adding characters when capacity starts at zero.
    std::printf("=== %d checks, %d failed ===\n", g_checks, g_failed); // I am printing the total number of checks and how many failed.
    if (g_failed == 0) {return 0;} // I am returning 0 to show that the program finished successfully when all checks pass.
    else {return 1;} // I am returning 1 to show that at least one check failed.
}