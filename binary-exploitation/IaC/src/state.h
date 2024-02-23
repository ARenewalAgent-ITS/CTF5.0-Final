#ifndef STATE_H 
#define STATE_H

#define CONFIG_MAXLEN 120
#define ID_MAXLEN 16
#define MAX_VM 15

#define RUNNING 77
#define FAULTY 66

#define WAIT -1
#define CREATE 0
#define DELETE 1
#define LAUNCH 2
#define TERMINATE 3
#define CHEAT 69

#define TRUE 1
#define FALSE 0

typedef struct VirtualMachine {
    char config[CONFIG_MAXLEN];
    long size;   
} VirtualMachine;

typedef struct IPC {
    VirtualMachine *vm;
    int command;
    int done;
} IPC;

typedef struct Instance {
    VirtualMachine *original;
    char config[CONFIG_MAXLEN];
} Instance;

#endif