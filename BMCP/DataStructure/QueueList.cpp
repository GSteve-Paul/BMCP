#include "QueueList.hpp"

BMCP::QueueList::QueueList(int size) : IntList(size)
{
    for (int *ptr = this->_begin; ptr != this->_end; ++ptr)
        *ptr = -1;
    head = 0;
}

void BMCP::QueueList::push_and_pop(int obj)
{
    head = (head - 1 + size()) % size();
    *(_begin + head) = obj;
}
