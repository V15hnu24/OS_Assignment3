#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

// This is FIFO P2
char s[50][6];

struct strAndID{
    char string[6];
    int id;
};

int highestID = -1;

void ReadtheStrings(){

    //printf("In ReadtheStrings of P2\n");

    int fd = open("fifo_file", O_RDONLY);
        
    for (int i = 0; i < 5; i++)
    {   
        struct strAndID str;

        char temp_string[5];
        int temp_int;

        if (read(fd, &temp_string, (sizeof(char)*5) ) == -1){
            printf("Failed in reading %s strings in P2\n",s[i]);
            return;
        }

        if (read(fd, &temp_int, sizeof(int)) == -1){
            printf("Failed in reading %s strings ID in P2\n",s[i]);
            return;
        }

        strcpy(str.string,temp_string);
        str.id = temp_int;

        if (str.id > highestID)
        {
            highestID = str.id;
        }

        printf("ID: %d -> String %s\n",str.id,str.string);
    }
    close(fd);
    //printf("Closed readthestrings in P2\n");
}

void sendHighestID(){

    //printf("In sendId of P2\n");

    int fd = open("fifo_file", O_WRONLY);

    if (write(fd, &highestID, sizeof(int))== -1)
    {
        printf("error in writing ID in P2\n");
        return;
    }
    
    close(fd);
}

int main(int argc, char* argv[]){

    while (highestID < 49)
    {
        ReadtheStrings();
        sendHighestID();
    }
    return 0;
}