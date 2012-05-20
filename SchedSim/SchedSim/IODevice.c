//
//  IODevice.c
//  SchedSim
//
//  Created by Kurt Hardin on 5/17/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "IODevice.h"

void * IODevice_run(void *);

IODevice * IODevice_init(char *type, CPU *cpu) {
    IODevice *device = malloc(sizeof(IODevice));
    device->type = type;
    device->blocked_queue = pcb_queue_init();
    device->cpu = cpu;
    device->tid = malloc(sizeof(pthread_t));
    pthread_create(device->tid, NULL, IODevice_run, device);
    return device;
}

void * IODevice_run(void *arg) {
    
    IODevice *device = (IODevice *)arg;
    PCB *pcb = pcb_queue_dequeue(device->blocked_queue);
    
    while (pcb != NULL) { // CAN THIS THREAD BE STOPPED WITH pthread_dettach?
        
        printf("%s device running to service process %d", device->type, pcb->pid);
        
        srand(time(NULL));
        sleep(rand() % 5000);
        
        CPU_signalInterrupt(device->cpu, Interrupt_make(INTERRUPT_TYPE_IO, device));
        
        pcb = pcb_queue_dequeue(device->blocked_queue);
    }
    
    return EXIT_SUCCESS;
}