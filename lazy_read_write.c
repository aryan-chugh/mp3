#include "headers.h"

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

    return 0;
}