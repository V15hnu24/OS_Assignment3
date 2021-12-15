#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

// This is msgQueue part P1

int Hid;
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

void perform(int msgid, int j){
 //   printf("j = %d", j);
    struct buffer msg;
    msg.mtype = 1;   
    for (int i = j; i < j+5; i++)
    {             
        strcpy(msg.msgtxt[i-j],s[i]);
        msg.msgtxt[i-j][5] = '\0';
        msg.index[i-j] = i;
    }        

    if (msgsnd(msgid,&msg,sizeof(msg),0)==-1)
    {
        perror("msgsnd in P1\n");
    }
	for(int i = 0; i < 5; i++){
		printf("Sent: %s\n", msg.msgtxt[i]);
	}
}

void rcvHid(int msgid2){
	struct buf msg;

    if (msgrcv(msgid2,&msg,sizeof(msg),0,0) == -1){
        perror("msgrcv\n");
        return;
    }
    
    Hid = msg.hid;
	printf("Hghest index = %d\n\n", Hid);
}

void random_gen(){

    char alpha[] = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM\n";

    for (int i = 0; i < 50; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            s[i][j]=alpha[rand() % 52];
        }
        //printf("String %d %s\n",i,s[i]);
    }

}

int main(){
    
    // random strings Generated 
    random_gen();
//  MsgQueue1 P1->P2

    int msgid;
    key_t key;
    
    if(( key = ftok("P1.c",65)) == -1){
        perror("key");
    }

    if ((msgid = msgget(key, 0666 | IPC_CREAT))==-1){
        perror("msgid");
    }

    printf("msgid = %d\n",msgid);

//  MsgQueue P1 <- P2

    int msgid2;
    key_t key2;
    
    if( (key2 = ftok("P2.c",65)) == -1){
        perror("key");
        return -1;
    }

    if ((msgid2 = msgget(key2, 0666 | IPC_CREAT))==-1){
        perror("msgid");
        return -1;
    }

    printf("msgid2 = %d\n",msgid2);


    int i = 0;
    perform(msgid,0);
    rcvHid(msgid2);

//  i = Hid;

    int count = 0;
    while (count <9)
    {
        perform(msgid,Hid+1);
        if (Hid <=44)
        {
            rcvHid(msgid2);
        }
        if (Hid > 45)
        {
            return 0;
        }

 // i = Hid;
        count++;
    }
    return 0;
}