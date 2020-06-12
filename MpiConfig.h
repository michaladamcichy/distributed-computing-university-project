#pragma once

#include "stdafx.h"

namespace MpiConfig
{
    int rank;
    int size;

    void init(int argc, char **argv)
    {
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    }

    void cleanUp()
    {
        MPI_Finalize();
    }

}; // namespace MpiConfig