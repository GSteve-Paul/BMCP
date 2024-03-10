#include "SetList.hpp"
#include <algorithm>

void BMCP::SetList::insert(const int obj)
{
    push_back(obj);
}

void BMCP::SetList::erase(const int index)
{
    std::swap(*(_begin + index), back());
    pop_back();
}
