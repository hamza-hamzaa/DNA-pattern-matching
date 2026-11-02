#ifndef HASHTABLE_H
#define HASHTABLE_H

// One slot of the table.
struct Node {
    char key;       // the character stored in this slot
    int  value;     // the number stored for that character
    bool occupied;  // true if the slot is in use; needed because any char, even '\0', can be a valid key
};

// Stores char -> int pairs using open addressing with linear probing.
// It only stores and retrieves pairs: Boyer-Moore-Horspool computes the shifts and owns a table of them.
// There is no deletion, so an empty slot always means a search can stop.
class HashTable {
private:
    // Highest allowed size / capacity. Lower means shorter probes but more unused memory.
    static constexpr double MAX_LOAD = 0.7;

    Node* table;    // dynamic array of slots, owned by this object
    int capacity;   // number of slots in `table`; always at least 1, so `% capacity` is safe
    int size;       // number of keys currently stored

    // Returns the slot where a search for `key` starts, in [0, capacity).
    int  hash(char key) const;
    // Moves every entry into a new array of `newCapacity` slots.
    void rehash(int newCapacity);

public:
    HashTable(int initialCapacity = 53); // prime number for better distribution&decrease collisions
    ~HashTable();

    // Copies are deep: a default copy would share `table`, so the array would be freed twice.
    HashTable(const HashTable& other);
    HashTable& operator=(const HashTable& other);

    // Adds the key, or overwrites its value if it already exists.
    void insertOrUpdate(char key, int value);
    // Returns true and writes the value to outValue, or returns false and leaves outValue unchanged.
    bool get(char key, int& outValue) const;
    // Returns true if the key is stored.
    bool contains(char key) const;
    // Removes all keys but keeps the current capacity.
    void clear();
    // Returns the number of keys stored.
    int  getSize() const;
};

#endif
