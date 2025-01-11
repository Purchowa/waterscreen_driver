#ifndef HMI_TASK_H_
#define HMI_TASK_H_

#include "FreeRTOSConfig.h"


#define HMI_TASK_PRIORITY   ( configTIMER_TASK_PRIORITY - 3 )
#define HMI_TASK_STACK_SIZE 512

void hmiTask( void *params );

#endif /* HMI_TASK_H_ */
