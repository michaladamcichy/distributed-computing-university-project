#pragma once

#include "stdafx.h"
#include "Messages.h"
#include "MpiConfig.h"
#include "Communication.h"

namespace COM
{
    bool logEnabled = true;

    void log(string message, MessageType type = MESSAGE_TYPES_COUNT)
    {
        if (logEnabled)
            cout << "PROCESS " << MpiConfig::rank << ": " << message << " " << Messages::getName(type) << endl;
    }

    void logSend(int target, const void *message, MessageType type)
    {
        if (logEnabled)
        {
            cout << "PROCESS " << MpiConfig::rank << "-> PROCESS " << target << ": ";
            cout << Messages::getName(type) << " ";
            cout << ((Message *)message)
                        ->toString()
                 << endl; //alert
        }
    }

    void logReceive(int source, const void *message, MessageType type)
    {
        if (logEnabled)
        {
            cout << "PROCESS " << MpiConfig::rank << "<--- PROCESS " << source << ": ";
            cout << Messages::getName(type) << " ";
            cout << "timestamp: " << ((Message *)message)->timestamp << " ";
            cout << ((Message *)message)
                        ->toString()
                 << endl; //alert
        }
    }

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
        else
        {
            logSend(target, message, (MessageType)type);
        }
    }

    void sendToAll(void *message, int type, int count = 1)
    {
        for (int i = 1; i < MpiConfig::size; i++) //ALERT!!!
        {
            if (i != MpiConfig::rank)
                send(i, message, type, count);
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
        else
        {
            Lamport::update(*(((int *)data + sizeof(int)))); //wydobywam timestamp z wiadomosci
            logReceive(sender, data, (MessageType)type);
        }

        return data;
    }
}; // namespace COM