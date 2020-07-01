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
            cout << "TIMESTAMP: " << ((Message *)message)->timestamp << " | PROCESS ID: " << MpiConfig::rank << " --> send to PROCESS ID: " << target << " | ";
            cout << Messages::getName(type) << " " << endl; //alert
        }
    }

    void logReceive(int source, const void *message, MessageType type)
    {
        if (logEnabled)
        {
            cout << "TIMESTAMP: " << ((Message *)message)->timestamp << " | PROCESS ID: " << MpiConfig::rank << " --> reply to PROCESS ID: " << source << " | ";
            cout << Messages::getName(type) << " " << endl; //alert
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