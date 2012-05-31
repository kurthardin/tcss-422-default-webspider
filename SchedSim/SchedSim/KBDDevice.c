//  Team: default
//  Names: Bartholomew, Dan
//         Choe, Ju
//         Hardin, Kurt
//
//  KBDDevice.c
//  SchedSim
//
//  Created by Kurt Hardin on 5/20/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "Defines.h"
#include "KBDDevice.h"

void * KBDDevice_run(void *);

KBDDevice * KBDDevice_init(CPU *cpu) {
    KBDDevice *kbd = malloc(sizeof(KBDDevice));
    kbd->cpu = cpu;
    kbd->tid = malloc(sizeof(pthread_t));
    kbd->inputBuffer = LinkedBlockingQueue_init();
    kbd->blockedQueue = PCBQueue_init();
    pthread_create(kbd->tid, NULL, KBDDevice_run, kbd);
    return kbd;
}

void * KBDDevice_run(void *arg) {
    KBDDevice *kbd = (KBDDevice *)arg;
    char *key;
    while (CPU_isRunning(kbd->cpu)) {
        key = malloc(sizeof(char));
        *key = getchar();
        if (*key == 'q') {
            CPU_setIsRunning(kbd->cpu, NO);
            break;
        }
        LinkedBlockingQueue_enqueue(kbd->inputBuffer, key); 
        if (PCBQueue_getSize(kbd->blockedQueue) > 0) {
            CPU_signalInterrupt(kbd->cpu, Interrupt_init(INTERRUPT_TYPE_KBD, PCBQueue_dequeue(kbd->blockedQueue)));
        }
    }
    return EXIT_SUCCESS;
}