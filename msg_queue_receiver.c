#include "msg_queue_common.h"

int main() {
    int msgid = msgget(MSG_KEY, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    struct msgbuf msg;
    if (msgrcv(msgid, &msg, MSG_TEXT_SIZE, 1, 0) == -1) {
        perror("msgrcv");
        exit(1);
    }

    printf("Receiver: got message: %s\n", msg.mtext);

    // 用完删除消息队列
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl(IPC_RMID)");
        exit(1);
    }

    printf("Receiver: message queue removed\n");

    getchar();
    return 0;
}
