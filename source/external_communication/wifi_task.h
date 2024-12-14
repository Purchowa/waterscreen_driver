#ifndef WIFI_TASK_H_
#define WIFI_TASK_H_

#include "http_return_codes.h"
#include "FreeRTOSConfig.h"


#define WIFI_TASK_PRIORITY   ( configTIMER_TASK_PRIORITY - 3 )
#define WIFI_TASK_STACK_SIZE 2048

void              wifiTask( void *params );
HttpReturnCodes_t getLastHttpCode();

#endif /* WIFI_TASK_H_ */
