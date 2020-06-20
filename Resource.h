#pragma once

#include "stdafx.h"
#include "enums.h"
#include "Messages.h"
#include "Lamport.h"
#include "Communication.h"
#include "MessageHandler.h"

class Resource
{
    vector<Request> requests;
    vector<Reply> replies;
    vector<Release> releases;
    ResourceType type;
    int maxResourceAmount;

    MessageHandler<Request> requestsHandler; //(&requests, MESSAGE_REQUEST);
    Mutex requestsMutex;

    MessageHandler<Reply> repliesHandler; //(&replies, MESSAGE_REPLY);
    Mutex repliesMutex;

    MessageHandler<Release>
        releasesHandler; //(&releases, MESSAGE_RELEASE);
    Mutex releasesMutex;

public:
    Resource(ResourceType type, int maxResourceAmount)
    {
        this->type = type;
        this->maxResourceAmount = maxResourceAmount;

        requestsHandler.init(&requests, MESSAGE_REQUEST, &requestsMutex);
        repliesHandler.init(&replies, MESSAGE_REPLY, &repliesMutex);
        releasesHandler.init(&releases, MESSAGE_RELEASE, &releasesMutex);
    }

    int acquire(int units)
    {
        Request request(units, Lamport::getTimestamp(), type);
        COM::sendToAll(&request, type + MESSAGE_REQUEST);

        while (replies.size() < MpiConfig::size - 1)
            ;

        int sum = 0;
        int returnValue = -1;

        requestsMutex.lock();
        requestsHandler.changed();
        int lastRequestsSize = requests.size();
        for (int i = 0; i < requests.size(); i++)
        {
            sum += requests[i].units;
            if ((requests[i].source == MpiConfig::rank) && units <= (maxResourceAmount - sum))
            {
                COM::log("acquired resource");

                if (type == RESOURCE_ZLECENIE)
                {
                    returnValue = i;
                }

                break;
            }
            else
            {
                requestsMutex.unlock();

                while (!requestsHandler.changed())
                {
                }

                requestsMutex.lock();

                i = 0;
            }
        }
        requestsMutex.unlock();

        replies.clear();

        return returnValue;
    }

    void release(int units)
    {
        Release release(MpiConfig::rank, units, Lamport::getTimestamp());
        COM::sendToAll(&release, type + MESSAGE_RELEASE);
    }
};