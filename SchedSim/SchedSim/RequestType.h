//
//  RequestType.h
//  SchedSim
//
//  Created by Kurt Hardin on 5/17/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#ifndef SchedSim_RequestType_h
#define SchedSim_RequestType_h

#define SYSTEM_REQUEST_TYPE_KBD                 0
#define SYSTEM_REQUEST_TYPE_IO_VID              2
#define SYSTEM_REQUEST_TYPE_IO_DISK             3
#define SYSTEM_REQUEST_TYPE_READ_SHARED_MEM     1
#define SYSTEM_REQUEST_TYPE_WRITE_SHARED_MEM    4

typedef struct {
    int step;
    int type;
} RequestType;

#endif
