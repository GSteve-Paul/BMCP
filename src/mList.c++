#include "../include/mList.h++"
#include <stdexcept>

void mList::push(int num)
{
    container[size++] = num;
}

void mList::pop()
{
    size--;
}

void mList::clear()
{
    size = 0;
}

int &mList::operator[](const int &index)
{
    if (index >= 0 && index < size)
        return container[index];
    else
        throw std::out_of_range("Index out of range");
}

int mList::back()
{
    return container[size - 1];
}

int mList::front()
{
    return container[0];
}
