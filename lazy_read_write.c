#include "headers.h"


#define MAX_REQ 100000

int read_time;
int write_time;
int delete_time;
int num_of_files;
int max_conc;
int max_req_wat;

Request requests[MAX_REQ];

void woken_up() {
    printf(BOLD "LAZY has woken up!\n" RESET);
}
void user_request(int uid, int opid, int fid, int _time_) {
    printf(BOLD YELLOW "User %d has made request for performing %d on file %d at %d seconds.\n" WHITE RESET, uid, opid, fid, _time_);
}
void taken_up(int uid, int _time_){
    printf(BOLD PINK "LAZY has taken up the request of User %d at %d seconds.\n" WHITE RESET, uid, _time_);
}
void decline_request(int uid, int _time_) {
    printf(BOLD WHITE "LAZY has declined the request of User %d at %d seconds because an invalid/deleted file was requested." WHITE RESET, uid, _time_);
}
void request_completed(int uid, int _time_) {
    printf(BOLD GREEN "The request for User %d was completed at %d seconds." WHITE RESET, uid, _time_);
}
void cancel_request(int uid, int _time_) {
    printf(BOLD RED "User %d canceled the request due to no response at %d seconds." WHITE RESET, uid, _time_);
}
void finished_all() {
    printf(BOLD "LAZY has no more pending requests and is going back to sleep!" RESET);
}


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