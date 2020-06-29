#pragma once

#include "stdafx.h"
#include "enums.h"

template <class T>
class MessageHandler
{
private:
    int type;
    vector<T> *messages;
    Mutex *messagesMutex;

    bool changedFlag = false;

    bool end = false;

public:
    MessageHandler() {}

    MessageHandler(vector<T> *messages, int type, Mutex *messageMutex)
    {
        init(messages, type, messagesMutex);
    }

    void init(vector<T> *messages, int type, Mutex *messagesMutex)
    {
        this->type = type;
        this->messages = messages;
        this->messagesMutex = messagesMutex;
        //this->messagesMutex->lock();
        //this->messagesMutex->unlock();

        pthread_t t;

        pthread_create(&t, NULL, MessageHandler::threadFunction, (void *)this);
    }

    bool changed()
    {
        bool answer = changedFlag;
        if (changedFlag)
            changedFlag = false;
        return answer;
    }

private:
    static void *threadFunction(void *args)
    {
        MessageHandler *that = (MessageHandler *)args;
        T buffer;
        while (true) //alert powinno byc end
        {
            //COM::log("waits for receive", (MessageType)that->type);
            MPI_Recv(
                &buffer,
                sizeof(T),
                MPI_BYTE,
                MPI_ANY_SOURCE,
                that->type,
                MPI_COMM_WORLD,
                MPI_STATUS_IGNORE);
            that->changedFlag = true;
            int sender = (*((int *)&buffer));
            COM::logReceive(sender, &buffer, (MessageType)that->type); //taki mały trik, żeby wyciągnąć pierwsze pole z obiektu :)

            that->messagesMutex->lock(); //alert
            that->messages->push_back(buffer);
            that->messagesMutex->unlock();

            if (Messages::castToMessage(that->type) == MESSAGE_REQUEST)
            {
                Reply reply;
                COM::send(sender, &reply, MESSAGE_REPLY + Messages::castToResource(that->type));
            }
        }
    }

    void remove(int index)
    {
        messagesMutex->lock();
        messages.erase(messages.begin() + index);
        changedFlag = true;
        messagesMutex->unlock();
    }
};
