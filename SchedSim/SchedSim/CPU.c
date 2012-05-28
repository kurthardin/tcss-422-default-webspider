//
//  CPU.c
//  SchedSim
//
//  Created by Kurt Hardin on 5/17/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "CPU.h"
#include "SysTimer.h"
#include "Scheduler.h"
#include "IODevice.h"
#include "KBDDevice.h"

Interrupt * Interrupt_init(int type, PCB *src) {
    Interrupt *interrupt = malloc(sizeof(Interrupt));
    interrupt->type = type;
    interrupt->src = src;
    return interrupt;
}


void checkForInterrupt(CPU *);
void checkForSystemRequest(CPU *, int);

CPU * CPU_init() {
    CPU *cpu = malloc(sizeof(CPU));
    cpu->ip = 0;
    cpu->runningProcess = NULL;
    cpu->timer = NULL;
    int i;
    for (i = 0; i < INTERRUPT_PRIORITY_COUNT; i++) {
        cpu->interruptQueues[i] = LinkedBlockingQueue_init();
    }
    cpu->scheduler = Scheduler_init(cpu);
    cpu->dvcDisk = IODevice_init("Disk", cpu);
    cpu->dvcVid = IODevice_init("Video", cpu);
    cpu->dvcKbd = KBDDevice_init(cpu);
    
    for (i = 0; i < NUMBER_SHARED_MEMORY; i++) {
        cpu->sharedMemory[i] = SharedMemory_init();
    }
    
    return cpu;
}

void CPU_signalInterrupt(CPU *cpu, Interrupt *interrupt) {
    LinkedBlockingQueue_enqueue(cpu->interruptQueues[interrupt->type], interrupt);
}

void CPU_step(CPU *cpu) {
    
    
    checkForInterrupt(cpu);
    
    int ip = cpu->ip;
    cpu->ip++;
    if (cpu->ip > cpu->runningProcess->proc->noSteps - 1) {
        cpu->ip = 0;
    }
    checkForSystemRequest(cpu, ip);
    
}

void checkForInterrupt(CPU *cpu) {
    Interrupt *interrupt = NULL;
    int i;
    for (i = 0; i < INTERRUPT_PRIORITY_COUNT; i++) {
        interrupt = LinkedBlockingQueue_dequeue(cpu->interruptQueues[i]);
        if (interrupt != NULL) {
            cpu->runningProcess->nextStep = cpu->ip; //handleInterrupt(cpu, interrupt);
            Scheduler_handleInterrupt(cpu->scheduler, interrupt->src, interrupt->type);
            break;
        }
    }
}

void checkForSystemRequest(CPU *cpu, int ip) {
    Process *proc = cpu->runningProcess->proc;
    RequestType request;
    int i;
    for (i = 0; i < proc->noRequests; i++) {
        request = cpu->runningProcess->proc->requests[i];
        if (request.step == ip) {
            CPU_systemRequest(cpu, request.type);
            break;
        }
    }
}

void handleKeyboardSystemRequest(CPU *cpu) {
    cpu->runningProcess->nextStep = cpu->ip;
    cpu->runningProcess->state = PCB_STATE_BLOCKED;
    PCBQueue_enqueue(cpu->dvcKbd->blockedQueue, cpu->runningProcess);
    printf("Process %d: blocked on keyboard request\n", cpu->runningProcess->pid);
}

void handleIOSystemRequest(CPU *cpu, int type) {
    cpu->runningProcess->nextStep = cpu->ip;
    cpu->runningProcess->state = PCB_STATE_BLOCKED;
    PCBQueue *blockedQueue = NULL;
    char *ioType;
    if (type == SYSTEM_REQUEST_TYPE_IO_VID) {
        blockedQueue = cpu->dvcVid->blockedQueue;
        ioType = cpu->dvcVid->type;
    } else if (type == SYSTEM_REQUEST_TYPE_IO_DISK) {
        blockedQueue = cpu->dvcDisk->blockedQueue;
        ioType = cpu->dvcDisk->type;
    }
    if (blockedQueue != NULL) {
        PCBQueue_enqueue(blockedQueue, cpu->runningProcess);
    }
    printf("Process %d: blocked on IO request (%s)\n", cpu->runningProcess->pid, ioType);
}

void handleSharedMemoryRead(CPU *cpu, SharedMemory *sharedMemory) {
    
    if (sharedMemory->mode == SHARED_MEM_MODE_READ && 
        PCBQueue_getSize(sharedMemory->mutexReadBlockedQueue) == 0) {
        
        sharedMemory->owner = cpu->runningProcess;
        printf("Process %d: Acquired mutex lock\n", sharedMemory->owner->pid);
        printf("Process %d: Read from shared memory\n", cpu->runningProcess->pid);
        
        sharedMemory->owner = PCBQueue_dequeue(sharedMemory->mutexWriteBlockedQueue);
        printf("Process %d: Released mutex lock\n", cpu->runningProcess->pid);
        
        if (sharedMemory->owner != NULL) {
            printf("Process %d: Acquired mutex lock\n", sharedMemory->owner->pid);
        }
    } else {
        PCBQueue_enqueue(sharedMemory->mutexReadBlockedQueue, cpu->runningProcess);
        cpu->runningProcess->state = PCB_STATE_BLOCKED;
        cpu->runningProcess->waitingOn = SHARED_MEM_MODE_READ;
        printf("Process %d: Blocked on shared memory mutex\n", cpu->runningProcess->pid);
    }
}

void handleSharedMemoryWrite(CPU *cpu, SharedMemory *sharedMemory) {
    
    if (sharedMemory->mode == SHARED_MEM_MODE_WRITE && 
        PCBQueue_getSize(sharedMemory->mutexWriteBlockedQueue) == 0) {
        
        sharedMemory->owner = cpu->runningProcess;
        printf("Process %d: Acquired mutex lock\n", sharedMemory->owner->pid);
        printf("Process %d: Wrote to shared memory\n", sharedMemory->owner->pid);
        
        sharedMemory->owner = PCBQueue_dequeue(sharedMemory->mutexReadBlockedQueue);
        printf("Process %d: Released mutex lock\n", cpu->runningProcess->pid);
        
        if (sharedMemory->owner != NULL) {
            printf("Process %d: Acquired mutex lock\n", sharedMemory->owner->pid);
        }
    } else {
        PCBQueue_enqueue(sharedMemory->mutexWriteBlockedQueue, cpu->runningProcess);
        cpu->runningProcess->state = PCB_STATE_BLOCKED;
        cpu->runningProcess->waitingOn = SHARED_MEM_MODE_WRITE;
        printf("Process %d: Blocked on shared memory mutex\n", cpu->runningProcess->pid);
    }
}

void CPU_systemRequest(CPU *cpu, int type) {
    int index = cpu->runningProcess->mem_ref;
    switch (type) {
        
        case SYSTEM_REQUEST_TYPE_KBD:
            handleKeyboardSystemRequest(cpu);
            break;
            
        case SYSTEM_REQUEST_TYPE_IO_VID:
        case SYSTEM_REQUEST_TYPE_IO_DISK:
            handleIOSystemRequest(cpu, type);
            break;
            
        case SYSTEM_REQUEST_TYPE_READ_SHARED_MEM:
            handleSharedMemoryRead(cpu, cpu->sharedMemory[index]);
            break;
            
        case SYSTEM_REQUEST_TYPE_WRITE_SHARED_MEM:
            handleSharedMemoryWrite(cpu, cpu->sharedMemory[index]);
            break;
            
        default:
            break;
    }
    
    cpu->runningProcess->nextStep = cpu->ip;
    Scheduler_handleSystemRequest(cpu->scheduler);
}