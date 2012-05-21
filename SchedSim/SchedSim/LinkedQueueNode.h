//
//  LinkedQueueNode.h
//  SchedSim
//
//  Created by Kurt Hardin on 5/17/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#ifndef SchedSim_LinkedQueueNode_h
#define SchedSim_LinkedQueueNode_h

typedef struct LinkedQueueNode {
    void* data;
    struct LinkedQueueNode* next_node;
} LinkedQueueNode;

LinkedQueueNode* LinkedQueueNode_init(void*);

#endif
