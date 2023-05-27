#include <cmath>
#include <cstring>

#include "pthread_model.h"

GlobalStorage storage{};

void EmulationConcurrencyWork() {
    storage.InitGlobalResources();
    pthread_t worker;
    pthread_t receiver;
    CreateThread(&worker, WorkerThreadJob);
    CreateThread(&receiver, ReceiverThreadJob);
    DestroyThread(worker);
    DestroyThread(receiver);
}

void CreateThread(pthread_t* new_thread, void*(*thread_routine)(void*)) {
    if (pthread_create(new_thread, nullptr, thread_routine, nullptr)) {
        std::cerr << std::strerror(errno);
        std::exit(constants::logic::THREAD_CREATE_FAILED);
    }
}

void DestroyThread(pthread_t this_thread) {
    if (pthread_join(this_thread, nullptr)) {
        std::cerr << std::strerror(errno);
        std::exit (constants::logic::THREAD_DESTROY_FAILED);
    }
}

void* WorkerThreadJob(void* t) {
    int task_on_iter;
    double local_res = 0;
    double start_time, end_time;
    while (storage.iter_count < constants::task::ITERATION_LIMIT) {
        start_time = MPI_Wtime();
        pthread_mutex_lock(&storage.mutex);
        ScheduleNewTasks();
        storage.cur_task = 0;
        storage.last_task = storage.list_size - 1;
        task_on_iter = 0;
        pthread_mutex_unlock(&storage.mutex);
        while (true) {
            pthread_mutex_lock(&storage.mutex);
            if (storage.cur_task > storage.last_task)  {
                pthread_mutex_unlock(&storage.mutex);
                break;
            } else pthread_mutex_unlock(&storage.mutex);
            DoWork(local_res, task_on_iter);
            RequestNewTasks();
        }
        end_time = MPI_Wtime();
        MPI_Allreduce(&local_res, &storage.global_res, 1,
                      MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        ProcessOutput(task_on_iter, end_time - start_time);
        storage.iter_count++;
    }
    NotifyReceiver();
    return t;
}

void ScheduleNewTasks() {
    for (auto i = 0; i < storage.list_size; ++i) {
        storage.task_list[i] = constants::task::WEIGHT_MULTIPLIER * std::abs(50 - i % 100)
                * std::abs(storage.rank - (storage.iter_count + 1) % (storage.size + 1));
    }
}

void DoWork(double& local_res, int& task_on_iter) {
    while(true) {
        pthread_mutex_lock(&storage.mutex);
        if (storage.cur_task > storage.last_task)  {
            pthread_mutex_unlock(&storage.mutex);
            break;
        } else pthread_mutex_unlock(&storage.mutex);
        for (auto i = 0; i < storage.task_list[storage.cur_task]; ++i) {
            local_res += std::sin(i);
        }
        pthread_mutex_lock(&storage.mutex);
        storage.cur_task++;
        pthread_mutex_unlock(&storage.mutex);
        task_on_iter++;
    }
}

void RequestNewTasks() {
    int message = constants::task::REQUEST_NEW_TASKS;
    for (auto i = 0; i < storage.size; ++i) {
        if (i != storage.rank) {
            MPI_Send(&message, 1, MPI_INT, i,
                     constants::thread::TO_RECEIVER_THREAD, MPI_COMM_WORLD);
        }
    }
    for (auto i = 0; i < storage.size; ++i) {
        if (i != storage.rank) {
            MPI_Recv(storage.additional_tasks + i, 1, MPI_INT, i,
                     constants::thread::TO_WORKER_THREAD, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }
    AddNewTasks();
}

void AddNewTasks() {
    int task_counter = 0;
    pthread_mutex_lock(&storage.mutex);
    for (auto i = 0; i < storage.size; ++i) {
        if (storage.additional_tasks[i] != constants::task::NO_TASKS) {
            storage.task_list[task_counter] = storage.additional_tasks[i];
            task_counter++;
        }
    }
    ResetTaskList(task_counter);
    pthread_mutex_unlock(&storage.mutex);
}

void ResetTaskList(int task_counter) {
    if (task_counter) {
        storage.cur_task = 0;
        storage.last_task = task_counter - 1;
    }
}

void ProcessOutput(int task_on_iter, double work_time) {
    MPI_Gather(&work_time, 1, MPI_DOUBLE,
               &storage.iter_time[0], 1,
               MPI_DOUBLE, constants::process::PRINTER_PROCESS, MPI_COMM_WORLD);
    if (storage.rank == constants::process::PRINTER_PROCESS) {
        storage.ComputeStatistics();
        std::cout << "\t*****ITER_INFO*****" << std::endl;
        std::cout << "iteration: " << storage.iter_count + 1 << std::endl;
        std::cout << "imbalance: " << storage.imbalance << std::endl;
        std::cout << "proportion of imbalance: " << storage.proportion << std::endl;
        std::cout << "------------------------------------------------------" << std::endl << std::endl;
    }
    for (auto i = 0; i < storage.size; ++i) {
        MPI_Barrier(MPI_COMM_WORLD);
        if (i == storage.rank) {
            std::cout << "\t***PROCESS №" << storage.rank+1 << "***" << std::endl;
            std::cout << "done task per iteration: " << task_on_iter << std::endl;
            std::cout << "global result: " << storage.global_res << std::endl;
            std::cout << "spent work time: " << work_time << " sec" << std::endl << std::endl;
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if (storage.rank == constants::process::PRINTER_PROCESS) {
        std::cout << "------------------------------------------------------" << std::endl << std::endl;
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

void NotifyReceiver() {
    int message = constants::thread::STOP_WORK;
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Send(&message, 1, MPI_INT, storage.rank,
             constants::thread::TO_RECEIVER_THREAD, MPI_COMM_WORLD);
}

void* ReceiverThreadJob(void* t) {
    int message;
    int tasks_to_send;
    MPI_Status status;
    while (true) {
        MPI_Recv(&message, 1, MPI_INT, MPI_ANY_SOURCE,
                 constants::thread::TO_RECEIVER_THREAD, MPI_COMM_WORLD, &status);
        if (message == constants::thread::STOP_WORK) break;
        else if (message != constants::task::REQUEST_NEW_TASKS) {
            std::cerr << "Invalid request\n";
            std::exit(constants::logic::RECEIVER_FAILED);
        }
        pthread_mutex_lock(&storage.mutex);
        if (storage.cur_task > storage.last_task) {
            tasks_to_send = constants::task::NO_TASKS;
        } else {
            tasks_to_send = storage.task_list[storage.last_task];
            storage.last_task--;
        }
        pthread_mutex_unlock(&storage.mutex);
        MPI_Send(&tasks_to_send, 1, MPI_INT, status.MPI_SOURCE,
                 constants::thread::TO_WORKER_THREAD, MPI_COMM_WORLD);
    }
    return t;
}