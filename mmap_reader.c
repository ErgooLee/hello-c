// mmap_reader.c
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FILEPATH "/tmp/mmap_demo"
#define SIZE 4096

int main() {
    const int fd = open(FILEPATH, O_RDWR, 0666);
    void* addr = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    printf("Reader read: %s\n", (char*)addr);
    munmap(addr, SIZE);
    return 0;
}
