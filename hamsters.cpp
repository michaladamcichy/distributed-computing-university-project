#include "stdafx.h"
#include "enums.h"
#include "Resource.h"
#include "MpiConfig.h"
#include "Communication.h"
#include "Constants.h"
#include "Messages.h"
#include "unistd.h"

int main(int argc, char **argv)
{
    srand(time(NULL));
    bool end = false;

    MpiConfig::init(argc, argv);

    if (MpiConfig::rank == BURMISTRZ_ID)
    {
        while (!end)
        {
            Zlecenie *zlecenia = Zlecenie::randomVector();

            COM::sendToAll(zlecenia, MESSAGE_INIT, Constants::MAX_ZLECENIA_COUNT);
            //cout << Lamport::readTimestamp() << endl;
            int wypelnioneZleceniaCount = 0;

            while (wypelnioneZleceniaCount < Constants::MAX_ZLECENIA_COUNT)
            {
                COM::receive(MESSAGE_COMPLETED, MPI_ANY_SOURCE);

                COM::log("TASK JUST CONFIRMED COMPLETED");
                wypelnioneZleceniaCount++;
                //cout <<"Remaining: " << Constants::MAX_ZLECENIA_COUNT - wypelnioneZleceniaCount << endl;
            }
            COM::log("### All tasks completed! ###");
			
			end = true;
        }
    }
    else
    {
        while (!end)
        {
            Zlecenie *rawZlecenia = (Zlecenie *)COM::receive(MESSAGE_INIT, BURMISTRZ_ID, Constants::MAX_ZLECENIA_COUNT);

            vector<Zlecenie> zleceniaMessages(rawZlecenia, rawZlecenia + Constants::MAX_ZLECENIA_COUNT);

            Resource zlecenia(RESOURCE_ZLECENIE, Constants::MAX_ZLECENIA_COUNT);
            Resource agrafki(RESOURCE_AGRAFKA, Constants::MAX_AGRAFKI_COUNT);
            Resource trucizny(RESOURCE_TRUCIZNA, Constants::MAX_TRUCIZNY_COUNT);

            Zlecenie zlecenie = zleceniaMessages[zlecenia.acquire(1)];
            COM::log(">>> zlecenie acquired (" + to_string(zlecenie.count) + " hamsters)");

            agrafki.acquire(1);
            COM::log(">>> agrafka acquired");

            trucizny.acquire(zlecenie.count);
            COM::log(">>> trucizna acquired (" + to_string(zlecenie.count) + " units)");

            trucizny.release(zlecenie.count);

            agrafki.release(1);
            COM::log(">>> agrafka released");


            COM::log(">>> zlecenie completed");
            Completed message;


            COM::send(BURMISTRZ_ID, &message, MESSAGE_COMPLETED);
        }

        //while (true) //alert
        //    ;
    }

    MpiConfig::cleanUp();
}