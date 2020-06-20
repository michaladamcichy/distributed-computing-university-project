#pragma once
#include "stdafx.h"
#include "Constants.h"

class Zlecenie
{
private:
    int count;

public:
    Zlecenie(int count)
    {
        this->count = count;
    }

    static Zlecenie random()
    {
        return Zlecenie(rand() % Constants::MAX_HAMSTERS_IN_ZLECENIE_COUNT);
    }

    static vector<Zlecenie> randomVector()
    {
        vector<Zlecenie> zlecenia;
        for (int i = 0; i < Constants::MAX_ZLECENIA_COUNT; i++)
        {
            zlecenia.push_back(Zlecenie::random());
        }

        return zlecenia;
    }

private:
};