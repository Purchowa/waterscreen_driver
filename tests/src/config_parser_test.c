#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

#include "config/waterscreen_config.h"
#include "external_communication/waterscreen_config_parser.h"

#include <string.h>

static bool compareConfigs( const WaterscreenConfig_t *first, const WaterscreenConfig_t *second )
{
    bool areEqual = true;

    areEqual &= first->mode == second->mode;
    areEqual &= first->standardModeConfig.isWorkingDuringWeekends == second->standardModeConfig.isWorkingDuringWeekends;
    areEqual &= first->standardModeConfig.workTimeInStandardMode == second->standardModeConfig.workTimeInStandardMode;
    areEqual &= first->standardModeConfig.idleTimeInStandardMode == second->standardModeConfig.idleTimeInStandardMode;
    areEqual &= first->standardModeConfig.workRange.from == second->standardModeConfig.workRange.from;
    areEqual &= first->standardModeConfig.workRange.to == second->standardModeConfig.workRange.to;

    areEqual &= first->customPictureSize == second->customPictureSize;
    if ( areEqual )
        areEqual &= !memcmp( first->customPicture, second->customPicture,
                             sizeof( *first->customPicture ) * first->customPictureSize );

    return areEqual;
}

void givenReadConfig_configParser_returnNoUpdate()
{
    WaterscreenConfig_t cfg;
    const char         *sampleJson =
        "{\"wasRead\":true,\"mode\":2,\"enableWeekends\":true,\"workTime\":10,\"idleTime\":5,\"picture\":{\"size\":"
        "4,\"data\":[0,0,24,1567]}, \"workRange\":{\"from\": 5, \"to\": 11}}";
    const bool isInitialRequest = false;

    assert_int_equal( fromJsonToWaterscreenCfg( sampleJson, &cfg, isInitialRequest ), Http_WaterscreenConfigNoUpdate );
}

void givenReadConfigAndInitialRequest_configParser_ignoreWasReadFlagAndParse()
{
    WaterscreenConfig_t cfg;
    const char         *sampleJson =
        "{\"wasRead\":true,\"mode\":2,\"enableWeekends\":true,\"workTime\":10,\"idleTime\":5,\"picture\":{\"size\":"
        "4,\"data\":[0,0,24,1567]}, \"workRange\":{\"from\": 5, \"to\": 11}}";
    const bool isInitialRequest = true;

    assert_int_equal( fromJsonToWaterscreenCfg( sampleJson, &cfg, isInitialRequest ), Http_Success );
}

void givenRawValidJson_configParser_fillConfigStructure()
{
    const char *sampleJson =
        "{\"wasRead\":false,\"mode\":2,\"enableWeekends\":true,\"workTime\":10,\"idleTime\":5,\"picture\":{\"size\":"
        "4,\"data\":[0,0,24,1567]}, \"workRange\":{\"from\": 5, \"to\": 11}}";
    const WaterscreenConfig_t expectedConfig   = { .mode = Mode_Service,
                                                   .standardModeConfig =
                                                       {
                                                           .isWorkingDuringWeekends = true,
                                                           .workTimeInStandardMode  = 10,
                                                           .idleTimeInStandardMode  = 5,
                                                           .workRange               = { .from = 5, .to = 11 },
                                                     },
                                                   .customPictureSize = 4,
                                                   .customPicture     = { 0, 0, 24, 1567 } };
    const bool                isInitialRequest = false;

    WaterscreenConfig_t config;
    assert_int_equal( fromJsonToWaterscreenCfg( sampleJson, &config, isInitialRequest ), Http_Success );
    assert_true( compareConfigs( &expectedConfig, &config ) );
}

void givenInvlaidMode_configParser_returnParsingError()
{
    WaterscreenConfig_t config;
    const bool          isInitialRequest = false;

    const char *invlaidMode_lower =
        "{\"wasRead\":false,\"mode\":-1,\"enableWeekends\":true,\"workTime\":10,\"idleTime\":5,\"picture\":{\"size\":"
        "4,\"data\":[0,0,24,1567]}, \"workRange\":{\"from\": 5, \"to\": 11}}";
    assert_int_equal( fromJsonToWaterscreenCfg( invlaidMode_lower, &config, isInitialRequest ),
                      Http_WaterscreenConfigParsingError );

    const char *invlaidMode_higher =
        "{\"wasRead\":false,\"mode\":3,\"enableWeekends\":true,\"workTime\":10,\"idleTime\":5,\"picture\":{\"size\":"
        "4,\"data\":[0,0,24,1567]}, \"workRange\":{\"from\": 5, \"to\": 11}}";
    assert_int_equal( fromJsonToWaterscreenCfg( invlaidMode_higher, &config, isInitialRequest ),
                      Http_WaterscreenConfigParsingError );
}

void givenArrayShorterThanSize_configParser_returnParsingError()
{
    WaterscreenConfig_t config;
    const bool          isInitialRequest = false;

    const char *invalidArrayWithSize =
        "{\"wasRead\":false,\"mode\":0,\"enableWeekends\":true,\"workTime\":0,\"idleTime\":0,\"picture\":{\"size\":"
        "4,\"data\":[0,0,24]}, \"workRange\":{\"from\": 5, \"to\": 11}}";
    assert_int_equal( fromJsonToWaterscreenCfg( invalidArrayWithSize, &config, isInitialRequest ),
                      Http_WaterscreenConfigParsingError );
}

void givenInvalidSize_configParser_returnSuccessAndClamp()
{
    WaterscreenConfig_t configLower;
    const bool          isInitialRequest = false;

    const char *invlaidSize_lower =
        "{\"wasRead\":false,\"mode\":2,\"enableWeekends\":true,\"workTime\":10,\"idleTime\":5,\"picture\":{\"size\":"
        "-1,\"data\":[0,0,24,1567]}, \"workRange\":{\"from\": 5, \"to\": 11}}";
    WaterscreenConfig_t expectedConfig = { .mode = Mode_Service,
                                           .standardModeConfig =
                                               {
                                                   .isWorkingDuringWeekends = true,
                                                   .workTimeInStandardMode  = 10,
                                                   .idleTimeInStandardMode  = 5,
                                                   .workRange               = { .from = 5, .to = 11 },
                                               },
                                           .customPictureSize = 0,
                                           .customPicture     = { 0, 0, 24, 1567 } };

    assert_int_equal( fromJsonToWaterscreenCfg( invlaidSize_lower, &configLower, isInitialRequest ), Http_Success );
    assert_true( compareConfigs( &expectedConfig, &configLower ) );

    WaterscreenConfig_t configHigher;
    const char         *invlaidSize_higher =
        "{\"wasRead\":false,\"mode\":2,\"enableWeekends\":true,\"workTime\":10,\"idleTime\":5,\"picture\":{\"size\":"
        "65,\"data\":[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"
        "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]}, \"workRange\":{\"from\": 5, \"to\": 11}}";

    expectedConfig.customPictureSize = MAX_CUSTOM_PICTURE_LENGTH;
    memset( expectedConfig.customPicture, 0,
            sizeof( *expectedConfig.customPicture ) * expectedConfig.customPictureSize );

    assert_int_equal( fromJsonToWaterscreenCfg( invlaidSize_higher, &configHigher, isInitialRequest ), Http_Success );
    assert_true( compareConfigs( &expectedConfig, &configHigher ) );
}

void givenWorkHoursOutsideRange_configParser_returnSuccessAndClamp()
{
    const char *sampleJson =
        "{\"wasRead\":false,\"mode\":2,\"enableWeekends\":true,\"workTime\":10,\"idleTime\":5,\"picture\":{\"size\":"
        "4,\"data\":[0,0,24,1567]}, \"workRange\":{\"from\": -1, \"to\": 25}}";
    const WaterscreenConfig_t expectedConfig   = { .mode = Mode_Service,
                                                   .standardModeConfig =
                                                       {
                                                           .isWorkingDuringWeekends = true,
                                                           .workTimeInStandardMode  = 10,
                                                           .idleTimeInStandardMode  = 5,
                                                           .workRange               = { .from = 0, .to = 24 },
                                                     },
                                                   .customPictureSize = 4,
                                                   .customPicture     = { 0, 0, 24, 1567 } };
    const bool                isInitialRequest = false;

    WaterscreenConfig_t config;
    assert_int_equal( fromJsonToWaterscreenCfg( sampleJson, &config, isInitialRequest ), Http_Success );
    assert_true( compareConfigs( &expectedConfig, &config ) );
}

void givenInvlaidWorkRange_configParser_returnParsingError()
{
    const char *sampleJson =
        "{\"wasRead\":false,\"mode\":2,\"enableWeekends\":true,\"workTime\":10,\"idleTime\":5,\"picture\":{\"size\":"
        "4,\"data\":[0,0,24,1567]}, \"workRange\":{\"from\": 10, \"to\": 9}}";
    const bool isInitialRequest = false;

    WaterscreenConfig_t config;
    assert_int_equal( fromJsonToWaterscreenCfg( sampleJson, &config, isInitialRequest ),
                      Http_WaterscreenConfigParsingError );
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test( givenRawValidJson_configParser_fillConfigStructure ),
        cmocka_unit_test( givenInvlaidMode_configParser_returnParsingError ),
        cmocka_unit_test( givenArrayShorterThanSize_configParser_returnParsingError ),
        cmocka_unit_test( givenInvalidSize_configParser_returnSuccessAndClamp ),
        cmocka_unit_test( givenReadConfig_configParser_returnNoUpdate ),
        cmocka_unit_test( givenReadConfigAndInitialRequest_configParser_ignoreWasReadFlagAndParse ),
        cmocka_unit_test( givenWorkHoursOutsideRange_configParser_returnSuccessAndClamp ),
        cmocka_unit_test( givenInvlaidWorkRange_configParser_returnParsingError ),
    };

    return cmocka_run_group_tests_name( "JSON config parser", tests, NULL, NULL );
}
