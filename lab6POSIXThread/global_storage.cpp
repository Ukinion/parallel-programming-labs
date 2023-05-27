#include <algorithm>
#include <cstring>

#include "global_storage.h"

void GlobalStorage::InitGlobalResources() {
    PrepareMPIMeta();
    ScheduleTask();
    SetupMutex();
}

void GlobalStorage::PrepareMPIMeta() {
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
}

void GlobalStorage::ScheduleTask() {
    task_list = new int[size*constants::task::TASK_MULTIPLIER];
    additional_tasks = new int[size]{};
    if (rank == constants::process::PRINTER_PROCESS) {
        iter_time = std::vector<double>(size, 1);
        itertime_proc_pair.reserve(size);
    }
    list_size = size*constants::task::TASK_MULTIPLIER;
    last_task = size*constants::task::TASK_MULTIPLIER - 1;
    cur_task = 0;
    global_res = 0;
    iter_count = 0;
    imbalance = 0;
    proportion = 0;
}

void GlobalStorage::SetupMutex() {
    if (pthread_mutex_init(&mutex, nullptr)) {
        std::cerr << std::strerror(errno);
        std::exit(constants::logic::MUTEX_FAILED);
    }
}

GlobalStorage::~GlobalStorage() {
    if (pthread_mutex_destroy(&mutex)) {
        std::cerr << std::strerror(errno);
        std::exit(constants::logic::MUTEX_FAILED);
    }
    delete[] task_list;
    delete[] additional_tasks;
}

void GlobalStorage::ComputeStatistics() {
    if (size == 1) {
        imbalance = iter_time[0];
        proportion = 100;
    } else {
        for (auto i = 0; i < size; ++i) {
            itertime_proc_pair.emplace_back(iter_time[i]);
        }
        std::sort(itertime_proc_pair.begin(), itertime_proc_pair.end());
        imbalance = *itertime_proc_pair.rbegin() - *itertime_proc_pair.begin();
        proportion = imbalance / *itertime_proc_pair.rbegin() * 100;
    }
}