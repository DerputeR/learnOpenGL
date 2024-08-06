#include <vector>

#pragma once

/**
 * @brief Templated class to help create bidirectional maps using std::vector
 * The sparseMap will map index K to I, and the packedMap will map index I back to K.
 * Note that this will NOT manage a value vector; this is up to you to manage
 * @tparam K integer type used to index sparseMap to get indices I
 * @tparam I integer type used to index packedMap to get keys K
 */
template <typename K, typename I>
class SparseMap
{
private:
    std::vector<I> sparseMap; // Maps a key K to an index I in the packed map
    std::vector<K> packedMap; // Maps an index I back to a key K in the sparse map

    K invalidKey;
    I invalidIndex;
public:

    SparseMap(K invalidKey, I invalidIndex) :
        invalidKey{ invalidKey },
        invalidIndex{ invalidIndex }
    { }

    SparseMap(K invalidKey, I invalidIndex, size_t initialSize) :
        invalidKey{ invalidKey },
        invalidIndex{ invalidIndex },
        sparseMap(initialSize, invalidIndex)
    { }

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
     * IMPORTANT: if you are using SparseMap to work with another packed vector, make sure
     * that you do the following steps BEFORE calling unmap:
     * 1. vec[sparseMap[key]] = vec.back();
     * 2. vec.pop_back();
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

    /**
     * @brief Get the index mapped to the given key. If key isn't mapped, this returns the value of invalidIndex,
     * which was set during initialization
     * @param key 
     * @return Index mapped to key 
     */
    const I& operator[](K key) const
    {
        if (key == invalidKey || key >= sparseMap.size())
        {
            return invalidIndex;
        }
        return sparseMap[key];
    }

    /**
     * @brief Get the key mapped to the given index. If the index isn't mapped, this returns the value of invalidKey,
     * which was set during initialization
     * @param index 
     * @return Key mapped to index
     */
    const K& operator()(I index) const
    {
        if (index == invalidIndex || index >= packedMap.size())
        {
            return invalidKey;
        }
        return packedMap[index];
    }

    /**
     * @return Get read-only reference to the packed vector
     */
    const std::vector<K>& getPacked() const
    {
        return packedMap;
    }

    /**
     * @return Get read-only reference to the sparse vector
     */
    const std::vector<I>& getSparse() const
    {
        return sparseMap;
    }
};