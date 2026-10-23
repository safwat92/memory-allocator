#include <iostream>
#include "MemoryAllocator.h"

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