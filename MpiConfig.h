#pragma once

#include "stdafx.h"

namespace MpiConfig
{
    int rank;
    int size;

    void checkThreadSupport(int provided)
    {
        if (provided != MPI_THREAD_MULTIPLE)
        {
            cout << "Threads NOT supported!!!\n";
        }
    }

    void init(int argc, char **argv)
    {
        int provided;
        MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
        checkThreadSupport(provided);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    }

    void cleanUp()
    {
        MPI_Finalize();
    }

}; // namespace MpiConfig