#pragma once

#include "stdafx.h"
#include "enums.h"
#include "MpiConfig.h"
#include "Zlecenie.h"

class Message
{
public:
    Message()
    {
    }

    string toString()
    {
        return "";
    }
};

class Request // : public Message
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

    string toString()
    {
        return "";
    }
};

class Reply // : public Message
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

    string toString()
    {
        return "";
    }
};

class Release // : public Message
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

    string toString()
    {
        return "";
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
        else if (type == MESSAGE_RELEASE)
        {
            return sizeof(Release);
        }
        else if (type == MESSAGE_INIT)
        {
            return sizeof(Zlecenie);
        }
        else if (type == MESSAGE_STOP)
        {
            return 0;
        }
        else if (type == MESSAGE_COMPLETED)
        {
            return 0;
        }

        return 0;
    }

    string
    getName(int type)
    {
        if (type == MESSAGE_REQUEST)
        {
            return "REQUEST";
        }
        else if (type == MESSAGE_REPLY)
        {
            return "REPLY";
        }
        else if (type == MESSAGE_RELEASE)
        {
            return "RELEASE";
        }
        else if (type == MESSAGE_INIT)
        {
            return "INIT";
        }
        else if (type == MESSAGE_COMPLETED)
        {
            return "COMPLETED";
        }
        else if (type == MESSAGE_STOP)
        {
            return "STOP";
        }
        else
            return "UNDEFINED";
    }
} // namespace Messages
