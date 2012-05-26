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

boolean isRunning = NO;

void * KBDDevice_run(void *);

KBDDevice * KBDDevice_init(CPU *cpu) {
    KBDDevice *kbd = malloc(sizeof(KBDDevice));
    kbd->cpu = cpu;
    kbd->tid = malloc(sizeof(pthread_t));
    kbd->inputBuffer = LinkedBlockingQueue_init();
    kbd->blockedQueue = PCBQueue_init();
    isRunning = YES;
    pthread_create(kbd->tid, NULL, KBDDevice_run, kbd);
    return kbd;
}

void * KBDDevice_run(void *arg) {
    KBDDevice *kbd = (KBDDevice *)arg;
    while (isRunning) {
        char key = getchar();
        LinkedBlockingQueue_enqueue(kbd->inputBuffer, &key); 
        if (PCBQueue_getSize(kbd->blockedQueue) > 0) {
            CPU_signalInterrupt(kbd->cpu, Interrupt_init(INTERRUPT_TYPE_KBD, PCBQueue_dequeue(kbd->blockedQueue)));
        }
    }
    return EXIT_SUCCESS;
}