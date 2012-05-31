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

IODevice * IODevice_init(int type, CPU *cpu) {
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
    
    PCB *pcb = PCBQueue_blockingPeek(device->blockedQueue);
    
    while (pcb != NULL) { // CAN THIS THREAD BE STOPPED WITH pthread_dettach?
        
        char msg[150];
        sprintf(msg, "running to service process %d", pcb->pid);
        SchedSimGUI_printLogMessage((SchedSimGUI *) device->cpu->gui, device->type, 0, msg);
        
        srand(time(NULL));
        sleep(rand() % 5);
        
        pcb = PCBQueue_dequeue(device->blockedQueue);
        CPU_signalInterrupt(device->cpu, Interrupt_init(INTERRUPT_TYPE_IO, pcb));
        
        sprintf(msg, "finished servicing process %d", pcb->pid);
        SchedSimGUI_printLogMessage((SchedSimGUI *) device->cpu->gui, device->type, 0, msg);
        
        pcb = PCBQueue_blockingPeek(device->blockedQueue);
        
        if (!CPU_isRunning(device->cpu)) {
            break;
        }
    }
    
    return EXIT_SUCCESS;
}