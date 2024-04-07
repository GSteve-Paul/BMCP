#ifndef BANDBMCP_INTLIST_HPP
#define BANDBMCP_INTLIST_HPP

namespace BMCP
{
    class IntList
    {
    protected:
        int *_begin;
        int *_end;
        int _capacity;

        void expand();

    public:
        typedef int *iterator;
        typedef const int *const_iterator;

        const_iterator begin() const;

        const_iterator end() const;

        iterator begin();

        iterator end();

        IntList();

        IntList(const int);

        IntList(const IntList&);

        IntList(IntList&&) noexcept;

        IntList& operator=(const IntList&);

        int size() const;

        int capacity();

        int &front();

        int &back();

        void push_back(int);

        void pop_back();

        void clear();

        bool empty();

        ~IntList();

        int &operator[](int);
    };
}

#endif //BANDBMCP_INTLIST_HPP
