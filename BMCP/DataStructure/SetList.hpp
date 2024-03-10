#ifndef BANDBMCP_SETLIST_HPP
#define BANDBMCP_SETLIST_HPP

#include <IntList.hpp>

namespace BMCP
{
    class SetList : public IntList
    {
    public:
        void insert(int);

        void erase(int);
    };
}

#endif //BANDBMCP_SETLIST_HPP
