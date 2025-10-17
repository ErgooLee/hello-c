#include "sem_common.h"

int main() {
    const int semid = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget");
        exit(1);
    }

    // 初始化信号量为 1
    union sem_union arg;
    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);

    struct sembuf p = {0, -1, 0};  // P操作（等待资源）
    struct sembuf v = {0, 1, 0};   // V操作（释放资源）

    for (int i = 0; i < 5; i++) {
        semop(semid, &p, 1);  // 进入临界区
        printf("Writer process writing... %d\n", i);
        fflush(stdout);
        sleep(1);
        semop(semid, &v, 1);  // 离开临界区
        sleep(1);
    }

    return 0;
}
