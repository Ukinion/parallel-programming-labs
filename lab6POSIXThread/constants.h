#pragma once

#include <iostream>

namespace constants {
    namespace logic {
        constexpr int MPI_INIT_FAILED = -5;
        constexpr int MUTEX_FAILED = -6;
        constexpr int THREAD_CREATE_FAILED = -7;
        constexpr int THREAD_DESTROY_FAILED = -10;
        constexpr int RECEIVER_FAILED = -8;
    }

    namespace process {
        constexpr int PRINTER_PROCESS = 0;
    }

    namespace task {
        constexpr int TASK_MULTIPLIER = 100;
        constexpr int ITERATION_LIMIT = 60;
        constexpr int WEIGHT_MULTIPLIER = 500;
        constexpr int REQUEST_NEW_TASKS = 0x11;
        constexpr int NO_TASKS = 0;
    }

    namespace thread {
        constexpr int TO_RECEIVER_THREAD = 0x2;
        constexpr int TO_WORKER_THREAD = 0x3;
        constexpr int STOP_WORK = 0x4;
    }

}
