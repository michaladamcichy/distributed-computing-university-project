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
        //COM::sendToAll(&reply, MESSAGE_REPLY);

        // for (int i = 0; i < MpiConfig::size; i++)
        // {
        //     Request request = Request(MpiConfig::rank, 5, RESOURCE_AGRAFKA);
        //     COM::send(i, &request, MESSAGE_REQUEST);
        //     COM::logSend(i, &request, MESSAGE_REQUEST);

        while (!end)
        {
            vector<Zlecenie> zlecenia = Zlecenie::randomVector();
            cout << zlecenia.size() << endl;
            COM::sendToAll(&zlecenia[0], MESSAGE_INIT, zlecenia.size());

            int wypelnioneZleceniaCount = 0;

            while (wypelnioneZleceniaCount < zlecenia.size())
            {
                COM::receive(MPI_ANY_SOURCE, MESSAGE_COMPLETED);

                wypelnioneZleceniaCount++;
            }
        }
    }
    else
    {
        // Reply *reply = (Reply *)COM::receive(MESSAGE_REPLY);

        // reply->print();

        while (!end)
        {
            void *rawZlecenia = COM::receive(MESSAGE_INIT, BURMISTRZ_ID, Constants::MAX_ZLECENIA_COUNT);
            vector<Zlecenie> zleceniaMessages((Zlecenie *)rawZlecenia, (Zlecenie *)rawZlecenia + Constants::MAX_ZLECENIA_COUNT);

            Resource zlecenia(RESOURCE_ZLECENIE, Constants::MAX_ZLECENIA_COUNT);
            Resource agrafki(RESOURCE_AGRAFKA, Constants::MAX_AGRAFKI_COUNT);
            Resource trucizny(RESOURCE_TRUCIZNA, Constants::MAX_TRUCIZNY_COUNT);

            while (true)
                ;
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