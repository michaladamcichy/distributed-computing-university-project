#include "stdafx.h"
#include "enums.h"
#include "Resource.h"
#include "MpiConfig.h"
#include "Communication.h"
#include "Constants.h"
#include "Zlecenie.h"

int main(int argc, char **argv)
{
    srand(time(NULL));

    bool end;

    MpiConfig::init(argc, argv);

    if (MpiConfig::rank == BURMISTRZ_ID)
    {
        // Reply reply;
        // COM::sendToAll(&reply, MESSAGE_REPLY);

        while (!end)
        {
            cout << "WYSYLAM\n";
            Zlecenie *zlecenia = Zlecenie::randomVector();

            //COM::sendToAll(&zlecenia[0], MESSAGE_INIT, zlecenia.size());
            COM::sendZlecenia(zlecenia);
            COM::logSend(-1, zlecenia, MESSAGE_INIT);

            int wypelnioneZleceniaCount = 0;

            while (wypelnioneZleceniaCount < Constants::MAX_ZLECENIA_COUNT)
            {
                COM::receive(MPI_ANY_SOURCE, MESSAGE_COMPLETED);

                wypelnioneZleceniaCount++;
            }
        }
    }
    else
    {
        // COM::receive(MESSAGE_REPLY, BURMISTRZ_ID);
        // Reply reply;
        // COM::logReceive(BURMISTRZ_ID, &reply, MESSAGE_REPLY);
        while (!end)
        {
            Zlecenie *rawZlecenia = COM::receiveZlecenia();
            cout << "*\n";
            //COM::logReceive(BURMISTRZ_ID, rawZlecenia, MESSAGE_INIT);

            vector<Zlecenie> zleceniaMessages((Zlecenie *)rawZlecenia, (Zlecenie *)rawZlecenia + Constants::MAX_ZLECENIA_COUNT);

            Resource zlecenia(RESOURCE_ZLECENIE, Constants::MAX_ZLECENIA_COUNT);
            Resource agrafki(RESOURCE_AGRAFKA, Constants::MAX_AGRAFKI_COUNT);
            Resource trucizny(RESOURCE_TRUCIZNA, Constants::MAX_TRUCIZNY_COUNT);

            //Zlecenie zlecenie = zlecenia.acquire(1);
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