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

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define WHITE   "\033[37m"
#define PINK    "\033[35m"
#define YELLOW  "\033[33m"
#define BOLD    "\033[1m"
#define RESET   "\033[0m"



#endif