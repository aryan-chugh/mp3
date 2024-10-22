#ifndef __HEADERS_H
#define __HEADERS_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>


#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define WHITE   "\033[37m"
#define PINK    "\033[35m"
#define YELLOW  "\033[33m"
#define BOLD    "\033[1m"
#define RESET   "\033[0m"

typedef struct request {
    int user_id;
    int file_id;
    char operation[10];
    int request_time;
    bool isProcessed;
    bool isCancelled;
    bool isEnded;

    bool isPrinted;
    pthread_t* working_thread;
} request;

typedef struct file {
    int fid;
    bool is_valid; 
    int users;

    sem_t write;
    sem_t total_users;

    sem_t delete_rd;
    sem_t delete_ru;

    pthread_mutex_t users_lock;
} file; 

typedef struct user_struct {
    request *req;
    file *f;
}user_struct;

#endif