#pragma once

#include "stdafx.h"
#include "Messages.h"
#include "MpiConfig.h"
#include "Communication.h"

namespace COM
{
    void send(int target, void *message, int type, int count = 1)
    {
        int result = MPI_Send(
            message,
            Messages::getSize(type) * count,
            MPI_BYTE,
            target,
            type,
            MPI_COMM_WORLD);
        if (result != MPI_SUCCESS)
        {
            cout << "Error sending to " << target << endl;
        }
    }

    void sendZlecenia(Zlecenie *zlecenia)
    {
        for (int i = 0; i < MpiConfig::size; i++)
        {
            int result = MPI_Send(
                zlecenia,
                sizeof(Zlecenie) * Constants::MAX_ZLECENIA_COUNT,
                MPI_BYTE,
                i,
                MESSAGE_INIT,
                MPI_COMM_WORLD);
            if (result != MPI_SUCCESS)
            {
                cout << "Error sending Zlecenia to " << i << endl;
            }
        }
        cout << "Zlecenia sent\n";
    }

    Zlecenie *receiveZlecenia()
    {
        Zlecenie *data = (Zlecenie *)malloc(sizeof(Zlecenie) * Constants::MAX_ZLECENIA_COUNT);

        int result = MPI_Recv(
            data,
            sizeof(Zlecenie) * Constants::MAX_ZLECENIA_COUNT,
            MPI_BYTE,
            BURMISTRZ_ID,
            MESSAGE_INIT,
            MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (result != MPI_SUCCESS)
        {
            cout << "Error receiving from " << BURMISTRZ_ID << endl;
        }
        else
        {
            cout << "Zlecenia received\n";
        }

        return data;
    }

    void sendToAll(void *message, int type, int count = 1)
    {
        for (int i = 0; i < MpiConfig::size; i++)
        {
            send(i, message, type, count);
            cout << " #\n";
        }
    }

    void *receive(int type, int sender = MPI_ANY_SOURCE, int count = 1)
    {
        void *data = malloc(Messages::getSize(type) * count);

        int result = MPI_Recv(
            data,
            Messages::getSize(type) * count,
            MPI_BYTE,
            sender,
            type,
            MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (result != MPI_SUCCESS)
        {
            cout << "Error receiving from " << sender << endl;
        }

        return data;
    }

    void log(string message, MessageType type = MESSAGE_TYPES_COUNT)
    {
        cout << "PROCESS " << MpiConfig::rank << ": " << message << " " << Messages::getName(type) << endl;
    }

    void logSend(int target, const void *message, MessageType type)
    {
        cout << "PROCESS " << MpiConfig::rank << "-> PROCESS " << target << ": ";
        cout << Messages::getName(type) << " ";
        cout << ((Message *)message)
                    ->toString()
             << endl; //alert
    }

    void logReceive(int source, const void *message, MessageType type)
    {
        cout << "PROCESS " << MpiConfig::rank << "<--- PROCESS " << source << ": ";
        cout << Messages::getName(type) << " ";
        cout << ((Message *)message)
                    ->toString()
             << endl; //alert
    }
    // template <class T>
    // void send(int target, T &message)
    // {
    //     MPI_Send(
    //         &message,
    //         sizeof(message),
    //         MPI_BYTE,
    //         target,
    //         message.getType(),
    //         MPI_COMM_WORLD);
    // }

    // template <class T>
    // void sendToAll(int target, T &message)
    // {
    //     for (int i = 0; i < MpiConfig::size; i++)
    //     {
    //         send(i, message);
    //     }
    // }

    // template <T>
    // T receive(int source)
    // {
    //     MPI_Send(
    //         &message,
    //         sizeof(message),
    //         MPI_BYTE,
    //         target,
    //         message.getType(),
    //         MPI_COMM_WORLD);
    // }
}; // namespace COM