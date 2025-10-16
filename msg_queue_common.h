// msg_common.h
#ifndef MSG_COMMON_H
#define MSG_COMMON_H

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MSG_KEY 1234       // 消息队列的 key，可自定义
#define MSG_TEXT_SIZE 100  // 消息体大小

struct msgbuf {
    long mtype;
    char mtext[MSG_TEXT_SIZE];
};

#endif
