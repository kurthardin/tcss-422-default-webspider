//
//  RequestType.h
//  SchedSim
//
//  Created by Kurt Hardin on 5/17/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#ifndef SchedSim_RequestType_h
#define SchedSim_RequestType_h

#define REQUEST_TYPE_IO     0
#define REQUEST_TYPE_SYNC   1

typedef struct {
    int step
    int type
} RequestType

#endif
