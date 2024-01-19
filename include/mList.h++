//random list

struct mList
{
private:
    static const int maxm = 1e6 + 5;
public:
    int size;
    int container[maxm];

    void push(int);

    void pop();

    void clear();

    int &operator[](const int &);

    int back();

    int front();

    mList()
    {
        size = 0;
    }
};


