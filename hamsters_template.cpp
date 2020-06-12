enum ResourceType
{
    ZLECENIE = 100,
    AGRAFKA = 200,
    TRUCIZNA = 300
};

enum MessageType
{
    REQUEST,
    REPLY,
    RELEASE
};

class Resource
{
    vector<Request> requests;
    vector<Reply> replies;
    ResourceType type;
    int MAX_RESOURCE_AMOUNT;

    Resource(ResourceType type, int MAX_RESOURCE_AMOUNT)
    {
        this->type = type;
        this->MAX_RESOURCE_AMOUNT = MAX_RESOURCE_AMOUNT;
        createRequestsListener(); //updates and sorts requests by timestamp
        createRepliesListener();  //updates replies
        createReleaseListener();
    }

    void acquire(int units)
    {
        Request request(units, Lamport::getTimestamp());
        sendToAll(type + REQUEST, request);

        while (replies.size() < size - 1)
            ;

        int sum = 0;
        for (int i = 0; i < requests.size(); i++)
        {
            if (request[i].rank == rank && units <= MAX_RESOURCE_AMOUNT - sum)
            {
                //Success, I have acquired resource
            }
            else
            {
                //Not enough resources
                //Should wait for release
            }

            replies.clear();
        }

        void release(int units)
        {
            ReleaseOrder releaseOrder(units, Lamport::getTimestamp());
            sendToAll(type + RELEASE, releaseOrder);
        }
    }
};

int main()
{
    if (rank == BURMISTRZ_ID)
    {
        while (!end)
        {
            vector<Zlecenie> zlecenia = generateZlecenia();
            burmistrz.sendToAll(zlecenia);

            int wypelnioneZleceniaCount = 0;

            while (wypelnioneZleceniaCount < zlecenia.size())
            {
                burmistrz.receive(ZLECENIE_COMPLETED_TAG);

                wypelnioneZleceniaCount++;
            }
        }
    }
    else
    {
        while (!end)
        {
            vector<Zlecenie> zlecenia = receiveZlecenia();

            Resource zlecenia(ZLECENIE, MAX_ZLECENIA_NUMBER);
            Resource agrafki(AGRAFKA, MAX_AGRAFKI_NUMBER);
            Resource trucizny(TRUCIZNA, MAX_TRUCIZNA_COUNT);

            while (...)
            { //Repeat until succeeded
                Zlecenie zlecenie = zlecenia.acquire(1);
            }

            while (...)
            { //Repeat until succeeded
                agrafki.acquire(1);
            }

            while (...)
            {
                trucizny.acquire(zlecenie.truciznyCount);
            }

            trucizny.release(...);
            agrafki.release(...);

            sendTo(BURMISTRZ_ID, ZLECENIE_COMPLETED_TAG);
        }
    }
}