#ifndef SEM_COMMON_H
#define SEM_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>

#define SEM_KEY 1234  // 信号量 key

// semctl 第四个参数的 union 定义（旧接口）
union sem_union {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

#endif
