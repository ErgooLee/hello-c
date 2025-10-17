#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define SHM_NAME "/myshm"
#define SIZE 4096

int main() {

    const int fd = shm_open(SHM_NAME, O_RDWR, 0666);
    void* addr = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    printf("Reader: %s\n", (char*)addr);

    // 读取进程同样调用 shm_open + mmap
    // shm_unlink(SHM_NAME); // 删除共享内存对象（可选）
    munmap(addr, SIZE);
    return 0;
}
