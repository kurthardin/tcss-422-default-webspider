//
//  LinkedQueueNode.c
//  SchedSim
//
//  Created by Kurt Hardin on 5/17/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "LinkedQueueNode.h"

LinkedQueueNode* LinkedQueueNode_init(void* the_data) {
    LinkedQueueNode* new_node = malloc(sizeof(LinkedQueueNode));
    new_node->data = the_data;
    new_node->next_node = NULL;
    return new_node;
}