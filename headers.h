#ifndef __HEADERS_H
#define __HEADERS_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <string.h>

typedef struct{
    int user_id;
    int file_id;
    char operation[10];
    int request_time;
} Request;

#endif