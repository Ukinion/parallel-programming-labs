#pragma once

#include <pthread.h>
#include "global_storage.h"

void CreateThread(pthread_t*, void*(*)(void*));
void DestroyThread(pthread_t);
void EmulationConcurrencyWork();
void* WorkerThreadJob(void*);
void ScheduleNewTasks();
void DoWork(double&, int&);
void RequestNewTasks();
void AddNewTasks();
void ResetTaskList(int);
void ProcessOutput(int, double);
void NotifyReceiver();
void* ReceiverThreadJob(void*);