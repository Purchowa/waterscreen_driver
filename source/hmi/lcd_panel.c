#include "lcd_panel.h"

#include "waterscreen_state/waterscreen_states.h"
#include "waterscreen_state/state_utils.h"
#include "datetime/rtc_provider.h"
#include "ble/ble_receiver.h"
#include "gpio/power_control.h"

#include "external/lcd/lcd.h"
#include <stdio.h>


#define TEXT_BUFFER_SIZE       LCD_WIDTH
#define SCREEN_SECOND_HALF_POS ( LCD_WIDTH / 2 )
#define MARGIN                 2

#define LAST_LINE  ( LCD_FONT_HEIGHT * LCD_TXT_LINES )
#define FIRST_LINE 0

#define LINE_OFFSET ( LCD_FONT_HEIGHT + LCD_FONT_HEIGHT / 2 )

typedef enum
{
    Screen_BGColor        = 0x3044,
    Screen_FontColor      = 0xffff,
    Screen_SelectionColor = 0x7ec6
} ScreenColors_t;

static char textBuffer[TEXT_BUFFER_SIZE] = {};

static size_t getTextMiddleXPos( size_t txtLen )
{
    return ( txtLen * LCD_FONT_WIDTH ) / 2;
}

static void drawHeader( const WaterscreenMode_t mode, bool isModeSelection )
{
    const char *modeStrFormat = isModeSelection ? "> %s <" : "  %s  ";

    snprintf( textBuffer, sizeof( textBuffer ), modeStrFormat, getModeName( mode ) );

    LCD_Puts( LCD_WIDTH / 2 - getTextMiddleXPos( strlen( textBuffer ) ), FIRST_LINE + MARGIN, textBuffer,
              isModeSelection ? Screen_SelectionColor : Screen_FontColor );

    LCD_Draw_Line( 0, LINE_OFFSET, LCD_WIDTH, LINE_OFFSET, Screen_FontColor );
}

static void drawFooter()
{
    const Datetime_t datetime = getRTCDatetime();

    LCD_Draw_Line( 0, LCD_HEIGHT - LINE_OFFSET, LCD_WIDTH, LCD_HEIGHT - LINE_OFFSET, Screen_FontColor );

    snprintf( textBuffer, sizeof( textBuffer ), "%u:%u:%u", datetime.time.hour, datetime.time.minute,
              datetime.time.second );
    LCD_Puts( MARGIN, LAST_LINE, textBuffer, Screen_FontColor );

    snprintf( textBuffer, sizeof( textBuffer ), "%u.%u.%u", datetime.date.day, datetime.date.month,
              datetime.date.year );
    LCD_Puts( SCREEN_SECOND_HALF_POS + 20 - MARGIN, LAST_LINE, textBuffer, Screen_FontColor );
}

static void drawInfoLineStr( const char *title, const char *str, const uint8_t strPos )
{
    snprintf( textBuffer, sizeof( textBuffer ), "%s: %s", title, str );
    LCD_Puts( MARGIN, strPos, textBuffer, Screen_FontColor );
}

#define HTTP_CODES_BUFFER_SIZE 2
#define HTTP_CODE_STR_LEN      32

static const char *getHttpCodesStr( const HttpReturnCodes_t newCode )
{
    static HttpReturnCodes_t httpCodes[HTTP_CODES_BUFFER_SIZE] = { Http_UnknownError, Http_UnknownError };
    static char              httpStrBuffer[LCD_WIDTH]          = { 0 };

    if ( httpCodes[0] != newCode )
    {
        memmove( &httpCodes[1], httpCodes, ( HTTP_CODES_BUFFER_SIZE - 1 ) * sizeof( *httpCodes ) );
        httpCodes[0] = newCode;
    }

    snprintf( httpStrBuffer, sizeof( httpStrBuffer ), "%s,%s", getHttpReturnCodeName( httpCodes[0] ),
              getHttpReturnCodeName( httpCodes[1] ) );

    return httpStrBuffer;
}

void drawInfoPanel( const WaterscreenContext_t *context, const WaterscreenMode_t mode, const HttpReturnCodes_t httpCode,
                    bool isModeSelection )
{
    LCD_Clear( Screen_BGColor );

    drawHeader( mode, isModeSelection ); // takes two lines

    drawInfoLineStr( "State", getStateName( context->waterscreenStateHandler ), 2 * LCD_FONT_HEIGHT );
    drawInfoLineStr( "ValvePower", getDeviceStateStr( getValvePowerState() ), 3 * LCD_FONT_HEIGHT + MARGIN );
    drawInfoLineStr( "WaterPump", getDeviceStateStr( getWaterPumpState() ), 4 * LCD_FONT_HEIGHT + 2 * MARGIN );
    drawInfoLineStr( "BLELogging", isLoggingActive() ? "active" : "inactive", 5 * LCD_FONT_HEIGHT + 3 * MARGIN );

    static const char *httpCodesTitle = "HttpCodes";
    LCD_Puts( LCD_WIDTH / 2 - getTextMiddleXPos( strlen( httpCodesTitle ) ), LCD_FONT_HEIGHT * 11, "HttpCodes",
              Screen_FontColor );
    LCD_Draw_DotLine( 0, LCD_FONT_HEIGHT * 12, LCD_WIDTH, LCD_FONT_HEIGHT * 12, Screen_FontColor );
    LCD_Puts( 0, LCD_FONT_HEIGHT * 13, (char *)getHttpCodesStr( httpCode ), Screen_FontColor );

    drawFooter();

    LCD_GramRefresh();
}
