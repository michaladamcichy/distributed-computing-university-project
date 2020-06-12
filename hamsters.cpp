#include "stdafx.h"
#include "enums.h"
#include "Resource.h"
#include "MpiConfig.h"
#include "Communication.h"

int main(int argc, char **argv)
{
    bool end;

    MpiConfig::init(argc, argv);

    if (MpiConfig::rank == BURMISTRZ_ID)
    {
        Reply reply = {10, 15};
        reply.print();

        COM::sendToAll(&reply, MESSAGE_REPLY);

        while (!end)
        {
            // vector<Zlecenie> zlecenia = generateZlecenia();
            // burmistrz.sendToAll(zlecenia);

            // int wypelnioneZleceniaCount = 0;

            // while (wypelnioneZleceniaCount < zlecenia.size())
            // {
            //     burmistrz.receive(ZLECENIE_COMPLETED_TAG);

            //     wypelnioneZleceniaCount++;
            // }
        }
    }
    else
    {
        Reply *reply = (Reply *)COM::receive(MESSAGE_REPLY);

        reply->print();

        while (!end)
        {
            // vector<Zlecenie> zlecenia = receiveZlecenia();

            // Resource zlecenia(ZLECENIE, MAX_ZLECENIA_NUMBER);
            // Resource agrafki(AGRAFKA, MAX_AGRAFKI_NUMBER);
            // Resource trucizny(TRUCIZNA, MAX_TRUCIZNA_COUNT);

            // while (...)
            // { //Repeat until succeeded
            //     Zlecenie zlecenie = zlecenia.acquire(1);
            // }

            // while (...)
            // { //Repeat until succeeded
            //     agrafki.acquire(1);
            // }

            // while (...)
            // {
            //     trucizny.acquire(zlecenie.truciznyCount);
            // }

            // trucizny.release(...);
            // agrafki.release(...);

            // sendTo(BURMISTRZ_ID, ZLECENIE_COMPLETED_TAG);
        }
    }

    MpiConfig::cleanUp();
}