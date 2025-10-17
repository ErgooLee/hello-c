#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t sem; // 定义信号量

void* worker(void* arg) {
    const int id = *(int*)arg;
    printf("Thread %d: Try Entering critical section.\n", id);
    sem_wait(&sem); // P操作（等待）
    printf("Thread %d: Entering critical section.\n", id);
    fflush(stdout); // 强制刷新输出缓冲区
    sleep(1); // 模拟处理

    printf("Thread %d: Leaving critical section.\n", id);
    fflush(stdout); // 强制刷新输出缓冲区
    sem_post(&sem); // V操作（释放）
    return NULL;
}

int main() {
    pthread_t threads[5];
    int ids[5];

    // 初始化信号量 sem_init(sem_t *sem, int pshared, unsigned int value)
    // pshared=0 表示用于同一进程内的线程同步
    // value=2 表示最多允许2个线程同时进入临界区
    // 检查信号量初始化是否成功
    if (sem_init(&sem, 0, 2) == -1) {
        perror("sem_init failed");
        return 1;
    }

    for (int i = 0; i < 5; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, worker, &ids[i]);
    }

    for (int i = 0; i < 5; i++)
        pthread_join(threads[i], NULL);

    sem_destroy(&sem);
    return 0;
}
