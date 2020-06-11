#include "stdafx.h"
#include "enum.h"

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

        createRequestsListener(); //updates and sorts requests by timestamp
        createRepliesListener();  //updates replies
        createReleaseListener();
    }

    void acquire(int units)
    {
        Request request(units, Lamport::getTimestamp());
        sendToAll(type + REQUEST, request);

        while (replies.size() < size - 1)
            ;

        int sum = 0;
        for (int i = 0; i < requests.size(); i++)
        {
            if (request[i].rank == rank && units <= maxResourceAmount - sum)
            {
                //Success, I have acquired resource
            }
            else
            {
                //Not enough resources
                //Should wait for release
            }

            replies.clear();
        }

        void release(int units)
        {
            ReleaseOrder releaseOrder(units, Lamport::getTimestamp());
            sendToAll(type + RELEASE, releaseOrder);
        }
    }
};