#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h> // For O_* constants
#include <pthread.h>
#include <sys/stat.h>
#include <unistd.h>


sem_t *sem = NULL;

void* worker(void* arg) {
    const int id = *(int*)arg;
    printf("Thread %d: Try Entering critical section.\n", id);
    sem_wait(sem); // P操作（等待）
    printf("Thread %d: Entering critical section.\n", id);
    fflush(stdout); // 强制刷新输出缓冲区
    sleep(1); // 模拟处理

    printf("Thread %d: Leaving critical section.\n", id);
    fflush(stdout); // 强制刷新输出缓冲区
    sem_post(sem); // V操作（释放）
    return NULL;
}

int main() {

    pthread_t threads[5];
    int ids[5];

    sem = sem_open("/mysem", O_CREAT, 0644, 1); // 名字为 /mysem，初始值1

    if (sem == SEM_FAILED) {
        perror("sem_open");
        return 1;
    }


    for (int i = 0; i < 5; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, worker, &ids[i]);
    }

    for (int i = 0; i < 5; i++)
        pthread_join(threads[i], NULL);

    sem_close(sem);
    sem_unlink("/mysem"); // 删除信号量
    return 0;
}
