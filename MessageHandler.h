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

    Mutex changeMutex;

    MessageHandler<Request> *otherMessageHandler = NULL;

    bool changedFlag = false;

    bool end = false;

public:
    MessageHandler() {}

    MessageHandler(vector<T> *messages, int type, Mutex *messageMutex, MessageHandler<Request> *otherMessageHandler = NULL)
    {
        init(messages, type, messagesMutex, otherMessageHandler);
    }

    void init(vector<T> *messages, int type, Mutex *messagesMutex, MessageHandler<Request> *otherMessageHandler = NULL)
    {
        this->type = type;
        this->messages = messages;
        this->messagesMutex = messagesMutex;
        this->otherMessageHandler = otherMessageHandler;

        pthread_t t;

        pthread_create(&t, NULL, MessageHandler::threadFunction, (void *)this);
    }

    // void setAsChanged()
    // {
    //     changedFlag = true;
    // }
    bool setNoChange()
    {
        messagesMutex->lock();
        changedFlag = false;
        messagesMutex->unlock();
    }

    bool changed()
    {
        // messagesMutex->lock();
        // COM::log("messagesMutex->lock()");
        // bool answer = changedFlag;
        // if (changedFlag)
        //     changedFlag = false;
        // messagesMutex->unlock();
        // COM::log("messagesMutex.unlock()");

        // return answer;
        return changedFlag;
    }

    void remove(int sender)
    {
        messagesMutex->lock();

        for (int i = 0; i < messages->size(); i++)
        {
            if ((*messages)[i].source == sender)
            {
                messages->erase(messages->begin() + i);
                break;
            }
        }
        changedFlag = true;

        messagesMutex->unlock();
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

            that->messagesMutex->lock();
            that->changedFlag = true;

            int sender = (*((Message *)&buffer)).source;
            COM::logReceive(sender, &buffer, (MessageType)that->type); //taki mały trik, żeby wyciągnąć pierwsze pole z obiektu :)

            that->messages->push_back(buffer);
            that->messagesMutex->unlock();

            if (Messages::castToMessage(that->type) == MESSAGE_REQUEST)
            {
                Reply reply;
                COM::send(sender, &reply, MESSAGE_REPLY + Messages::castToResource(that->type));
            }
            else if (Messages::castToMessage(that->type) == MESSAGE_RELEASE)
            {
                if (that->otherMessageHandler == 0)
                {
                    cout << "cos tu jest ostro nie halo\n";
                }
                else
                {
                    that->otherMessageHandler->remove(sender);
                }
            }
        }
    }
};
