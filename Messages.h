#pragma once

#include "stdafx.h"
#include "enums.h"
#include "MpiConfig.h"
#include "Zlecenie.h"
#include "Lamport.h"

class Message
{
public:
    int source;
    int timestamp;
    Message()
    {
    }

    string toString()
    {
        return "";
    }

    bool operator<(const Message &d)
    {
        if (timestamp < d.timestamp)
        {
            return true;
        }
        else if (timestamp == d.timestamp)
        {
            return source < d.source;
        }
        return false;
    }
};

class Request : public Message
{
public:
    int units;
    ResourceType type;

    Request() {}

    Request(int units, ResourceType type)
    {
        this->source = MpiConfig::rank;
        this->units = units;
        this->type = type;
        this->timestamp = Lamport::getTimestamp();
		
		string typeString = "";
		switch(type) {
			case 100: { typeString = "zlecenie"; break; }
			case 200: { typeString = "agrafka"; break; }
			case 300: { typeString = "trucizna"; break; }
		}
		
		cout << "REQUEST " << typeString << ": id(" << this->source << ") timestamp(" << this->timestamp << ") units(" << this->units << ")\n"; 
    }

    string toString()
    {
        return "";
    }
};

class Reply : public Message
{
public:
    Reply()
    {
        this->source = MpiConfig::rank;
        this->timestamp = Lamport::getTimestamp();
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

class Release : public Message
{
public:
    int units;
    int type;

    Release() {}

    Release(int units, ResourceType type)
    {
        this->source = MpiConfig::rank;
        this->units = units;
        this->timestamp = Lamport::getTimestamp();
        this->type = type;
    }

    string toString()
    {
        return "";
    }
};

namespace Messages
{
    int castToMessage(int type)
    {
        while (type - 100 >= 0)
        {
            type -= 100;
        }

        return type;
    }

    int castToResource(int type)
    {
        type /= 100;
        type *= 100;
        return type;
    }

    unsigned int
    getSize(int type)
    {
        type = castToMessage(type);

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
        type = castToMessage(type);

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