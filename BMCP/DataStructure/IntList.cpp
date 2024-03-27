#include "IntList.hpp"
#include <stdexcept>
#include <string>

BMCP::IntList::IntList()
{
    _begin = nullptr;
    _capacity = 0;
    _end = nullptr;
}

BMCP::IntList::IntList(const int size)
{
    _begin = new int[size];
    _capacity = size;
    _end = _begin + size;
}

int BMCP::IntList::size() const
{
    return _end - _begin;
}

int &BMCP::IntList::operator[](const int index)
{
    if (index >= 0 && index < size()) [[likely]]
        return *(_begin + index);
    else
        throw std::out_of_range("Index " + std::to_string(index) + " Out of Range");
}

void BMCP::IntList::push_back(int value)
{
    if (size() + 1 > _capacity) [[unlikely]]
    {
        expand();
    }
    *_end = value;
    _end++;
}

void BMCP::IntList::clear()
{
    _end = _begin;
}

BMCP::IntList::~IntList()
{
    delete[] _begin;
}

void BMCP::IntList::expand()
{
    if (_capacity == 0) [[unlikely]]
    {
        _begin = new int[16];
        _capacity = 16;
        _end = _begin;
        return;
    }
    int sz = size();
    int *new_begin = new int[2 * _capacity];
    for (int *i = _begin, *j = new_begin; i < _end; i++, j++)
    {
        *j = *i;
    }
    delete[] _begin;
    _begin = new_begin;
    _end = _begin + sz;
    _capacity = 2 * _capacity;
}

void BMCP::IntList::pop_back()
{
    if (size() <= 0)
        [[unlikely]]
                throw std::overflow_error("Not enough int in List to be popped back");
    _end--;
}

BMCP::IntList::const_iterator BMCP::IntList::begin() const
{
    return _begin;
}

BMCP::IntList::const_iterator BMCP::IntList::end() const
{
    return _end;
}

BMCP::IntList::iterator BMCP::IntList::begin()
{
    return _begin;
}

BMCP::IntList::iterator BMCP::IntList::end()
{
    return _end;
}

int BMCP::IntList::capacity()
{
    return _capacity;
}

int &BMCP::IntList::front()
{
    if (size() > 0) [[likely]]
        return *_begin;
    throw std::out_of_range("no front.");
}

int &BMCP::IntList::back()
{
    if (size() > 0) [[likely]]
        return *(_end - 1);
    throw std::out_of_range("no back.");
}

bool BMCP::IntList::empty()
{
    return size() == 0;
}

BMCP::IntList::IntList(const BMCP::IntList &list)
{
    this->_capacity = list._capacity;
    this->_begin = new int[this->_capacity];
    this->_end = _begin + list.size();
    for (int *it = this->_begin, *it2 = list._begin; it != this->_end; ++it, ++it2)
    {
        *it = *it2;
    }
}

BMCP::IntList::IntList(BMCP::IntList &&list) noexcept
{
    this->_capacity = list._capacity;
    this->_begin = list._begin;
    this->_end = list._end;
    list._begin = nullptr;
    list._end = nullptr;
}

BMCP::IntList &BMCP::IntList::operator=(const BMCP::IntList &list)
{
    if(this != &list)
    {
        this->_capacity = list._capacity;
        this->_begin = new int[this->_capacity];
        this->_end = _begin + list.size();
        for (int *it = this->_begin, *it2 = list._begin; it != this->_end; ++it, ++it2)
        {
            *it = *it2;
        }
    }
    return *this;
}
