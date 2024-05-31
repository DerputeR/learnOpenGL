#include <vector>

#pragma once

/**
 * @brief Templated class to help create bidirectional maps using std::vector
 * The sparseMap will map index K to I, and the packedMap will map index I back to V.
 * Note that this will NOT manage a value vector; this is up to you to manage
 * @tparam K integer type used to index sparseMap to get indices I
 * @tparam I integer type used to index packedMap to get keys K
 */
template <typename K, typename I>
struct SparseMap
{
private:
    K invalidKey;
    I invalidIndex;
public:
    std::vector<I> sparseMap;
    std::vector<K> packedMap;

    SparseMap(K invalidKey, I invalidIndex) :
        invalidKey{ invalidKey },
        invalidIndex{ invalidIndex } { }

    /**
     * @brief Appends packedMap and maps key to the new back
     * @param key 
     */
    void map(K key)
    {
        if (key == invalidKey)
        {
            return;
        }
        size_t sparseSize = sparseMap.size();
        size_t newSize = sparseSize;
        while (key >= newSize)
        {
            if (newSize == 0)
            {
                newSize = 10;
            }
            else
            {
                newSize *= 2;
            }
        }
        if (newSize > sparseSize)
        {
            sparseMap.resize(newSize, invalidIndex);
        }

        I packedIndex = static_cast<I>(packedMap.size());
        packedMap.push_back(key);
        sparseMap[key] = packedIndex;
    }

    /**
     * @brief Removes the bi-directional mapping between a key and its index.
     * 
     * IMPORTANT: if you are using this SparseMap to work with another packed vector, make sure
     * that you do the following steps on it so that its structure matches with packedMap:
     * 1. vec[key] = vec.back()
     * 2. vec.pop_back()
     * 
     * @param key 
     */
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

        // copy the back packedMap to index
        packedMap[index] = backKey;
        // pop the back of packedMap
        packedMap.pop_back();

        // now that the back is located at index, update the key to point here
        sparseMap[backKey] = index;

        // finally, unmap key
        sparseMap[key] = invalidIndex;
    }
};