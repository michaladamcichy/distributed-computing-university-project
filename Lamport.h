#pragma once
#include "stdafx.h"

class Lamport
{
    static int lastValue;

public:
    static int getTimestamp()
    {
        return ++lastValue;
    }
};

int Lamport::lastValue = 0;