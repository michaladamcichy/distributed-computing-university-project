#pragma once

enum ResourceType
{
    RESOURCE_ZLECENIE = 100,
    RESOURCE_AGRAFKA = 200,
    RESOURCE_TRUCIZNA = 300
};

enum MessageType
{
    MESSAGE_REQUEST,
    MESSAGE_REPLY,
    MESSAGE_RELEASE,
    MESSAGE_TYPES_COUNT
};

#define BURMISTRZ_ID 0