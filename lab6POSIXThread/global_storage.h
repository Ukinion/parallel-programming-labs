#pragma once

#include <pthread.h>
#include <vector>
#include "mpi.h"

#include "constants.h"

class GlobalStorage {
public:
    ~GlobalStorage();

    int* task_list;
    std::vector<double> iter_time;
    std::vector<double> itertime_proc_pair;
    int* additional_tasks;
    int list_size;
    int cur_task;
    int last_task;
    int size;
    int rank;
    int iter_count;
    double global_res;
    double imbalance;
    double proportion;
    pthread_mutex_t mutex;

    void InitGlobalResources();
    void ComputeStatistics();
private:
    void PrepareMPIMeta();
    void SetupMutex();
    void ScheduleTask();
};

