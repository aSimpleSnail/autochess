

#include "gpio.h"
#include <stdio.h>
#include <string.h>

const short valid_pins[] = { 0, 1, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 21, 22, 23, 24, 25 };
#define SYSF_HANDLE_STR "/sys/class/gpio/"
#define SET_PIN_STRLEN 3

//test whether or not a pin is availible in the pi gpio header
short validPin(short pinNr);
//base function for exporting and unexporting pins to sysfs
short setPin(short pinNr, short export, FILE* sysf_handle);
short openValueFile(FILE** sysfs_handle, short pinNr, char* mode);
void safeClose(FILE* f);

short setPin(short pinNr, short export, FILE* sysf_handle){

    if(!validPin(pinNr)){
        printf("ERROR: pin %d is invalid\n", pinNr);
        return -1;
    }
    char* mode;
    if(export == 1){
        mode = "export";
    }else{
        mode = "unexport";
    }

    //Export gpio to be written
    char handle [100] = SYSF_HANDLE_STR;
    strcat(handle, mode);
    if((sysf_handle = fopen(handle, "w")) == NULL){
        printf("ERROR: Cannot %s GPIO. Does the program have root access?\n", mode);
        return 1;
    }
    //Export pin to be written
    char init_str[SET_PIN_STRLEN];
    snprintf(init_str, SET_PIN_STRLEN*sizeof(char), "%d", pinNr);
    if(fwrite(&init_str, sizeof(char), SET_PIN_STRLEN, sysf_handle ) != SET_PIN_STRLEN){
        printf("ERROR: Cannot %s GPIO pin %d", mode, pinNr);
        return 2;
    }
    return 0;
}
short initPin(short pinNr){
    #ifdef debug
        printf("DEBUG: Initializing gpio pin %d\n", pinNr);
    #endif
    FILE *sysf_handle = NULL;
    setPin(pinNr, 1, sysf_handle);
    safeClose(sysf_handle);
    return 0;
}

short setPinMode(short pinNr, gpio_mode_t pinMode){
    #ifdef debug
        printf("DEBUG: Setting pin %d to mode %s\n", pinNr, pinMode==0?"in":"out");
    #endif

    //Open file for writing
    FILE *sysf_handle = NULL;
    char handle[100] = SYSF_HANDLE_STR;
    char nr_str[SET_PIN_STRLEN];
    snprintf(nr_str, SET_PIN_STRLEN*sizeof(char), "%d", pinNr);
    strcat(handle, "gpio");
    strcat(handle, nr_str);
    strcat(handle, "/direction");
    if((sysf_handle = fopen(handle, "w")) == NULL){
        printf("ERROR: Cannot open direction file\n");
        return 1;
    }
    //check for desired mode
    char* mode_str;
    short mode_str_len;
    if(pinMode == 0){
        mode_str = "in";
        mode_str_len = 3;
    }else{
        mode_str = "out";
        mode_str_len = 4;
    }
    //write mode to direction file
    if(fwrite(mode_str, sizeof(char), mode_str_len, sysf_handle) != mode_str_len){
        printf("ERROR: Cannot write direction file\n");
        safeClose(sysf_handle);
        return 2;
    }
    safeClose(sysf_handle);

    #ifdef debug
        printf("DEBUG: Set pin %d to mode %s\n", pinNr, pinMode==0?"in":"out");
    #endif
    return 0;
}
short uninitPin(short pinNr){
    #ifdef debug
        printf("DEBUG: Uninitializing gpio pin %d\n", pinNr);
    #endif
    FILE *sysf_handle = NULL;
    setPin(pinNr, 0, sysf_handle);
    safeClose(sysf_handle);
}
short readPin(short pinNr){
    #ifdef debug
        printf("DEBUG: Reading pin %d\n", pinNr);
    #endif

    //Open file for reading
    FILE* sysfs_handle = NULL;
    char* mode = "r";


    if(openValueFile(&sysfs_handle, pinNr, mode) != 0){
        return -1;
    }

    //Read first character from file ('1'  or ' 0')
    int c;

    if((c = fgetc(sysfs_handle)) == EOF){
        printf("ERROR: Failed to read value from pin %d\n", pinNr);
        safeClose(sysfs_handle);
        return -2;
    }

    #ifdef debug
        char debugChar = (char) c;
        printf("DEBUG: read %c from pin %d\n", debugChar, pinNr);
    #endif

    return c - '0';
}
short writePin(short pinNr, short value){
    #ifdef debug
        printf("DEBUG: Writing pin %d with value %d\n", pinNr, value);
    #endif

    //Open file for writing
    FILE* sysfs_handle = NULL;
    char* mode = "w";
    if(openValueFile(&sysfs_handle, pinNr, mode) != 0){
        return -1;
    }
    char v[2];
    snprintf(v, 2, "%d", value);
    if(fwrite(v, sizeof(char), 1, sysfs_handle) != 1){
        printf("ERROR: Failed to write to %d pin %d\n", value, pinNr);
        return -2;
    }

    #ifdef debug
        printf("DEBUG: Wrote %d to pin %d\n", value, pinNr);
    #endif

    return 0;
}
short validPin(short pinNr){
    int i = 0;
    for(i ; i < sizeof(valid_pins)/sizeof(int) ; i++){
        if(pinNr == valid_pins[i]){
            return 1;
        }
    }
    return 0;
}
short openValueFile(FILE** sysfs_handle, short pinNr, char* mode){
    char nr_str[SET_PIN_STRLEN];
    char handle [100] = SYSF_HANDLE_STR;
    snprintf(nr_str, SET_PIN_STRLEN, "%d", pinNr);
    strcat(handle, "gpio");
    strcat(handle, nr_str);
    strcat(handle, "/value");

    if((*sysfs_handle = fopen(handle, mode)) == NULL){
        printf("ERROR: failed to open pin %d value file in mode %s\n", pinNr, mode);
        return -1;
    }

    #ifdef debug
        printf("DEBUG: Opened valuefile for pin %d\n", pinNr);
    #endif
    return 0;
}
void safeClose(FILE* f){
    if(f != NULL){
        fclose(f);
    }
}