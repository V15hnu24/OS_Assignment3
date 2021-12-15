#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#define SOCK_PATH "echo_socket"

// This is socket P2
int main(void)
{
    
    int soc;

    struct sockaddr_un rm;
    char str[100];

    if ((soc = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }
    
    printf("Trying to connect...\n");
    rm.sun_family = AF_UNIX;
    strcpy(rm.sun_path, SOCK_PATH);

    int len;
    len = strlen(rm.sun_path) + sizeof(rm.sun_family);
    if (connect(soc, (struct sockaddr *)&rm, len) == -1) {
        perror("connect");
        exit(1);
    }
    
    
    int rcvid;
    int hid = 0;
    int sndid;

    while (hid < 49)
    {
        
        char str[5][6];
        int id[5];

        if ((rcvid = recv(soc,str,sizeof(str),0) )== -1)
        {
            printf("rcvid\n");
        }
        

        if ((rcvid=recv(soc, &id, sizeof(id), 0) )==-1 )
        {
            perror("rcvid\n");
        }

        hid = id[4];
        
        for (int i = 0; i < 5; i++)
        {
            printf("ID: %d, string %s\n",id[i],str[i]);
        }

        if ((sndid=send(soc, &hid, sizeof(int), 0)) < 0) {
            perror("sndid");
        }
        
    }
    close(soc);
    return 0;
}