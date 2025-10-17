#include "sem_common.h"

int main() {
    const int semid = semget(SEM_KEY, 1, 0666);
    if (semid == -1) {
        perror("semget");
        exit(1);
    }

    struct sembuf p = {0, -1, 0};  // P操作
    struct sembuf v = {0, 1, 0};   // V操作

    for (int i = 0; i < 5; i++) {
        semop(semid, &p, 1);
        printf("Reader process reading... %d\n", i);
        fflush(stdout);
        sleep(1);
        semop(semid, &v, 1);
        sleep(1);
    }

    // 删除信号量（最后一个进程可以做）
    semctl(semid, 0, IPC_RMID, 0);
    printf("Reader: semaphore removed\n");

    return 0;
}
