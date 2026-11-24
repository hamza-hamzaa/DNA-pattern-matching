// Tests for HashTable. Each test creates its own table, so a failure in one can't affect another.
#include "../DataStructures/HashTable.h"
#include <iostream>

// Counts the failed checks so main() can report a total at the end.
int failures = 0;

// Prints the test name and the failed check, then keeps running the other tests.
void check(bool condition, const char* testName, const char* description) {
    if (!condition) {
        std::cout << "FAIL [" << testName << "] " << description << "\n";
        failures++;
    }
}

// A new table holds nothing, and lookups must say so.
void testEmptyTable() {
    HashTable table;
    int value = -1;

    check(table.getSize() == 0, "empty", "size starts at 0");
    check(!table.contains('A'), "empty", "contains() is false");
    check(!table.get('A', value), "empty", "get() returns false");
    check(value == -1, "empty", "get() leaves outValue alone when the key is missing");
}

// A stored key can be read back, and other keys stay absent.
void testInsertAndGet() {
    HashTable table;
    int value = 0;

    table.insertOrUpdate('A', 10);

    check(table.getSize() == 1, "insert", "size is 1");
    check(table.get('A', value), "insert", "get() finds the key");
    check(value == 10, "insert", "get() returns the stored value");
    check(table.contains('A'), "insert", "contains() is true");
    check(!table.contains('C'), "insert", "a different key is still absent");
}

// Inserting an existing key replaces its value and doesn't add an entry.
void testUpdate() {
    HashTable table;
    int value = 0;

    table.insertOrUpdate('A', 1);
    table.insertOrUpdate('A', 2);

    check(table.getSize() == 1, "update", "size is still 1");
    check(table.get('A', value) && value == 2, "update", "the value is the newest one");
}

// With capacity 4, 'A' (65) and 'E' (69) both hash to slot 1, so 'E' has to probe.
void testCollision() {
    HashTable table(4);
    int value = 0;

    table.insertOrUpdate('A', 1);
    table.insertOrUpdate('E', 2);

    check(table.getSize() == 2, "collision", "both keys are stored");
    check(table.get('A', value) && value == 1, "collision", "first key keeps its value");
    check(table.get('E', value) && value == 2, "collision", "second key keeps its value");
    check(!table.contains('I'), "collision", "a third key with the same hash is absent");

    // Updating the probed key must not touch the key in its home slot.
    table.insertOrUpdate('E', 3);
    check(table.getSize() == 2, "collision", "updating a probed key doesn't add an entry");
    check(table.get('E', value) && value == 3, "collision", "probed key is updated");
    check(table.get('A', value) && value == 1, "collision", "other key is unchanged");
}

// With capacity 4, 'C' (67) and 'G' (71) both hash to the last slot, so 'G' wraps to slot 0.
void testWrapAround() {
    HashTable table(4);
    int value = 0;

    table.insertOrUpdate('C', 1);
    table.insertOrUpdate('G', 2);

    check(table.get('C', value) && value == 1, "wrap", "key in the last slot is found");
    check(table.get('G', value) && value == 2, "wrap", "wrapped key is found");
}

// Starting from capacity 2, all 256 char values force several rehashes.
// Keys go in descending order so their slots change at each rehash. In ascending order,
// each key's slot equals its own number, which would hide a rehash that doesn't re-hash.
void testResize() {
    HashTable table(2);
    int value = 0;

    for (int i = 255; i >= 0; i--) {
        table.insertOrUpdate(static_cast<char>(i), i * 2);
    }

    check(table.getSize() == 256, "resize", "size counts every key");

    bool allFound = true;
    for (int i = 0; i < 256; i++) {
        if (!table.get(static_cast<char>(i), value) || value != i * 2) {
            allFound = false;
        }
    }
    check(allFound, "resize", "every key keeps its value across rehashes");
}

// Chars above 127 can be negative, which would give hash() a negative index without its cast.
void testHighChars() {
    HashTable table;
    int value = 0;

    table.insertOrUpdate(static_cast<char>(128), 1);
    table.insertOrUpdate(static_cast<char>(200), 2);
    table.insertOrUpdate(static_cast<char>(255), 3);

    check(table.get(static_cast<char>(128), value) && value == 1, "high chars", "char 128 is found");
    check(table.get(static_cast<char>(200), value) && value == 2, "high chars", "char 200 is found");
    check(table.get(static_cast<char>(255), value) && value == 3, "high chars", "char 255 is found");
    check(!table.contains(static_cast<char>(201)), "high chars", "an unused high char is absent");
}

// clear() empties the table, and the table still works afterwards.
void testClear() {
    HashTable table;
    int value = 0;

    table.insertOrUpdate('A', 1);
    table.insertOrUpdate('C', 2);
    table.clear();

    check(table.getSize() == 0, "clear", "size is 0");
    check(!table.contains('A'), "clear", "first key is gone");
    check(!table.contains('C'), "clear", "second key is gone");

    table.insertOrUpdate('G', 3);
    check(table.getSize() == 1, "clear", "size counts new keys");
    check(table.get('G', value) && value == 3, "clear", "table can be reused");
}

// The brief's "ABCD" example, filled in by hand because the HashTable doesn't compute shifts.
void testShiftExample() {
    HashTable table;
    int value = 0;

    table.insertOrUpdate('A', 3);
    table.insertOrUpdate('B', 2);
    table.insertOrUpdate('C', 1);

    check(table.get('A', value) && value == 3, "shift", "A shifts by 3");
    check(table.get('B', value) && value == 2, "shift", "B shifts by 2");
    check(table.get('C', value) && value == 1, "shift", "C shifts by 1");

    // Missing keys are where BMH falls back to the pattern length, 4.
    check(!table.get('D', value), "shift", "D is absent because the last char is skipped");
    check(!table.get('Z', value), "shift", "Z is absent because it isn't in the pattern");
}

// Runs every test and reports the result through the exit code.
int main() {
    testEmptyTable();
    testInsertAndGet();
    testUpdate();
    testCollision();
    testWrapAround();
    testResize();
    testHighChars();
    testClear();
    testShiftExample();

    if (failures == 0) {
        std::cout << "All checks passed\n";
        return 0;
    }
    std::cout << failures << " check(s) failed\n";
    return 1;
}
