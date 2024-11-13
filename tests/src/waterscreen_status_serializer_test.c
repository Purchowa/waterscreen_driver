#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

#include "config/waterscreen_config.h"
#include "external_communication/waterscreen_status_serializer.h"

#include <stdlib.h>

void givenStatus_serializeWaterscreenStatus_returnNotNullJsonString()
{
    const WaterscreenStatus_t status = {
        .mode = Mode_Standard, .fluidLevel = FluidLevel_Optimal, .isPresenting = true };
    const char *expectedJsonStr = "{\"mode\":0,\"fluidLevel\":0,\"isPresenting\":true}";

    char *jsonStr = serializeWaterscreenStatus( &status );
    assert_non_null( jsonStr );
    assert_string_equal( expectedJsonStr, jsonStr );

    free( jsonStr );
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test( givenStatus_serializeWaterscreenStatus_returnNotNullJsonString ),
    };

    return cmocka_run_group_tests_name( "water-screen status JSON serializer", tests, NULL, NULL );
}
