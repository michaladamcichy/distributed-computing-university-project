#pragma once
#include "stdafx.h"
#include "Constants.h"
#include "Lamport.h"

class Zlecenie
{
public:
    int count;
    int timestamp;

public:
    Zlecenie() {}
    Zlecenie(int count)
    {
        this->count = count;
        this->timestamp = Lamport::getTimestamp();
    }

    static Zlecenie random()
    {
        return Zlecenie(rand() % Constants::MAX_HAMSTERS_IN_ZLECENIE_COUNT + 1);
    }

    static Zlecenie *randomVector()
    {
        Zlecenie *zlecenia = new Zlecenie[Constants::MAX_ZLECENIA_COUNT];
        for (int i = 0; i < Constants::MAX_ZLECENIA_COUNT; i++)
        {
            zlecenia[i] = Zlecenie::random();
        }

        return zlecenia;
    }

private:
};