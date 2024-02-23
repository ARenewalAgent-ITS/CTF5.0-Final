#include <stdio.h>
#include <pthread.h> 
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "state.h"

char* buffer;
IPC *ipc;
VirtualMachine *vms[MAX_VM];
Instance *active;

void printConfig(char *src, unsigned long size) {
    for(size_t i = 0; i < size; i++) {
        printf("%c", src[i]);
    } 
}

void ExecuteLaunch() {
    if (ipc->vm->size > CONFIG_MAXLEN) {
        puts("[-] Configuration too long");
        ipc->command = WAIT;
        ipc->done = TRUE;
        return;
    }

    if (active != NULL) {
        puts("[-]  VM already active, Please terminate");
        ipc->command = WAIT;
        ipc->done = TRUE;
        return;
    }

    ipc->command = WAIT;
    ipc->done = TRUE;

    active = malloc(sizeof(Instance));
    active->original = ipc->vm;
    sleep(5);
    memcpy(active->original, ipc->vm, ipc->vm->size);

    puts("[+] Launch Protocol Success");
}

void ExecuteTerminate() {
    ipc->command = WAIT;
    ipc->done = TRUE;

    if (ipc->vm->size > CONFIG_MAXLEN) {
        puts("[-] Configuration too long");
        ipc->done = TRUE;
        return;
    }
    
    if (active == NULL) {
        puts("[-] No active VM");
    }

    sleep(5);
    puts("[*] Your Config: ");
    printConfig(active->original->config, ipc->vm->size);

    free(active);
    active = NULL;

    ipc->command = WAIT;
    ipc->done = TRUE;

    puts("[+] Terminate Protocol Success");
}

void create() {
    unsigned long idx;
    unsigned long size; 
    char *tmp;

    printf("idx >> ");
    scanf("%lu%*c", &idx);

    if (idx > MAX_VM - 1) {
        puts("[-] Out of bond idx");
        return;
    }

    printf("size >> ");
    scanf("%lu%*c", &size);

    if (size >= 500) {
        size = 500 - 1;
    }
    
    printf("config >> ");
    fgets(buffer, size, stdin);

    vms[idx] = malloc(sizeof(VirtualMachine));
    vms[idx]->size = size;
    memcpy(vms[idx]->config, buffer, (size > CONFIG_MAXLEN) ? CONFIG_MAXLEN : size);
    bzero(buffer, size - 1);

    printf("[+] VM[%ld] Created\n", idx);
}

void delete() {
    unsigned long idx;

    printf("idx >> ");
    scanf("%lu%*c", &idx);

    if (idx > MAX_VM - 1) {
        puts("[-] Out of bond idx");
        return;
    }

    if (vms[idx] == NULL) {
        puts("[-] VM not found");
        return;
    }

    free(vms[idx]);
    vms[idx] = NULL;
}

void launch() {
    unsigned long idx;

    printf("idx >> ");
    scanf("%lu%*c", &idx);
    ipc->vm = vms[idx];

    if (idx > MAX_VM - 1) {
        puts("[-] Out of bond idx");
        return;
    }

    if (vms[idx] == NULL) {
        puts("[-] VM not found");
        return;
    }

    ipc->command = LAUNCH;
    ipc->done = FALSE;

    printf("[*] Executing VM[%ld] Launch Protocol\n", idx);

    while(!ipc->done) {
        sleep(1);
    }
    ipc->done = FALSE;
}

void terminate() {
    if (active == NULL) {
        puts("[-] No active VM");
        return;
    }

    ipc->command = TERMINATE;
    ipc->done = FALSE;

    puts("[*] Executing Terminate Protocol");

    while(!ipc->done) {
        sleep(1);
    }
    ipc->done = FALSE;
}

void run_control() {
    int choice = 0;

    puts("[*] Controller started");

    while(1) {
        puts("Options:");
        puts("0. Create VM");
        puts("1. Delete VM");
        puts("2. Launch VM");
        puts("3. Terminate VM");
        puts("Input: ");

        scanf("%lu%*c", &choice);
        switch(choice) {
            case CREATE:
                create();
                break;
            case DELETE: 
                delete();
                break;
            case LAUNCH:
                launch();
                break;
            case TERMINATE:
                terminate();
                break;
            default:
                return;
        }
    }
}

void *run_backend() {
    puts("[*] Backend started");

    while(1) {
        switch (ipc->command) {
            case LAUNCH:
                ExecuteLaunch();
                break;
            case TERMINATE:
                ExecuteTerminate();
                break;
            default:
                sleep(1);
                break;
        }
    }
}

void init() {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
}

int main(int argc, char* argv[]) {
    init();

    buffer = malloc(500);
    ipc = malloc(sizeof(IPC));
    ipc->done = FALSE;
    ipc->command = WAIT;

    pthread_t backend;
    pthread_create(&backend, NULL, run_backend, NULL);

    run_control();

    pthread_join(backend, NULL);

    return 0;
}