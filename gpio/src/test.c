#include "gpio.h"
#include <stdio.h>

int main(){
    short run = 1;
    char command = '0';
    short i;

    while(run){
        printf("q to quit, r to read, w to write\n");
        command = getchar(); 
        switch(command){
            case 'q':
            case 'Q':
                run = 0;
                break;
            case 'R':
            case 'r':
                printf("Enter pinNr\n");
                if(scanf("%hi", &i) != 1){
                    printf("Invalid number\n");
                }else{
                    initPin(i);
                    setPinMode(i, IN);
                    printf("Read: %d\n", readPin(i));
                    uninitPin(i);
                }
                break;
            case 'w':
            case 'W':
                printf("Enter pinNr\n");
                if(scanf("%hi", &i) != 1){
                    printf("Invalid number");
                }else{
                    initPin(i);
                    setPinMode(i, OUT);
                    writePin(i, 1);
                    writePin(i, 0);
                    uninitPin(i);
                }
                break;
            default:
                printf("Unkown command\n");
        };
        fflush(stdin);
    }
    return 0;
}