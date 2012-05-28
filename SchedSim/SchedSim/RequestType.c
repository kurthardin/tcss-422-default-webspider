//
//  RequestType.c
//  SchedSim
//
//  Created by Ju Choe on 5/28/12.
//  Copyright (c) 2012 University of Washington at Tacoma. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "RequestType.h"


RequestType * RequestType_init(int step, int type) {
    RequestType *request = malloc(sizeof(RequestType));
    request->step = step;
    request->type = type;
    return request;
}