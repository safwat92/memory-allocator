#include <iostream>
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
    void *allocate(size_t bytes)
    {
        if (bytes <= 0)
            return nullptr;

        for (int i = 0; i < v.size(); i++)
        {
            if (!v[i].isFree || v[i].size < bytes)
                continue;

            void *address = v[i].address;
            size_t size = v[i].size;

            if (v[i].size == bytes)
            {
                v[i].isFree = false;
                return v[i].address;
            }

            v[i].size = bytes;
            v[i].isFree = false;

            void *newAddress = static_cast<byte *>(address) + bytes;
            size_t newSize = size - bytes;

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

    void deallocate(void *p)
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

    // Coalescing free blocks into one block
    void coalesce()
    {
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

    void reset()
    {
        memset(heap, 0, sizeof(heap));
        v.clear();
        used = 0;
    }
};

int main()
{
    MemoryAllocator m;

    void *p1 = m.allocate(2);
    void *p2 = m.allocate(2);
    void *p3 = m.allocate(4);

    m.deallocate(p1);
    m.deallocate(p2);

    void *p4 = m.allocate(4);

    cout << "p1: " << p1 << '\n';
    cout << "p2: " << p2 << '\n';
    cout << "p3: " << p3 << '\n';
    cout << "p4: " << p4 << '\n';
}