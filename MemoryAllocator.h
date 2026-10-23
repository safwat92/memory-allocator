#pragma once

#include <cstddef>
#include <vector>
using namespace std;

struct MemoryBlock
{
    void *address;
    size_t size;
    bool isFree;
};

class MemoryAllocator
{
private:
    static constexpr size_t BUFFER_SIZE = 16;
    byte heap[BUFFER_SIZE];
    size_t used = 0;
    vector<MemoryBlock> v;

public:
    void *allocate(size_t bytes);

    void deallocate(void *p);

    void coalesce();

    void reset();
};
