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

        // requestsMutex.test();
        requestsHandler.init(&requests, MESSAGE_REQUEST + type, &requestsMutex);
        // requestsMutex.test();
        repliesHandler.init(&replies, MESSAGE_REPLY + type, &repliesMutex);
        releasesHandler.init(&releases, MESSAGE_RELEASE + type, &releasesMutex);
    }

    int acquire(int units)
    {
        COM::log("acquiring\n");
        Request request(units, type);
        COM::sendToAll(&request, type + MESSAGE_REQUEST);

        while (replies.size() < MpiConfig::size - 2)
        {
        };
        COM::log("got replies\n");

        int returnValue = -1;

        requestsMutex.lock();
        requests.push_back(request);
        sort(requests.begin(), requests.end());
        requestsHandler.changed();

        int lastRequestsSize = requests.size();

        bool acquired = false;

        while (!acquired)
        {
            int sum = 0;
            int i = 0;
            while ((requests[i].source != MpiConfig::rank))
            {
                sum += requests[i].units;
                i++;
            }

            if (units <= (maxResourceAmount - sum))
            {
                COM::log("acquired resource");
                acquired = true;
                if (type == RESOURCE_ZLECENIE)
                {
                    returnValue = i;
                }
            }
            else
            {
                requestsMutex.unlock();

                while (!requestsHandler.changed())
                {
                }

                requestsMutex.lock();
                sort(requests.begin(), requests.end());
            }
        }

        requestsMutex.unlock();

        replies.clear();

        return returnValue;
    }

    void release(int units)
    {
        Release release(units, type);
        COM::sendToAll(&release, type + MESSAGE_RELEASE);
    }
};
