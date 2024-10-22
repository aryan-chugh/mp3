#include "headers.h"

#define MAX_REQ 100000

int read_time;
int write_time;
int delete_time;
int num_of_files;
int max_conc;
int max_req_wat;

Request requests[MAX_REQ];

int main(int argc, char *argv[]) {
    scanf("%d%d%d", &read_time, &write_time, &delete_time);
    scanf("%d%d%d", &num_of_files, &max_conc, &max_req_wat);
   
    char input[1024];
    int curr = 0;

    getchar();

    while(fgets(input, sizeof(input), stdin) && strcmp(input, "STOP\n") != 0){
        sscanf(input, "%d%d%s%d", &requests[curr].user_id, &requests[curr].file_id, requests[curr].operation, &requests[curr].request_time);
        curr++;
    }

    for(int i = 0; i < curr; i++){
        printf("%d %d %s %d\n", requests[i].user_id, requests[i].file_id, requests[i].operation, requests[i].request_time);
    }

    return 0;
}