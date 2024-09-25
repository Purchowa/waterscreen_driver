#ifndef DATETIME_UTILS_H_
#define DATETIME_UTILS_H_

#include "datetime_types.h"


typedef enum
{
    FirstIsBigger  = 1,
    Equal          = 0,
    SecondIsBigger = -1

} Comparison_t;

Comparison_t compareShortDates( const ShortDate_t, const ShortDate_t );


#endif /* DATETIME_UTILS_H_ */
