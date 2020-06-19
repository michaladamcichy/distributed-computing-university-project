#pragma once
#include "stdafx.h"

class Mutex
{
private:
    pthread_mutex_t mutex;

public:
    Mutex()
    {
        if (pthread_mutex_init(&mutex, NULL) != 0)
        {
            cout << "Failed to create mutex\n";
        }
    }

    ~Mutex()
    {
        pthread_mutex_destroy(&mutex);
    }

    void lock()
    {
        pthread_mutex_lock(&mutex);
    }

    void unlock()
    {
        pthread_mutex_unlock(&mutex);
    }

private:
};