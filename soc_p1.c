#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>  
#include <unistd.h>
#define SOCK_PATH "echo_socket"

// This is socket P1
int main(void)
{
    char s[50][6];
    int soc;
    struct sockaddr_un s1, s2;
    char str[100];

    if ((soc = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    s1.sun_family = AF_UNIX;
    strcpy(s1.sun_path, SOCK_PATH);
    unlink(s1.sun_path);

    int len;

    len = strlen(s1.sun_path) + sizeof(s1.sun_family);
    
    if (bind(soc, (struct sockaddr *)&s1, len) == -1) {
        perror("bind");
        exit(1);
    }
    
    if (listen(soc, 5) == -1) {
        perror("listen");
        exit(1);
    }

    int t;

    printf("Waiting for a connection...\n");
    t = sizeof(s2);

    int acpt;
    if ((acpt = accept(soc, (struct sockaddr *)&s2, &t)) == -1) {
        perror("accept");
        exit(1);
    }
    
    printf("Connected.\n");
    
    int rcvid;

    char alpha[] = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM\n";

    for (int i = 0; i < 50; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            s[i][j]=alpha[rand() % 52];
        }
        printf("String %d %s\n",i,s[i]);
    }

    int hid = 0;
    while(hid < 49) {

        char temp[5][6];
        int index[5];

        for (int i = hid; i < hid+5; i++)
        {
            s[i][5] = '\0';
            strcpy(temp[i-hid],s[i]);
            temp[i][5] = '\0';
            index[i-hid] = i;       
        }
        
        if ((rcvid = send(acpt,temp,sizeof(temp),0)) == -1)
        {
            perror("send me error");
        }
        for (int j = 0; j < 5; j++)
        {
            printf("Sent %s\n",temp[j]);
        }
        
        if ((rcvid = send (acpt,index,sizeof(index),0)) == -1)
        {
            perror("error");
        }

        if ((rcvid = recv(acpt,&hid,sizeof(int),0) ) == -1)
        {
            perror("rcvid\n");
        }
        hid = hid +1;
        printf("recvd %d\n",hid);
    }
    close(acpt);
    return 0;
}