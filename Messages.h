#pragma once

#include "stdafx.h"
#include "enums.h"
#include "MpiConfig.h"

class Request
{
public:
    int source;
    int units;
    ResourceType type;

    Request() {}

    Request(int source, int units, ResourceType type)
    {
        this->source = source;
        this->units = units;
        this->type = type;
    }
};

class Reply
{
public:
    int source;
    int timestamp;

    Reply() {}

    Reply(int source, int timestamp)
    {
        this->source = source;
        this->timestamp = timestamp;
    }

    void print()
    {
        cout << MpiConfig::rank << endl;
        cout << source << endl;
        cout << timestamp << endl
             << endl;
    }
};

class Release
{
public:
    int source;
    int units;
    int timestamp;

    Release() {}

    Release(int source, int units, int timestamp)
    {
        this->source = source;
        this->units = units;
        this->timestamp = timestamp;
    }
};

namespace Messages
{
    unsigned int
    getSize(int type)
    {
        type /= 100;
        type *= 100;

        if (type == MESSAGE_REQUEST)
        {
            return sizeof(Request);
        }
        else if (type == MESSAGE_REPLY)
        {
            return sizeof(Reply);
        }
        else
        {
            return sizeof(Release);
        }
    }
} // namespace Messages
