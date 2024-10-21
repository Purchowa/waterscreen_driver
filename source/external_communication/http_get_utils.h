#ifndef HTTP_GET_BUFFER_H_
#define HTTP_GET_BUFFER_H_

#include <stdbool.h>

#define HTTP_HEAD_CONVERSION_FAILED_VALUE 0


char *getClearedHttpGETBuffer();

bool httpGET_receiveContent( char **destContent, const char *authorization, const char *apiUrl );

#endif /* HTTP_GET_BUFFER_H_ */
