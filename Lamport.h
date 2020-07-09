#pragma once
#include "stdafx.h"

class Lamport
{
    static int lastValue;
    static Mutex mutex;

public:
    static int getTimestamp()
    {
        mutex.lock();
        int value = ++lastValue;
        mutex.unlock();

        return value;
    }

    static int readTimestamp()
    {
        return lastValue;
    }

    static void update(int incomingTimestamp)
    {
        mutex.lock();
        lastValue = max(lastValue, incomingTimestamp);
        mutex.unlock();
    }
};

int Lamport::lastValue = 0;
Mutex Lamport::mutex;