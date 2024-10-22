#include "headers.h"

#define MAX_REQ 100000

void woken_up() {
    printf(BOLD "LAZY has woken up!\n" RESET);
}
void user_request(int uid, char *opid, int fid, int _time_) {
    printf(BOLD YELLOW "User %d has made request for performing %s on file %d at %d seconds.\n" WHITE RESET, uid, opid, fid, _time_);
}
void taken_up(int uid, int _time_){
    printf(BOLD PINK "LAZY has taken up the request of User %d at %d seconds.\n" WHITE RESET, uid, _time_);
}
void decline_request(int uid, int _time_) {
    printf(BOLD WHITE "LAZY has declined the request of User %d at %d seconds because an invalid/deleted file was requested.\n" WHITE RESET, uid, _time_);
}
void request_completed(int uid, int _time_) {
    printf(BOLD GREEN "The request for User %d was completed at %d seconds.\n" WHITE RESET, uid, _time_);
}
void cancel_request(int uid, int _time_) {
    printf(BOLD RED "User %d canceled the request due to no response at %d seconds.\n" WHITE RESET, uid, _time_);
}
void finished_all() {
    printf(BOLD "LAZY has no more pending requests and is going back to sleep!\n" RESET);
}

int r, w, d, n, c, T;
int time_count = 0;
pthread_mutex_t time_mutex;

void *read_file(void *arg) {
    // file structure, request structure
    user_struct *ustr = (user_struct *) arg;
    
    file *f_requested = ustr -> f;
    request *req = ustr -> req;

    sem_wait(&(f_requested->delete_ru));
    sem_post(&(f_requested->delete_ru));
    if(f_requested->is_valid == false) {
        decline_request(req->user_id, time_count);     
        return NULL;
    }

    sem_wait(&(f_requested->total_users));

    if(req->isCancelled == true) {
        sem_post(&(f_requested->total_users));
        return NULL;
    }

    taken_up(req->user_id, time_count);

    pthread_mutex_lock(&(f_requested->users_lock));
    int tmp = ++ f_requested->users;
    pthread_mutex_unlock(&(f_requested->users_lock));
    
    // tmp stores the updated number of users.
    if(tmp == 1) {
        sem_wait(&(f_requested->delete_rd));
    }
    req->isProcessed = true;
    sleep(r);

    pthread_mutex_lock(&(f_requested->users_lock));
    int tmp2 = --(f_requested->users);
    pthread_mutex_unlock(&(f_requested->users_lock));

    if(tmp2 == 0) {
        sem_post(&(f_requested->delete_rd));
    }
    
    sem_post(&(f_requested->total_users));
    
    // request_completed(req->user_id, time_count);
    req->isEnded = true;

}

void *write_file(void *arg) {
    user_struct *ustr = (user_struct *) arg;
    
    file *f_requested = ustr -> f;
    request *req = ustr -> req;

    sem_wait(&(f_requested->delete_ru));
    sem_post(&(f_requested->delete_ru));
    if(f_requested->is_valid == false) {
        // file is deleted ..    
        decline_request(req->user_id, time_count);      
        return NULL;
    }

    sem_wait(&(f_requested->write));
    sem_wait(&(f_requested->total_users));

    if(req->isCancelled == true) {
        sem_post(&(f_requested->write));
        sem_post(&(f_requested->total_users));
        return NULL;
    }

    taken_up(req->user_id, time_count);

    pthread_mutex_lock(&(f_requested->users_lock));
    int tmp = ++ (f_requested->users);
    pthread_mutex_unlock(&(f_requested->users_lock));
    if(tmp == 1) {
        sem_wait(&(f_requested->delete_rd));
    }
    req->isProcessed=true;
    sleep(w);

    pthread_mutex_lock(&(f_requested->users_lock));
    int tmp2 = --(f_requested->users);
    pthread_mutex_unlock(&(f_requested->users_lock));

    if(tmp2 == 0) {
        sem_post(&(f_requested->delete_rd));
    }
    sem_post(&(f_requested->write));
    sem_post(&(f_requested->total_users));

    // request_completed(req->user_id, time_count);
    req->isEnded = true;
}

void *delete_file(void *arg) {
    user_struct *ustr = (user_struct *) arg;
    
    file *f_requested = ustr -> f;
    request *req = ustr -> req;

    sem_wait(&(f_requested->delete_rd));
    sem_wait(&(f_requested->delete_ru));

    if(f_requested->is_valid == false) {
        // file is deleted ..      
        sem_post(&(f_requested->delete_rd));
        sem_post(&(f_requested->delete_ru));

        decline_request(req->user_id, time_count);      
        return NULL;
    }

    if(req->isCancelled == true) {
        sem_post(&(f_requested->delete_rd));
        sem_post(&(f_requested->delete_ru));
        return NULL;
    }

    taken_up(req->user_id, time_count);

    req->isProcessed=true; 
    sleep(d);
    f_requested->is_valid = false;

    sem_post(&(f_requested->delete_ru));
    sem_post(&f_requested->delete_rd);

    // request_completed(req->user_id, time_count);
    req->isEnded=true;
}

int main(int argc, char *argv[]) {
    scanf("%d %d %d", &r, &w, &d);
    scanf("%d %d %d", &n, &c, &T);
    
    request *req[MAX_REQ];
    file *files[n + 1];
    for(int t = 1; t <= n; ++ t) { 
        files[t] = (file *) malloc(sizeof(file));

        files[t] -> fid = t;
        files[t] -> is_valid = true;
        sem_init(&(files[t] -> total_users), 0, c);
        sem_init(&(files[t] -> write), 0, 1);     

        sem_init(&(files[t] -> delete_rd), 0, 1);     
        sem_init(&(files[t] -> delete_ru), 0, 1);    

        pthread_mutex_init(&(files[t]->users_lock), NULL);
    }

    char input[1024];
    int curr = 0;

    getchar();

    while(fgets(input, sizeof(input), stdin) && strcmp(input, "STOP\n") != 0){
        req[curr] = (request *) malloc(sizeof(request));

        sscanf(input, "%d%d%s%d", &(req[curr]->user_id), &(req[curr]->file_id), (req[curr]->operation), &(req[curr]->request_time));
        req[curr] -> isProcessed = false;
        req[curr] -> isEnded = false;
        req[curr] -> isCancelled = false;
        curr ++;
    }

    // for(int i = 0; i < curr; i++){
    //     printf("%d %d %s %d\n", req[i]->user_id, req[i]->file_id, req[i]->operation, req[i]->request_time);
    // }

    woken_up();

    int num_requests = curr;
    bool is_one = true;
    while (is_one) {
        is_one = false; 
        for(int i = 0; i < num_requests; i ++) {
            if(req[i]-> isEnded == true || req[i]->isCancelled == true) continue;
            is_one = true;

            if(req[i]->request_time == time_count) {
                user_request(req[i]->user_id, req[i]->operation, req[i]->file_id, time_count);
            }else
            if(req[i]->request_time + 1 == time_count) {
                pthread_t p;
                user_struct ustr;
                ustr.f = files[req[i]->file_id];
                ustr.req = req[i];

                if(strcmp(req[i]->operation, "READ") == 0) {
                    pthread_create(&p, NULL, read_file, &ustr); 
                }else if(strcmp(req[i]->operation, "WRITE") == 0) {
                    pthread_create(&p, NULL, write_file, &ustr); 
                }else if(strcmp(req[i]->operation, "DELETE") == 0) {
                    pthread_create(&p, NULL, delete_file, &ustr); 
                }
                // pthread_join(p, NULL);
                req[i] -> working_thread = &p;
            }
            if((time_count - req[i]->request_time) >= T && req[i]->isProcessed == false) {
                req[i]->isCancelled = true;
                cancel_request(req[i]->user_id, time_count);

                // pthread_kill(*(req[i]->working_thread), SIGTERM);
            }
        }

        sleep(1);
        time_count ++;

        for(int i = 0; i < num_requests; i ++) {
            if(req[i]->isEnded == true && req[i]->isPrinted == false) {
                request_completed(req[i]->user_id, time_count);
                req[i]->isPrinted = true;
            }
        }
    }
    // for(int i = 0; i < num_requests; i ++) {
    //     pthread_join(req[i]->working_thread, NULL);
    // }

    finished_all();    
    return 0;
}