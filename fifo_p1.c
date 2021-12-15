#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

// This is fifo P1

struct strAndID{
    char string[6];
    int id;
};

char s[50][6];

void perform(int a){

    //printf("In perform of P1\n");

    int fd = open("fifo_file", O_WRONLY);

    for (int i = a; i < a+5; i++)
    {   
        struct strAndID str;
        strcpy(str.string,s[i]); 
        str.id = i;

        if (write(fd, &str.string, (sizeof(char)*5) ) == -1){
            printf("Failed in writing string %d to fifo\n",i);
            return;
        }

        if (write(fd,&str.id,sizeof(int)) == -1)
        {
            printf("Failed in writing %d ID to fifo\n",i);
            return;
        }
    }

    close(fd);
    //printf("Closed fd in perform of P1\n");
}

int readHighID(){
    
    //printf("In readId of P1\n");

    int i = 0;
    int fd = open("fifo_file", O_RDONLY);

    if (read(fd,&i,sizeof(int))== -1)
    {
        printf("Failed in reading ID in P1\n");
        return 8;
    }

    printf("received Highest id = %d from P2\n",i);
    close(fd);

    return i;
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

int main(int argc, char* argv[]){
    
    // Making fifo 

    if (mkfifo("fifo_file",0666) == -1)
    {   
        // error handled

        if (errno != EEXIST)
        {
            printf("Couldn't create fifo.. Erro from P1\n");
            return 1;
        }
    }

    // Random string generator.
    random_gen();

    int i = 0;
    perform(0);
    i = readHighID();

    int count = 0;
    while (count < 9)
    {
        perform(i+1);
        i = readHighID();
        count++;

        if (i == 49){
            return 0;
        }
    }
    return 0;
}