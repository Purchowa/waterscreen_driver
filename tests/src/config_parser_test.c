#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

#include "config/waterscreen_config.h"
#include "http/waterscreen_config_parser.h"

static bool compareConfigs( const WaterscreenConfig_t *first, const WaterscreenConfig_t *second )
{
    bool areEqual = true;

    areEqual &= first->mode.current == second->mode.current;
    areEqual &= first->standardModeConfig.isWorkingDuringWeekends == second->standardModeConfig.isWorkingDuringWeekends;
    areEqual &= first->standardModeConfig.workTimeInStandardMode == second->standardModeConfig.workTimeInStandardMode;
    areEqual &= first->standardModeConfig.idleTimeInStandardMode == second->standardModeConfig.idleTimeInStandardMode;
    areEqual &= first->standardModeConfig.workRange.from == second->standardModeConfig.workRange.from;
    areEqual &= first->standardModeConfig.workRange.to == second->standardModeConfig.workRange.to;

    return areEqual;
}

void givenReadConfig_configParser_returnNoUpdate()
{
    WaterscreenConfig_t cfg = {};
    const char *sampleJson  = "{\"wasRead\":true,\"mode\":2,\"enableWeekends\":true,\"workTime\":10,\"idleTime\":5,"
                              "\"workRange\":{\"from\": 5, \"to\": 11}}";
    const bool  isInitialRequest = false;

    assert_int_equal( parseJsonToWaterscreenCfg( sampleJson, &cfg, isInitialRequest ), Http_WaterscreenConfigNoUpdate );
}

void givenReadConfigAndInitialRequest_configParser_ignoreWasReadFlagAndParse()
{
    WaterscreenConfig_t cfg = {};
    const char *sampleJson  = "{\"wasRead\":true,\"mode\":2,\"enableWeekends\":true,\"workTime\":10,\"idleTime\":5,"
                              "\"workRange\":{\"from\": 5, \"to\": 11}}";
    const bool  isInitialRequest = true;

    assert_int_equal( parseJsonToWaterscreenCfg( sampleJson, &cfg, isInitialRequest ), Http_Success );
}

void givenRawValidJson_configParser_fillConfigStructure()
{
    const char *sampleJson = "{\"wasRead\":false,\"mode\":2,\"enableWeekends\":true,\"workTime\":10,\"idleTime\":5,"
                             "\"workRange\":{\"from\": 5, \"to\": 11}}";

    const WaterscreenConfig_t expectedConfig = {
        .mode = Mode_Service,
        .standardModeConfig =
            {
                .isWorkingDuringWeekends = true,
                .workTimeInStandardMode  = 10,
                .idleTimeInStandardMode  = 5,
                .workRange               = { .from = 5, .to = 11 },
            },
    };
    const bool isInitialRequest = false;

    WaterscreenConfig_t config = {};
    assert_int_equal( parseJsonToWaterscreenCfg( sampleJson, &config, isInitialRequest ), Http_Success );
    assert_true( compareConfigs( &expectedConfig, &config ) );
}

void givenInvlaidMode_configParser_returnParsingError()
{
    WaterscreenConfig_t config           = {};
    const bool          isInitialRequest = false;

    const char *invlaidMode_lower = "{\"wasRead\":false,\"mode\":-1,\"enableWeekends\":true,\"workTime\":10,"
                                    "\"idleTime\":5,\"workRange\":{\"from\": 5, \"to\": 11}}";
    assert_int_equal( parseJsonToWaterscreenCfg( invlaidMode_lower, &config, isInitialRequest ),
                      Http_WaterscreenConfigParsingError );

    const char *invlaidMode_higher = "{\"wasRead\":false,\"mode\":3,\"enableWeekends\":true,\"workTime\":10,"
                                     "\"idleTime\":5,\"workRange\":{\"from\": 5, \"to\": 11}}";
    assert_int_equal( parseJsonToWaterscreenCfg( invlaidMode_higher, &config, isInitialRequest ),
                      Http_WaterscreenConfigParsingError );
}

void givenWorkHoursOutsideRange_configParser_returnSuccessAndClamp()
{
    const char *sampleJson = "{\"wasRead\":false,\"mode\":2,\"enableWeekends\":true,\"workTime\":10,\"idleTime\":5,"
                             "\"workRange\":{\"from\": -1, \"to\": 24}}";

    const WaterscreenConfig_t expectedConfig = {
        .mode = Mode_Service,
        .standardModeConfig =
            {
                .isWorkingDuringWeekends = true,
                .workTimeInStandardMode  = 10,
                .idleTimeInStandardMode  = 5,
                .workRange               = { .from = 0, .to = 24 },
            },
    };
    const bool isInitialRequest = false;

    WaterscreenConfig_t config = {};
    assert_int_equal( parseJsonToWaterscreenCfg( sampleJson, &config, isInitialRequest ), Http_Success );
    assert_true( compareConfigs( &expectedConfig, &config ) );
}

void givenInvlaidWorkRange_configParser_returnParsingError()
{
    const char *sampleJson = "{\"wasRead\":false,\"mode\":2,\"enableWeekends\":true,\"workTime\":10,\"idleTime\":5,"
                             "\"workRange\":{\"from\": 10, \"to\": 9}}";
    const bool  isInitialRequest = false;

    WaterscreenConfig_t config = {};
    assert_int_equal( parseJsonToWaterscreenCfg( sampleJson, &config, isInitialRequest ),
                      Http_WaterscreenConfigParsingError );
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test( givenRawValidJson_configParser_fillConfigStructure ),
        cmocka_unit_test( givenInvlaidMode_configParser_returnParsingError ),
        cmocka_unit_test( givenReadConfig_configParser_returnNoUpdate ),
        cmocka_unit_test( givenReadConfigAndInitialRequest_configParser_ignoreWasReadFlagAndParse ),
        cmocka_unit_test( givenWorkHoursOutsideRange_configParser_returnSuccessAndClamp ),
        cmocka_unit_test( givenInvlaidWorkRange_configParser_returnParsingError ),
    };

    return cmocka_run_group_tests_name( "JSON config parser", tests, NULL, NULL );
}
