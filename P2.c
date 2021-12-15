#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

// This is msgQueue part P2

int hId = -1;
char s[50][6];

struct buffer
{
    long mtype;
    char msgtxt[5][6];
    int index[5];
};

struct buf
{
    long mtype;
    int hid;
};

void readStrings(int msgid){
    struct buffer msg;
    if (msgrcv(msgid,&msg,sizeof(msg),1,0) == -1)
    {
        perror("msgrcv\n");
    }

    for (int i = 0; i < 5; i++)
    {
        printf("ID: %d -> String %s\n",msg.index[i],msg.msgtxt[i]);
    }

    hId = msg.index[4];
}

void sendHid(int msgid2){
//    printf("In send of P2\n");
    struct buf index;
    index.hid = hId;
    index.mtype = 1;

    if (msgsnd(msgid2,&index,sizeof(index),0)==-1){
        perror("msgsnd in P2\n");
    }
//    printf(" Highest Id : %d\n", hId);
}

int main(){
    
    int msgid;
    key_t key;

//  MsgQueue P2 <- P1

    if( (key = ftok("P1.c",65)) == -1){
        perror("key in P2\n");
        return -1;
    }

    if ((msgid = msgget(key, 0666 | IPC_CREAT)) ==-1){
        perror("msgid in P2\n");
        return -1;
    }
    
// MsgQueue P2 -> P1

    int msgid2;
    key_t key2;
    
    if( (key2 = ftok("P2.c",65) )== -1){
        perror("key2");
        return -1;
    }

    if ((msgid2 = msgget(key2, 0666 | IPC_CREAT)) ==-1){
        perror("msgid");
        return -1;
    }


    while (hId <= 49)
    {
        readStrings(msgid);
        sendHid(msgid2);
        if (hId == 49)
        {
            break;
        }
        
    }

    
    if (msgctl(msgid,IPC_RMID,NULL) == -1)
    {
        perror("msgctl");
    }

    if (msgctl(msgid2,IPC_RMID,NULL) == -1)
    {
        perror("msgctl");
    }
    return 0;
}