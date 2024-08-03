#ifndef COMMON_STATE_ASSERTY_H_
#define COMMON_STATE_ASSERTY_H_

#include "waterscreen_state_context.h"

typedef enum {
    Success,
    InvalidArgument,
    Timeout
} ReturnCodes_t;

void assertClosedValves();

#endif