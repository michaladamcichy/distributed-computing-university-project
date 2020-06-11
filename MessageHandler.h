#include "stdafx.h"
#include "enums.h"

template <class T>
class MessageHandler
{
private:
    MessageType type;
    vector<T> *messages;
    mutex messagesMutex;

    bool end = false;

public:
    MessageHandler(vector<T> *messages, int type)
    {
        this->type = type;
        this->messages = messages;
    }

private:
    void threadFunction()
    {
        T buffer;
        while (!end)
        {
            MPI_Recv(
                &buffer,
                sizeof(buffer),
                MPI_BYTE,
                MPI_ANY_SOURCE,
                type,
                MPI_COMM_WORLD,
                MPI_STATUS_IGNORE); //alert z tym tpyem

            messagesMutex.lock();
            messages.push_back(buffer);
            messagesMutex.unlock();
        }
    }

    void remove(int index)
    {
        messagesMutex.lock();
        messages.erase(messages.begin() + index);
        messagesMutex.unlock();
    }
}