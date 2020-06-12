#pragma once

#include "stdafx.h"
#include "Messages.h"
#include "MpiConfig.h"

namespace COM
{
    void send(int target, void *message, int type)
    {
        MPI_Send(
            message,
            Messages::getSize(type),
            MPI_BYTE,
            target,
            type,
            MPI_COMM_WORLD);
    }

    void sendToAll(void *message, int type)
    {
        for (int i = 0; i < MpiConfig::size; i++)
        {
            send(i, message, type);
        }
    }

    void *receive(int type, int sender = MPI_ANY_SOURCE)
    {
        Request *request = new Request();
        Reply *reply = new Reply(0, 0);
        Release *release = new Release();

        void *data;

        if (type == MESSAGE_REQUEST)
        {
            data = request;
        }
        else if (type == MESSAGE_REPLY)
        {
            data = reply;
        }
        else if (type == MESSAGE_RELEASE)
        {
            data = release;
        }
        else
        {
            data = 0;
        }

        MPI_Recv(
            data,
            Messages::getSize(type),
            MPI_BYTE,
            sender,
            type,
            MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        return data;
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