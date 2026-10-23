#include "MemoryAllocator.h"

void *MemoryAllocator::allocate(size_t bytes)
{
    // this function checks if there any free blocks that can reserve instead of 
    // reserving a new memory block, if dind't find any it reserves a new block, calculate
    // address and amount of memory used

    if (bytes <= 0)
        return nullptr;


    // check for free blocks
    for (int i = 0; i < v.size(); i++)
    {
        if (!v[i].isFree || v[i].size < bytes)
            continue;

        void *address = v[i].address;
        size_t size = v[i].size;

        if (v[i].size == bytes)
        {
            // reserve block if it matches the needed bytes exactly
            v[i].isFree = false;
            return v[i].address;
        }

        // split the current block into the needed size
        v[i].size = bytes;
        v[i].isFree = false;

        // calculate metadata of the new splitted block
        void *newAddress = static_cast<byte *>(address) + bytes;
        size_t newSize = size - bytes;

        // create new block with the remaining size
        v.push_back({newAddress, newSize, true});

        return address;
    }

    if (BUFFER_SIZE - used < bytes)
        return nullptr;

    void *address = heap + used;

    v.push_back({address, bytes, false});

    used += bytes;

    return address;
}

void MemoryAllocator::deallocate(void *p)
{
    for (auto &block : v)
    {
        if (block.address == p && !block.isFree)
        {
            block.isFree = true;
            break;
        }
    }
    coalesce();
}

void MemoryAllocator::coalesce()
{
    // this function merges any free blocks staying side by side into one block; instead
    // of reserving new block
    for (size_t i = 0; i < v.size(); i++)
    {
        for (size_t j = i + 1; j < v.size();)
        {
            void *expectedAddress = static_cast<byte *>(v[i].address) + v[i].size;
            if (v[i].isFree && v[j].isFree && v[j].address == expectedAddress)
            {
                v[i].size += v[j].size;
                v.erase(v.begin() + j);
            }
            else
            {
                j++;
            }
        }
    }
}

void MemoryAllocator::reset()
{
    memset(heap, 0, sizeof(heap));
    v.clear();
    used = 0;
}