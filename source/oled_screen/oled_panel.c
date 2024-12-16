#include "oled_panel.h"

#include "waterscreen_state/waterscreen_states.h"
#include "waterscreen_state/state_utils.h"
#include "datetime/rtc_provider.h"
#include "external/oled/oled.h"

#include <stdio.h>


#define TEXT_BUFFER_SIZE       OLED_WIDTH
#define SCREEN_SECOND_HALF_POS ( OLED_WIDTH / 2 )
#define HORIZONTAL_MARGIN      2

#define OLED_LAST_PAGE  ( OLED_PAGES - 1 )
#define OLED_FIRST_PAGE 0

#define LINE_OFFSET ( OLED_PAGE_HEIGHT + OLED_PAGE_HEIGHT / 2 )

typedef enum
{
    OLED_Black,
    OLED_Color
} OledColors_t;

static char textBuffer[TEXT_BUFFER_SIZE] = {};

static void drawHeader( const WaterscreenMode_t mode, bool isModeSelection )
{
    static const uint8_t xOffset       = 34;
    const char          *modeStrFormat = isModeSelection ? "> %s <" : "  %s  ";

    snprintf( textBuffer, sizeof( textBuffer ), modeStrFormat, getModeName( mode ) );

    OLED_Puts( OLED_WIDTH / 2 - xOffset, OLED_FIRST_PAGE, textBuffer );

    OLED_Draw_Line( 0, LINE_OFFSET, OLED_WIDTH, LINE_OFFSET );
}

static void drawFooter()
{
    const Datetime_t datetime = getRTCDatetime();

    OLED_Draw_Line( 0, OLED_HEIGHT - LINE_OFFSET, OLED_WIDTH, OLED_HEIGHT - LINE_OFFSET );

    snprintf( textBuffer, sizeof( textBuffer ), "%u:%u:%u", datetime.time.hour, datetime.time.minute,
              datetime.time.second );
    OLED_Puts( 0, OLED_LAST_PAGE, textBuffer );

    snprintf( textBuffer, sizeof( textBuffer ), "%u.%u.%u", datetime.date.day, datetime.date.month,
              datetime.date.year );
    OLED_Puts( SCREEN_SECOND_HALF_POS + HORIZONTAL_MARGIN, OLED_LAST_PAGE, textBuffer );
}

static void drawInfoLineStr( const char *title, const char *str, const uint8_t lineNumber )
{
    snprintf( textBuffer, sizeof( textBuffer ), "%s: %s", title, str );
    OLED_Puts( HORIZONTAL_MARGIN, lineNumber, textBuffer );
}

#define HTTP_CODES_BUFFER_SIZE 4
#define HTTP_CODE_STR_LEN      32

static const char *getHttpCodesStr( const HttpReturnCodes_t newCode )
{
    static HttpReturnCodes_t httpCodes[HTTP_CODES_BUFFER_SIZE] = { Http_UnknownError, Http_UnknownError,
                                                                   Http_UnknownError, Http_UnknownError };
    static char              httpStrBuffer[HTTP_CODE_STR_LEN]  = { 0 };

    if ( httpCodes[0] != newCode )
    {
        memmove( &httpCodes[1], httpCodes, ( HTTP_CODES_BUFFER_SIZE - 1 ) * sizeof( *httpCodes ) );
        httpCodes[0] = newCode;
    }

    snprintf( httpStrBuffer, sizeof( httpStrBuffer ), "%d,%d,%d,%d", httpCodes[0], httpCodes[1], httpCodes[2],
              httpCodes[3] );

    return httpStrBuffer;
}

void drawInfoPanel( const WaterscreenContext_t *context, const WaterscreenMode_t mode, const HttpReturnCodes_t httpCode,
                    bool isModeSelection )
{
    OLED_Clear_Screen( OLED_Black );

    const uint8_t bodyLineStart = 2;

    drawHeader( mode, isModeSelection );

    drawInfoLineStr( "State", getStateName( context->waterscreenStateHandler ), bodyLineStart );
    drawInfoLineStr( "HttpCodes", getHttpCodesStr( httpCode ), bodyLineStart + 1 );

    drawFooter();

    OLED_Refresh_Gram();
}
