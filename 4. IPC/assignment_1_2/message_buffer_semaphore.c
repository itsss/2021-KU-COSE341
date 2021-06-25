#include "message_buffer_semaphore.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>

int shmid;
void *memory_segment=NULL;

int semid;
union semun sem_union;

void init_sem() {
    /*---------------------------------------*/
    /* TODO 1 : init semaphore               */

    if ((semid = semget(SEM_KEY, 1, IPC_CREAT|IPC_EXCL|0666)) == -1) {
        // try as a client
        if ((semid = semget(SEM_KEY, 0, 0)) == -1) return ;
    } else {
        sem_union.val = 1;
        semctl(semid, 0, SETVAL, sem_union);
    }

    /* TODO 1 : END                          */
    /*---------------------------------------*/
    printf("init semid : %d\n", semid);
}

void destroy_sem() {
    /*---------------------------------------*/
    /* TODO 2 : destroy semaphore            */

    if (semctl(semid, 0, IPC_RMID) == -1) return ;
    printf("destroy semid : %d\n", semid);

    /* TODO 2 : END                          */
    /*---------------------------------------*/
}

void s_wait() {
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = -1;
    buf.sem_flg = SEM_UNDO;

    if (semop(semid, &buf, 1) == -1) {
        printf("<s_wait> semop error!\n");
        return ;
    }
}

void s_quit() {
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = 1;
    buf.sem_flg = SEM_UNDO;

    if (semop(semid, &buf, 1) == -1) {
        printf("<s_quit> semop error!\n");
        return ;
    }
}

/*---------------------------------------------*/
/* TODO 3 : use s_quit() and s_wait()          */
/* (1) clone from assignment 1-1 !!!           */
/* (2) use semaphore                           */
/* (3) report explanation (ex. 2020020000.pdf) */

int init_buffer(MessageBuffer **buffer) {
    if ((shmid = shmget(SHM_KEY, sizeof(MessageBuffer), IPC_CREAT|0666)) == -1) {
        printf("shmget error!\n\n");
        return -1;
    }

    if ((memory_segment = shmat(shmid, NULL, 0)) == (void*)-1) {
        printf("shmat error!\n\n");
        return -1;
    }

    (*buffer) = (MessageBuffer*)memory_segment;
    (*buffer)->in = 0;
    (*buffer)->out = 0;

    printf("init buffer\n");
    return 0;
}

int attach_buffer(MessageBuffer **buffer) {
    //do not consider "no buffer situation"
    if ((shmid = shmget(SHM_KEY, sizeof(MessageBuffer), IPC_CREAT|0666)) == -1) {
        printf("shmget error!\n\n");
        return -1;
    }

    if ((memory_segment = shmat(shmid, NULL, 0)) == (void*)-1) {
        printf("shmat error!\n\n");
        return -1;
    }

    *buffer = (MessageBuffer*)memory_segment;

    printf("attach buffer\n");
    printf("\n");
    return 0;
}

int detach_buffer() {
    if (shmdt(memory_segment) == -1) {
        printf("shmdt error!\n\n");
        return -1;
    }

    printf("detach buffer\n\n");
    return 0;
}

int destroy_buffer() {
    if(shmctl(shmid, IPC_RMID, NULL) == -1) {
        printf("shmctl error!\n\n");
        return -1;
    }

    printf("destroy shared_memory\n\n");
    return 0;
}

int produce(MessageBuffer **buffer, int sender_id, char *data) {
    s_wait();
    if (is_full(**buffer)) {
        printf("full!\n\n");
        s_quit();
        return -1;
    }

    if (strlen(data) > 100) {
        printf("len(data) > 100\n\n");
        s_quit();
        return -1;
    }

    (*buffer)->messages[(*buffer)->in].sender_id = sender_id;
    memcpy((*buffer)->messages[(*buffer)->in].data, data, sizeof(char)*strlen(data));
    (*buffer)->in = ((*buffer)->in + 1) % BUFFER_SIZE;

    printf("produce message\n");
    s_quit();
    return 0;
}

int consume(MessageBuffer **buffer, Message **message) {
    s_wait();
    if (is_empty(**buffer)) {
        s_quit();
        return -1;
    }

    *message = &(*buffer)->messages[(*buffer)->out];
    (*buffer)->out = ((*buffer)->out + 1) % BUFFER_SIZE;
    s_quit();
    return 0;
}

int is_empty(MessageBuffer buffer) {
    if (buffer.in == buffer.out) {
        return 1;
    }
    return 0;
}

int is_full(MessageBuffer buffer) {
    if ((buffer.in + 1) % BUFFER_SIZE == buffer.out) {
        return 1;
    }
    return 0;
}

/* TODO 4 : END                                */
/*---------------------------------------------*/