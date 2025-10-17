// mmap_writer.c
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FILEPATH "/tmp/mmap_demo"
#define SIZE 4096

int main() {
    const int fd = open(FILEPATH, O_RDWR | O_CREAT, 0666);
    ftruncate(fd, SIZE); // 设置文件大小

    void* addr = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    strcpy((char*)addr, "Hello from mmap writer!");
    printf("Writer wrote: %s\n", (char*)addr);

    munmap(addr, SIZE);
    return 0;
}
