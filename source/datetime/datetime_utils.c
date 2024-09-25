#include "datetime_utils.h"

#include <stdbool.h>


Comparison_t compareShortDates( const ShortDate_t first, const ShortDate_t second )
{
    const bool areMonthsEqual = first.month == second.month;

    if ( first.month < second.month || ( areMonthsEqual && first.day < second.day ) )
        return SecondIsBigger;

    if ( second.month < first.month || ( areMonthsEqual && second.day < first.day ) )
        return FirstIsBigger;

    return Equal;
}
