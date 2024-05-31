#include <vector>

#pragma once

/**
 * @brief Templated class to help create bidirectional maps using std::vector
 * The sparseMap will map index K to V, and the packedMap will map index V back to K.
 * @tparam K integer type used to index the maps
 * @tparam V values stored by the packed vector
 */
template <typename K, typename I, class V>
struct SparseMap
{
private:
    K invalidKey;
    I invalidIndex;
public:
    std::vector<I> sparseMap;
    std::vector<K> packedMap;
    std::vector<V> packed;

    SparseMap(K invalidKey, I invalidIndex) :
        invalidKey{ invalidKey },
        invalidIndex{ invalidIndex } { }

    void map(K key, V value)
    {
        if (key == invalidKey)
        {
            return;
        }
        size_t sparseSize = sparseMap.size();
        size_t newSize = sparseSize;
        while (key > newSize)
        {
            if (newSize == 0)
            {
                newSize = key + 1;
            }
            else
            {
                newSize *= 2;
            }
        }
        if (newSize > sparseSize)
        {
            sparseMap.resize(sparseSize);
        }

        I packedIndex = packed.size();
        packedMap.push_back(key);
        packed.push_back(value);
        sparseMap[key] = packedIndex;
    }

    void unmap(K key)
    {
        // return if key is invalid
        if (key >= sparseMap.size()) return;
        I index = sparseMap[key];
        // return if key is already unmapped
        if (index == invalidIndex) return;

        // we will copy the back to the slot we want removed,
        // pop the back, then make sure the key that had the back
        // now points to the replaced slot, and that the replaced slot is
        // linked back to said key
        K backKey = packedMap.back();
        I backIndex = sparseMap[backKey];

        // copy the back of packed and packedMap to index
        packed[index] = packed[backIndex];
        packedMap[index] = backKey;
        // pop the back of packed and packedMap
        packed.pop_back();
        packedMap.pop_back();

        // now that the back is located at index, update the key to point here
        sparseMap[backKey] = index;

        // finally, unmap key
        sparseMap[key] = invalidIndex;
    }
};