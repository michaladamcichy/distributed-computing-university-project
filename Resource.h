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
        releasesHandler.init(&releases, MESSAGE_RELEASE + type, &releasesMutex, &requestsHandler);
    }

    int acquire(int units)
    {
        Request request(units, type);
        COM::sendToAll(&request, type + MESSAGE_REQUEST);

        while (replies.size() < MpiConfig::size - 2)
        {
        };
        COM::log(">>> got replies\n");

        int returnValue = -1;

        requestsMutex.lock();
        //COM::log("requestsMutex.lock()");
        requests.push_back(request);
        sort(requests.begin(), requests.end());
        requestsHandler.changed();

        int lastRequestsSize = requests.size();

        bool acquired = false;

        while (!acquired)
        {
            // COM::log(">>> acquiring loop iteration ...");
            int sum = 0;
            int i = 0;
            while ((requests[i].source != MpiConfig::rank))
            {
                sum += requests[i].units;
                i++;
            }
            //COM::log(">>> In a moment I will know if there's enough for me");

            if (units <= (maxResourceAmount - sum))
            {
                //COM::log(">>> acquired resource");
                acquired = true;
                if (type == RESOURCE_ZLECENIE)
                {
                    returnValue = i;
                }
            }
            else
            {
                COM::log(">>> Not enoough. Retrying\n");
                requestsMutex.unlock();
                //COM::log("requestsMutex.unlock()");

                //COM::log(">>> waiting for change");
                while (!requestsHandler.changed())
                {
                    //COM::log(">>> Noting has changed");
                }
                requestsHandler.setNoChange();
                //COM::log(">>> requests change detected");

                requestsMutex.lock();
                //COM::log("requestsMutex.lock()");
                sort(requests.begin(), requests.end
				Lamport::update(requests[0].timestamp);
                //COM::log("sorted! :)");
            }
        }
        COM::log("ACQUIRED " + Messages::resourceToName(type));

        requestsMutex.unlock();
        //COM::log("requestsMutex.unlock()");

        replies.clear();

        return returnValue;
    }

    void release(int units)
    {
        Release release(units, type);
        COM::sendToAll(&release, type + MESSAGE_RELEASE);
		COM::log("RELEASED " + Messages::resourceToName(type));
        requestsHandler.remove(MpiConfig::rank);
    }
};
