#pragma once
#include "stdafx.h"
#include "Constants.h"

class Zlecenie
{
private:
    int count;

public:
    Zlecenie() {}
    Zlecenie(int count)
    {
        this->count = count;
    }

    static Zlecenie random()
    {
        return Zlecenie(rand() % Constants::MAX_HAMSTERS_IN_ZLECENIE_COUNT);
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