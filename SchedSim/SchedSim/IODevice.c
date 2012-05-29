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
#include "GUI.h"

#include "IODevice.h"

void * IODevice_run(void *);

IODevice * IODevice_init(char *type, CPU *cpu) {
    IODevice *device = malloc(sizeof(IODevice));
    device->type = type;
    device->blockedQueue = PCBQueue_init();
    device->cpu = cpu;
    device->tid = malloc(sizeof(pthread_t));
    pthread_create(device->tid, NULL, IODevice_run, device);
    return device;
}

void * IODevice_run(void *arg) {
    
    IODevice *device = (IODevice *)arg;
    
    PCB *pcb = PCBQueue_blockingDequeue(device->blockedQueue);
    
    while (pcb != NULL) { // CAN THIS THREAD BE STOPPED WITH pthread_dettach?
        
        char msg[150];
        sprintf(msg, "%s device running to service process %d", device->type, pcb->pid);
        SchedSimGUI_printLogMessage((SchedSimGUI *) device->cpu->gui, msg);
        
        srand(time(NULL));
        sleep(rand() % 5);
        
        CPU_signalInterrupt(device->cpu, Interrupt_init(INTERRUPT_TYPE_IO, pcb));
        
        sprintf(msg, "%s device finished service for process %d", device->type, pcb->pid);
        SchedSimGUI_printLogMessage((SchedSimGUI *) device->cpu->gui, msg);
        
        pcb = PCBQueue_blockingDequeue(device->blockedQueue);
    }
    
    return EXIT_SUCCESS;
}