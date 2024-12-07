#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

#include "config/waterscreen_config.h"
#include "external_communication/waterscreen_config_parser.h"

#include <string.h>

static pictureRow_t  customPictureBuffer[MAX_CUSTOM_PICTURE_HEIGHT];
static PictureInfo_t customPicture = { .picture = { .data = customPictureBuffer } };

static bool compareRGB( const ColorRGB_t *first, const ColorRGB_t *second )
{
    bool areEqual = true;

    areEqual &= first->r == second->r;
    areEqual &= first->g == second->g;
    areEqual &= first->b == second->b;

    return areEqual;
}

static bool compareConfigs( const WaterscreenConfig_t *first, const WaterscreenConfig_t *second )
{
    bool areEqual = true;

    areEqual &= first->mode == second->mode;
    areEqual &= first->standardModeConfig.isWorkingDuringWeekends == second->standardModeConfig.isWorkingDuringWeekends;
    areEqual &= first->standardModeConfig.workTimeInStandardMode == second->standardModeConfig.workTimeInStandardMode;
    areEqual &= first->standardModeConfig.idleTimeInStandardMode == second->standardModeConfig.idleTimeInStandardMode;
    areEqual &= first->standardModeConfig.workRange.from == second->standardModeConfig.workRange.from;
    areEqual &= first->standardModeConfig.workRange.to == second->standardModeConfig.workRange.to;

    areEqual &= first->customPicture->picture.size == second->customPicture->picture.size;
    if ( areEqual )
        areEqual &= !memcmp( first->customPicture->picture.data, second->customPicture->picture.data,
                             first->customPicture->picture.size * sizeof( *first->customPicture->picture.data ) );

    areEqual &= compareRGB( &first->customPicture->colors.main, &second->customPicture->colors.main );
    areEqual &= compareRGB( &first->customPicture->colors.secondary, &second->customPicture->colors.secondary );

    return areEqual;
}

void givenReadConfig_configParser_returnNoUpdate()
{
    WaterscreenConfig_t cfg = { .customPicture = &customPicture };
    const char         *sampleJson =
        "{\"wasRead\":true,\"mode\":2,\"enableWeekends\":true,\"workTime\":10,\"idleTime\":5,\"picture\":{\"size\":"
        "4,\"data\":[0,0,24,1567]}, \"workRange\":{\"from\": 5, \"to\": 11}}";
    const bool isInitialRequest = false;

    assert_int_equal( fromJsonToWaterscreenCfg( sampleJson, &cfg, isInitialRequest ), Http_WaterscreenConfigNoUpdate );
}

void givenReadConfigAndInitialRequest_configParser_ignoreWasReadFlagAndParse()
{
    WaterscreenConfig_t cfg = { .customPicture = &customPicture };
    const char *sampleJson  = "{\"wasRead\":true,\"mode\":2,\"enableWeekends\":true,\"workTime\":10,\"idleTime\":5,"
                              "\"picture\":{\"size\":4,\"data\":[0,0,24,1567],"
                              " \"colors\": { "
                              "  \"main\": { \"r\": 0, \"g\": 100, \"b\": 200 },"
                              "  \"secondary\":{ \"r\": 200, \"g\": 100, \"b\": 0 } } },"
                              "\"workRange\":{\"from\": 5, \"to\": 11}}";
    const bool  isInitialRequest = true;

    assert_int_equal( fromJsonToWaterscreenCfg( sampleJson, &cfg, isInitialRequest ), Http_Success );
}

void givenRawValidJson_configParser_fillConfigStructure()
{
    const char *sampleJson = "{\"wasRead\":false,\"mode\":2,\"enableWeekends\":true,\"workTime\":10,\"idleTime\":5,"
                             "\"picture\":{\"size\":4,\"data\":[0,0,24,1567],"
                             " \"colors\": { "
                             "  \"main\": { \"r\": 0, \"g\": 100, \"b\": 200 },"
                             "  \"secondary\":{ \"r\": 200, \"g\": 100, \"b\": 0 } } },"
                             "\"workRange\":{\"from\": 5, \"to\": 11}}";

    pictureRow_t  expectedPictureData[MAX_CUSTOM_PICTURE_HEIGHT] = { 0, 0, 24, 1567 };
    PictureInfo_t expectedCustomPicture = { .picture = { .size = 4, .data = expectedPictureData },
                                            .colors  = { .main = { 0, 100, 200 }, .secondary = { 200, 100, 0 } } };

    const WaterscreenConfig_t expectedConfig   = { .mode = Mode_Service,
                                                   .standardModeConfig =
                                                       {
                                                           .isWorkingDuringWeekends = true,
                                                           .workTimeInStandardMode  = 10,
                                                           .idleTimeInStandardMode  = 5,
                                                           .workRange               = { .from = 5, .to = 11 },
                                                     },
                                                   .customPicture = &expectedCustomPicture };
    const bool                isInitialRequest = false;

    WaterscreenConfig_t config = { .customPicture = &customPicture };
    assert_int_equal( fromJsonToWaterscreenCfg( sampleJson, &config, isInitialRequest ), Http_Success );
    assert_true( compareConfigs( &expectedConfig, &config ) );
}

void givenInvlaidMode_configParser_returnParsingError()
{
    WaterscreenConfig_t config           = { .customPicture = &customPicture };
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
    WaterscreenConfig_t config           = { .customPicture = &customPicture };
    const bool          isInitialRequest = false;

    const char *invalidArrayWithSize =
        "{\"wasRead\":false,\"mode\":0,\"enableWeekends\":true,\"workTime\":0,\"idleTime\":0,\"picture\":{\"size\":"
        "4,\"data\":[0,0,24]}, \"workRange\":{\"from\": 5, \"to\": 11}}";
    assert_int_equal( fromJsonToWaterscreenCfg( invalidArrayWithSize, &config, isInitialRequest ),
                      Http_WaterscreenConfigParsingError );
}

void givenInvalidSize_configParser_returnSuccessAndClamp()
{
    WaterscreenConfig_t configLower      = { .customPicture = &customPicture };
    const bool          isInitialRequest = false;

    const char *invlaidSize_lower =
        "{\"wasRead\":false,\"mode\":2,\"enableWeekends\":true,\"workTime\":10,\"idleTime\":5,"
        "\"picture\":{\"size\":-1,\"data\":[0,0,24,1567],"
        " \"colors\": { "
        "  \"main\": { \"r\": 0, \"g\": 100, \"b\": 200 },"
        "  \"secondary\":{ \"r\": 200, \"g\": 100, \"b\": 0 } } },"
        "\"workRange\":{\"from\": 5, \"to\": 11}}";

    pictureRow_t  expectedPictureData[MAX_CUSTOM_PICTURE_HEIGHT] = { 0, 0, 24, 1567 };
    PictureInfo_t expectedCustomPicture = { .picture = { .size = 0, .data = expectedPictureData },
                                            .colors  = { .main = { 0, 100, 200 }, .secondary = { 200, 100, 0 } } };

    WaterscreenConfig_t expectedConfig = { .mode = Mode_Service,
                                           .standardModeConfig =
                                               {
                                                   .isWorkingDuringWeekends = true,
                                                   .workTimeInStandardMode  = 10,
                                                   .idleTimeInStandardMode  = 5,
                                                   .workRange               = { .from = 5, .to = 11 },
                                               },
                                           .customPicture = &expectedCustomPicture };

    assert_int_equal( fromJsonToWaterscreenCfg( invlaidSize_lower, &configLower, isInitialRequest ), Http_Success );
    assert_true( compareConfigs( &expectedConfig, &configLower ) );

    WaterscreenConfig_t configHigher = { .customPicture = &customPicture };
    const char         *invlaidSize_higher =
        "{\"wasRead\":false,\"mode\":2,\"enableWeekends\":true,\"workTime\":10,\"idleTime\":5,"
        "\"picture\":{\"size\":130,\"data\":[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"
        "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"
        "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],"
        " \"colors\": { "
        "  \"main\": { \"r\": 0, \"g\": 100, \"b\": 200 },"
        "  \"secondary\":{ \"r\": 200, \"g\": 100, \"b\": 0 } "
        " } "
        "},"
        "\"workRange\":{\"from\": 5, \"to\": 11}}";


    expectedConfig.customPicture->picture.size = MAX_CUSTOM_PICTURE_HEIGHT;
    memset( expectedConfig.customPicture->picture.data, 0,
            expectedConfig.customPicture->picture.size * sizeof( *expectedConfig.customPicture->picture.data ) );

    assert_int_equal( fromJsonToWaterscreenCfg( invlaidSize_higher, &configHigher, isInitialRequest ), Http_Success );
    assert_true( compareConfigs( &expectedConfig, &configHigher ) );
}

void givenWorkHoursOutsideRange_configParser_returnSuccessAndClamp()
{
    const char *sampleJson = "{\"wasRead\":false,\"mode\":2,\"enableWeekends\":true,\"workTime\":10,\"idleTime\":5,"
                             "\"picture\":{\"size\":4,\"data\":[0,0,24,1567],"
                             " \"colors\": { "
                             "  \"main\": { \"r\": 0, \"g\": 100, \"b\": 200 },"
                             "  \"secondary\":{ \"r\": 200, \"g\": 100, \"b\": 0 } } },"
                             "\"workRange\":{\"from\": -1, \"to\": 24}}";

    pictureRow_t  expectedPictureData[MAX_CUSTOM_PICTURE_HEIGHT] = { 0, 0, 24, 1567 };
    PictureInfo_t expectedCustomPicture = { .picture = { .size = 4, .data = expectedPictureData },
                                            .colors  = { .main = { 0, 100, 200 }, .secondary = { 200, 100, 0 } } };

    const WaterscreenConfig_t expectedConfig   = { .mode = Mode_Service,
                                                   .standardModeConfig =
                                                       {
                                                           .isWorkingDuringWeekends = true,
                                                           .workTimeInStandardMode  = 10,
                                                           .idleTimeInStandardMode  = 5,
                                                           .workRange               = { .from = 0, .to = 24 },
                                                     },
                                                   .customPicture = &expectedCustomPicture };
    const bool                isInitialRequest = false;

    WaterscreenConfig_t config = { .customPicture = &customPicture };
    assert_int_equal( fromJsonToWaterscreenCfg( sampleJson, &config, isInitialRequest ), Http_Success );
    assert_true( compareConfigs( &expectedConfig, &config ) );
}

void givenInvlaidWorkRange_configParser_returnParsingError()
{
    const char *sampleJson =
        "{\"wasRead\":false,\"mode\":2,\"enableWeekends\":true,\"workTime\":10,\"idleTime\":5,\"picture\":{\"size\":"
        "4,\"data\":[0,0,24,1567]}, \"workRange\":{\"from\": 10, \"to\": 9}}";
    const bool isInitialRequest = false;

    WaterscreenConfig_t config = { .customPicture = &customPicture };
    assert_int_equal( fromJsonToWaterscreenCfg( sampleJson, &config, isInitialRequest ),
                      Http_WaterscreenConfigParsingError );
}

static int setupCustomPicture()
{
    customPicture.picture.size = 0;
    memset( customPicture.picture.data, 0, MAX_CUSTOM_PICTURE_HEIGHT * sizeof( *customPicture.picture.data ) );

    return 0;
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup( givenRawValidJson_configParser_fillConfigStructure, setupCustomPicture ),
        cmocka_unit_test_setup( givenInvlaidMode_configParser_returnParsingError, setupCustomPicture ),
        cmocka_unit_test_setup( givenArrayShorterThanSize_configParser_returnParsingError, setupCustomPicture ),
        cmocka_unit_test_setup( givenInvalidSize_configParser_returnSuccessAndClamp, setupCustomPicture ),
        cmocka_unit_test_setup( givenReadConfig_configParser_returnNoUpdate, setupCustomPicture ),
        cmocka_unit_test_setup( givenReadConfigAndInitialRequest_configParser_ignoreWasReadFlagAndParse,
                                setupCustomPicture ),
        cmocka_unit_test_setup( givenWorkHoursOutsideRange_configParser_returnSuccessAndClamp, setupCustomPicture ),
        cmocka_unit_test_setup( givenInvlaidWorkRange_configParser_returnParsingError, setupCustomPicture ),
    };

    return cmocka_run_group_tests_name( "JSON config parser", tests, NULL, NULL );
}
