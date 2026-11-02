#include "HashTable.h"

// Allocates the array and marks every slot empty.
// `new Node[n]` leaves the fields holding garbage, so `occupied` must be set by hand.
HashTable::HashTable(int initialCapacity) {
    // A capacity of 0 or less would make `% capacity` in hash() undefined, so fall back to 1.
    capacity = (initialCapacity > 0) ? initialCapacity : 1;
    size = 0;
    table = new Node[capacity];

    for (int i = 0; i < capacity; i++) {
        table[i].occupied = false;
    }
}

// The array came from new[], so it must be released with delete[].
HashTable::~HashTable() {
    delete[] table;
}

// Copy constructor: builds a brand-new table that owns its OWN array.
// Slots keep their positions because the copy has the same capacity, so every key hashes the same.
HashTable::HashTable(const HashTable& other) {
    capacity = other.capacity;
    size     = other.size;
    table    = new Node[capacity];
    for (int i = 0; i < capacity; i++)
        table[i] = other.table[i];
}

// Copy assignment: replaces this table's contents with a deep copy of `other`.
// The new array is built before the old one is deleted, so if `new` throws, this table is left intact.
HashTable& HashTable::operator=(const HashTable& other) {
    if (this == &other) return *this;

    Node* newTable = new Node[other.capacity];
    for (int i = 0; i < other.capacity; i++)
        newTable[i] = other.table[i];

    delete[] table;
    table    = newTable;
    capacity = other.capacity;
    size     = other.size;
    return *this;
}

// Maps a char to an index in [0, capacity).
// The cast matters: plain char can be signed, so a byte above 127 could be negative,
// and a negative value % capacity gives a negative index, which is out of bounds.
int HashTable::hash(char key) const {
    return static_cast<unsigned char>(key) % capacity;
}

// Moves every entry into a bigger array.
// Entries can't be copied to the same positions: an index depends on capacity,
// so each key has to be hashed and probed again using the new capacity.
void HashTable::rehash(int newCapacity) {
    Node* oldTable = table; // Save the old array so we can re-insert its entries.
    int oldCapacity = capacity; // Save the old capacity so we can loop through the old array.

    // hash() reads the `capacity` member, so it must be updated before re-inserting.
    table = new Node[newCapacity]; 
    capacity = newCapacity;

    for (int i = 0; i < capacity; i++) {
        table[i].occupied = false;
    }

    for (int i = 0; i < oldCapacity; i++) {
        if (oldTable[i].occupied) {
            int index = hash(oldTable[i].key);

            // Keys in the old table are unique, so we only need the first empty slot.
            while (table[index].occupied) { 
                index = (index + 1) % capacity; //
            }

            table[index] = oldTable[i];//
        }
    }

    // `size` is unchanged: the same entries are stored, just in a bigger array.
    delete[] oldTable; 
}

// Linear probing: start at the hashed index and step forward (wrapping around)
// until we reach either the key or an empty slot.
void HashTable::insertOrUpdate(char key, int value) {
    int index = hash(key);   // the slot currently being checked

    // If the key is already present, overwrite its value. Otherwise, find an empty slot.
    while (table[index].occupied) { 
        if (table[index].key == key) {
            // Key already exists: overwrite its value. `size` doesn't change.
            table[index].value = value;
            return;
        }
        index = (index + 1) % capacity;
    }

    // We reached an empty slot, so the key is new. Grow if adding it would push the
    // load factor (size / capacity) above MAX_LOAD. The cast avoids integer division,
    // which would round the result down to 0. Checking here means updates alone
    // never trigger a rehash.
    if (static_cast<double>(size + 1) / capacity > MAX_LOAD) {
        rehash(capacity * 2 + 1);

        // The slot we found belongs to the old array, so probe again in the new one.
        index = hash(key);
        while (table[index].occupied) {
            index = (index + 1) % capacity;
        }
    }

    table[index].key = key;
    table[index].value = value;
    table[index].occupied = true;
    size++;
}

// Follows the same path insertOrUpdate() used. Reaching an empty slot means the key
// is absent, because an insert would have stopped there. The load factor limit
// guarantees an empty slot always exists, so this loop always ends.
bool HashTable::get(char key, int& outValue) const {
    int index = hash(key);   // the slot currently being checked

    while (table[index].occupied) {
        if (table[index].key == key) {
            outValue = table[index].value;
            return true;
        }
        index = (index + 1) % capacity;
    }

    return false;
}

// Reuses get() so the probing logic lives in one place; the value itself is ignored.
bool HashTable::contains(char key) const {
    int ignored;   // get() needs somewhere to write the value; it is never read
    return get(key, ignored);
}

// Empties the table but keeps the allocated array and its current capacity.
// Clearing `occupied` is enough, since a slot's old key/value are never read once it's empty.
void HashTable::clear() {
    for (int i = 0; i < capacity; i++) {
        table[i].occupied = false;
    }
    size = 0;
}

// Number of keys stored (not the capacity of the array).
int HashTable::getSize() const {
    return size;
}
