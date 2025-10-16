#include "msg_queue_common.h"

int main() {
    const int msgid = msgget(MSG_KEY, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    struct msgbuf msg;
    msg.mtype = 1;  // 消息类型
    snprintf(msg.mtext, MSG_TEXT_SIZE, "Hello from sender PID %d", getpid());

    if (msgsnd(msgid, &msg, strlen(msg.mtext) + 1, 0) == -1) {
        perror("msgsnd");
        exit(1);
    }

    printf("Sender: message sent: %s\n", msg.mtext);

    return 0;
}
