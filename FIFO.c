#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

const char *fifo_path = "/tmp/myfifo";

void* writer_thread(void *arg) {


    const int fd = open(fifo_path, O_WRONLY);
    if (fd == -1) {
        perror("writer open");
        pthread_exit(NULL);
    }

    const char *msg = "Hello from writer thread!\n";
    write(fd, msg, strlen(msg));
    printf("Writer thread: message sent\n");
    close(fd);
    pthread_exit(NULL);
}

void* reader_thread(void *arg) {
    usleep(2*1000*1000);
    const int fd = open(fifo_path, O_RDONLY);
    if (fd == -1) {
        perror("reader open");
        pthread_exit(NULL);
    }

    char buf[128];
    int n = read(fd, buf, sizeof(buf)-1);
    if (n > 0) {
        buf[n] = '\0';
        printf("Reader thread received: %s", buf);
    }

    close(fd);
    pthread_exit(NULL);
}

int main() {
    // 确保 FIFO 已存在
    if (access(fifo_path, F_OK) == -1) {
        if (mkfifo(fifo_path, 0666) == -1) {
            perror("mkfifo");
            return 1;
        }
    }

    pthread_t writer, reader;

    pthread_create(&reader, NULL, reader_thread, NULL);


    pthread_create(&writer, NULL, writer_thread, NULL);

    pthread_join(writer, NULL);
    pthread_join(reader, NULL);

    return 0;
}
