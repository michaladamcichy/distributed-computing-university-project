#pragma once

#include "stdafx.h"
#include "enums.h"
#include "Messages.h"
#include "Lamport.h"
#include "Communication.h"

class Resource
{
    vector<Request> requests;
    vector<Reply> replies;
    ResourceType type;
    int maxResourceAmount;

    Resource(ResourceType type, int maxResourceAmount)
    {
        this->type = type;
        this->maxResourceAmount = maxResourceAmount;

        // createRequestsListener(); //updates and sorts requests by timestamp
        // createRepliesListener();  //updates replies
        // createReleaseListener();
    }

    void acquire(int units)
    {
        Request request(units, Lamport::getTimestamp(), type);
        COM::sendToAll(&request, type + MESSAGE_REQUEST);

        while (replies.size() < MpiConfig::size - 1)
            ;

        int sum = 0;
        for (int i = 0; i < requests.size(); i++)
        {
            sum += requests[i].units;
            if ((requests[i].source == MpiConfig::rank) && units <= (maxResourceAmount - sum))
            {
                //Success, I have acquired resource
            }
            else
            {
                //Not enough resources
                //Should wait for release
            }
        }
        replies.clear();
    }

    void release(int units)
    {
        Release release(MpiConfig::rank, units, Lamport::getTimestamp());
        COM::sendToAll(&release, type + MESSAGE_RELEASE);
    }
};