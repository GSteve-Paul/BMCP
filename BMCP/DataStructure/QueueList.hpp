#ifndef BANDBMCP_QUEUELIST_HPP
#define BANDBMCP_QUEUELIST_HPP

#include <IntList.hpp>

namespace BMCP
{
    class QueueList : public IntList
    {
    public:
        int head;

        QueueList(int);

        void push_and_pop(int);
    };
}

#endif //BANDBMCP_QUEUELIST_HPP
