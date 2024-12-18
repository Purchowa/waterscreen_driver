/*
 * lcd.c
 *
 *  Created on: 12 gru 2021
 *      Author: daniel
 */


#include "lcd.h"

#include "fsl_gpio.h"
#include "pin_mux.h"
#include <math.h>


#define LCD_RST_0                                                                                                      \
    GPIO_PinWrite( BOARD_INITLCDPINS_LCD_RST_GPIO, BOARD_INITLCDPINS_LCD_RST_PORT, BOARD_INITLCDPINS_LCD_RST_PIN, 0 )
#define LCD_RST_1                                                                                                      \
    GPIO_PinWrite( BOARD_INITLCDPINS_LCD_RST_GPIO, BOARD_INITLCDPINS_LCD_RST_PORT, BOARD_INITLCDPINS_LCD_RST_PIN, 1 )

#define LCD_DC_0                                                                                                       \
    GPIO_PinWrite( BOARD_INITLCDPINS_LCD_DC_GPIO, BOARD_INITLCDPINS_LCD_DC_PORT, BOARD_INITLCDPINS_LCD_DC_PIN, 0 )
#define LCD_DC_1                                                                                                       \
    GPIO_PinWrite( BOARD_INITLCDPINS_LCD_DC_GPIO, BOARD_INITLCDPINS_LCD_DC_PORT, BOARD_INITLCDPINS_LCD_DC_PIN, 1 )

#define PI 3.14159265358979f

static SPI_Type *SPI_base = NULL;

static uint16_t  frameBuffer[LCD_HEIGHT][LCD_WIDTH] = { 0 };
static uint16_t *linearFrameBuffer;

static const unsigned char fontSystem5x8[] = {

    0x00, 0x00, 0x00, 0x00, 0x00, /* Space */
    0x00, 0x00, 0x4f, 0x00, 0x00, /* ! */
    0x00, 0x07, 0x00, 0x07, 0x00, /* " */
    0x14, 0x7f, 0x14, 0x7f, 0x14, /* # */
    0x24, 0x2a, 0x7f, 0x2a, 0x12, /* 0x */
    0x23, 0x13, 0x08, 0x64, 0x62, /* % */
    0x36, 0x49, 0x55, 0x22, 0x20, /* & */
    0x00, 0x05, 0x03, 0x00, 0x00, /* ' */
    0x00, 0x1c, 0x22, 0x41, 0x00, /* ( */
    0x00, 0x41, 0x22, 0x1c, 0x00, /* ) */
    0x14, 0x08, 0x3e, 0x08, 0x14, /* // */
    0x08, 0x08, 0x3e, 0x08, 0x08, /* + */
    0x50, 0x30, 0x00, 0x00, 0x00, /* , */
    0x08, 0x08, 0x08, 0x08, 0x08, /* - */
    0x00, 0x60, 0x60, 0x00, 0x00, /* . */
    0x20, 0x10, 0x08, 0x04, 0x02, /* / */
    0x3e, 0x51, 0x49, 0x45, 0x3e, /* 0 */
    0x00, 0x42, 0x7f, 0x40, 0x00, /* 1 */
    0x42, 0x61, 0x51, 0x49, 0x46, /* 2 */
    0x21, 0x41, 0x45, 0x4b, 0x31, /* 3 */
    0x18, 0x14, 0x12, 0x7f, 0x10, /* 4 */
    0x27, 0x45, 0x45, 0x45, 0x39, /* 5 */
    0x3c, 0x4a, 0x49, 0x49, 0x30, /* 6 */
    0x07, 0x71, 0x09, 0x05, 0x03, /* 7 */
    0x36, 0x49, 0x49, 0x49, 0x36, /* 8 */
    0x06, 0x49, 0x49, 0x29, 0x1e, /* 9 */
    0x00, 0x36, 0x36, 0x00, 0x00, /* : */
    0x00, 0x56, 0x36, 0x00, 0x00, /* ; */
    0x08, 0x14, 0x22, 0x41, 0x00, /* < */
    0x14, 0x14, 0x14, 0x14, 0x14, /* = */
    0x00, 0x41, 0x22, 0x14, 0x08, /* > */
    0x02, 0x01, 0x51, 0x09, 0x06, /* ? */
    0x3e, 0x41, 0x5d, 0x55, 0x1e, /* @ */
    0x7e, 0x11, 0x11, 0x11, 0x7e, /* A */
    0x7f, 0x49, 0x49, 0x49, 0x36, /* B */
    0x3e, 0x41, 0x41, 0x41, 0x22, /* C */
    0x7f, 0x41, 0x41, 0x22, 0x1c, /* D */
    0x7f, 0x49, 0x49, 0x49, 0x41, /* E */
    0x7f, 0x09, 0x09, 0x09, 0x01, /* F */
    0x3e, 0x41, 0x49, 0x49, 0x7a, /* G */
    0x7f, 0x08, 0x08, 0x08, 0x7f, /* H */
    0x00, 0x41, 0x7f, 0x41, 0x00, /* I */
    0x20, 0x40, 0x41, 0x3f, 0x01, /* J */
    0x7f, 0x08, 0x14, 0x22, 0x41, /* K */
    0x7f, 0x40, 0x40, 0x40, 0x40, /* L */
    0x7f, 0x02, 0x0c, 0x02, 0x7f, /* M */
    0x7f, 0x04, 0x08, 0x10, 0x7f, /* N */
    0x3e, 0x41, 0x41, 0x41, 0x3e, /* O */
    0x7f, 0x09, 0x09, 0x09, 0x06, /* P */
    0x3e, 0x41, 0x51, 0x21, 0x5e, /* Q */
    0x7f, 0x09, 0x19, 0x29, 0x46, /* R */
    0x26, 0x49, 0x49, 0x49, 0x32, /* S */
    0x01, 0x01, 0x7f, 0x01, 0x01, /* T */
    0x3f, 0x40, 0x40, 0x40, 0x3f, /* U */
    0x1f, 0x20, 0x40, 0x20, 0x1f, /* V */
    0x3f, 0x40, 0x38, 0x40, 0x3f, /* W */
    0x63, 0x14, 0x08, 0x14, 0x63, /* X */
    0x07, 0x08, 0x70, 0x08, 0x07, /* Y */
    0x61, 0x51, 0x49, 0x45, 0x43, /* Z */
    0x00, 0x7f, 0x41, 0x41, 0x00, /* [ */
    0x02, 0x04, 0x08, 0x10, 0x20, /* \ */
    0x00, 0x41, 0x41, 0x7f, 0x00, /* ] */
    0x04, 0x02, 0x01, 0x02, 0x04, /* ^ */
    0x40, 0x40, 0x40, 0x40, 0x40, /* _ */
    0x00, 0x00, 0x03, 0x05, 0x00, /* ` */
    0x20, 0x54, 0x54, 0x54, 0x78, /* a */
    0x7F, 0x44, 0x44, 0x44, 0x38, /* b */
    0x38, 0x44, 0x44, 0x44, 0x44, /* c */
    0x38, 0x44, 0x44, 0x44, 0x7f, /* d */
    0x38, 0x54, 0x54, 0x54, 0x18, /* e */
    0x04, 0x04, 0x7e, 0x05, 0x05, /* f */
    0x08, 0x54, 0x54, 0x54, 0x3c, /* g */
    0x7f, 0x08, 0x04, 0x04, 0x78, /* h */
    0x00, 0x44, 0x7d, 0x40, 0x00, /* i */
    0x20, 0x40, 0x44, 0x3d, 0x00, /* j */
    0x7f, 0x10, 0x28, 0x44, 0x00, /* k */
    0x00, 0x41, 0x7f, 0x40, 0x00, /* l */
    0x7c, 0x04, 0x7c, 0x04, 0x78, /* m */
    0x7c, 0x08, 0x04, 0x04, 0x78, /* n */
    0x38, 0x44, 0x44, 0x44, 0x38, /* o */
    0x7c, 0x14, 0x14, 0x14, 0x08, /* p */
    0x08, 0x14, 0x14, 0x14, 0x7c, /* q */
    0x7c, 0x08, 0x04, 0x04, 0x00, /* r */
    0x48, 0x54, 0x54, 0x54, 0x24, /* s */
    0x04, 0x04, 0x3f, 0x44, 0x44, /* t */
    0x3c, 0x40, 0x40, 0x20, 0x7c, /* u */
    0x1c, 0x20, 0x40, 0x20, 0x1c, /* v */
    0x3c, 0x40, 0x30, 0x40, 0x3c, /* w */
    0x44, 0x28, 0x10, 0x28, 0x44, /* x */
    0x0c, 0x50, 0x50, 0x50, 0x3c, /* y */
    0x44, 0x64, 0x54, 0x4c, 0x44, /* z */
    0x08, 0x36, 0x41, 0x41, 0x00, /* { */
    0x00, 0x00, 0x77, 0x00, 0x00, /* | */
    0x00, 0x41, 0x41, 0x36, 0x08, /* } */
    0x08, 0x08, 0x2a, 0x1c, 0x08, /* <- */
    0x08, 0x1c, 0x2a, 0x08, 0x08, /* -> */
    0xff, 0xff, 0xff, 0xff, 0xff, /*  */
    0x3e, 0x41, 0x47, 0x43, 0x3e, /* Ó */
    0x38, 0x44, 0x46, 0x45, 0x38, /* ó */

};
// 97
static const unsigned char fontSevenSeg32x24[] = {

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x7F, 0xFE, 0x0F, 0xF0, 0x7F, 0xFE, 0x0F, 0xEC, 0x3F, 0xFC,
    0x37, 0xDC, 0x1F, 0xF8, 0x3B, 0x3C, 0x00, 0x00, 0x3C, 0x3C, 0x00, 0x00, 0x3C, 0x3C, 0x00, 0x00, 0x3C, 0x3C, 0x00,
    0x00, 0x3C, 0x3C, 0x00, 0x00, 0x3C, 0x3C, 0x00, 0x00, 0x3C, 0x3C, 0x00, 0x00, 0x3C, 0x3C, 0x00, 0x00, 0x3C, 0x3C,
    0x00, 0x00, 0x3C, 0x3C, 0x00, 0x00, 0x3C, 0x3C, 0x00, 0x00, 0x3C, 0x3C, 0x00, 0x00, 0x3C, 0xDC, 0x1F, 0xF8, 0x3B,
    0xEC, 0x3F, 0xFC, 0x37, 0xF0, 0x7F, 0xFE, 0x0F, 0xF0, 0x7F, 0xFE, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x1F, 0xF8,
    0x03, 0xE0, 0x3F, 0xFC, 0x07, 0xF0, 0x7F, 0xFE, 0x0F, 0xF0, 0x7F, 0xFE, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x0F, 0x00, 0x00, 0xFE, 0x0F, 0x0C,
    0x80, 0xFD, 0x37, 0x1C, 0xC0, 0xFB, 0x3B, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C,
    0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03,
    0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0xDC, 0xDF,
    0x03, 0x38, 0xEC, 0xBF, 0x01, 0x30, 0xF0, 0x7F, 0x00, 0x00, 0xF0, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0C, 0x80, 0x01, 0x30, 0x1C, 0xC0, 0x03, 0x38, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03,
    0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0,
    0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0xDC,
    0xDF, 0xFB, 0x3B, 0xEC, 0xBF, 0xFD, 0x37, 0xF0, 0x7F, 0xFE, 0x0F, 0xF0, 0x7F, 0xFE, 0x0F, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x7F, 0x00, 0x00, 0xF0, 0x7F, 0x00,
    0x00, 0xE0, 0xBF, 0x01, 0x00, 0xC0, 0xDF, 0x03, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0xC0,
    0x03, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00,
    0xC0, 0x03, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0xC0, 0x03, 0x00,
    0xC0, 0xDF, 0xFB, 0x03, 0xE0, 0xBF, 0xFD, 0x07, 0xF0, 0x7F, 0xFE, 0x0F, 0xF0, 0x7F, 0xFE, 0x0F, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x7F, 0x00, 0x00, 0xF0, 0x7F,
    0x00, 0x00, 0xEC, 0xBF, 0x01, 0x30, 0xDC, 0xDF, 0x03, 0x38, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C,
    0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C,
    0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03,
    0x3C, 0x1C, 0xC0, 0xFB, 0x3B, 0x0C, 0x80, 0xFD, 0x37, 0x00, 0x00, 0xFE, 0x0F, 0x00, 0x00, 0xFE, 0x0F, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x7F, 0xFE, 0x0F, 0xF0,
    0x7F, 0xFE, 0x0F, 0xEC, 0xBF, 0xFD, 0x37, 0xDC, 0xDF, 0xFB, 0x3B, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C,
    0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03,
    0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0,
    0x03, 0x3C, 0x1C, 0xC0, 0xFB, 0x3B, 0x0C, 0x80, 0xFD, 0x37, 0x00, 0x00, 0xFE, 0x0F, 0x00, 0x00, 0xFE, 0x0F, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00,
    0x00, 0x3C, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x3C, 0x00,
    0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x3C,
    0x00, 0x00, 0x00, 0xDC, 0x1F, 0xF8, 0x03, 0xEC, 0x3F, 0xFC, 0x07, 0xF0, 0x7F, 0xFE, 0x0F, 0xF0, 0x7F, 0xFE, 0x0F,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x7F, 0xFE,
    0x0F, 0xF0, 0x7F, 0xFE, 0x0F, 0xEC, 0xBF, 0xFD, 0x37, 0xDC, 0xDF, 0xFB, 0x3B, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0,
    0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C,
    0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C,
    0x3C, 0xC0, 0x03, 0x3C, 0xDC, 0xDF, 0xFB, 0x3B, 0xEC, 0xBF, 0xFD, 0x37, 0xF0, 0x7F, 0xFE, 0x0F, 0xF0, 0x7F, 0xFE,
    0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x7F,
    0x00, 0x00, 0xF0, 0x7F, 0x00, 0x00, 0xEC, 0xBF, 0x01, 0x30, 0xDC, 0xDF, 0x03, 0x38, 0x3C, 0xC0, 0x03, 0x3C, 0x3C,
    0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C,
    0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0x3C, 0xC0, 0x03,
    0x3C, 0x3C, 0xC0, 0x03, 0x3C, 0xDC, 0xDF, 0xFB, 0x3B, 0xEC, 0xBF, 0xFD, 0x37, 0xF0, 0x7F, 0xFE, 0x0F, 0xF0, 0x7F,
    0xFE, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0xC0, 0x03, 0x00,
    0x00, 0xC0, 0x03, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0xC0, 0x03,
    0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0xC0,
    0x03, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static const unsigned char dots[] = {

    0x00, 0x18, 0x18, 0x00, 0x00, 0x3C, 0x3C, 0x00, 0x00, 0x3C, 0x3C, 0x00, 0x00, 0x18, 0x18, 0x00 };

void Driver_Delay_ms( uint32_t xms )
{

    for ( volatile int j = xms * 10000; j > 0; j-- )
        ;
}

static void SPI_Write_Byte( SPI_Type *base, uint16_t data )
{

    uint32_t      control = 0U;
    spi_config_t *g_config;

    uint32_t configFlags = kSPI_FrameAssert;
    g_config             = SPI_GetConfig( base );
    /* set data width */
    control |= (uint32_t)SPI_FIFOWR_LEN( kSPI_Data8Bits );
    /* set sssel */
    control |= ( SPI_DEASSERT_ALL & ( ~SPI_DEASSERTNUM_SSEL( (uint32_t)( g_config->sselNum ) ) ) );
    /* mask configFlags */
    control |= ( configFlags & (uint32_t)SPI_FIFOWR_FLAGS_MASK );
    /* ignore RX */
    control |= ( 1 << 22 );

    while ( !( base->FIFOSTAT & kSPI_TxNotFullFlag ) )
        ;
    base->FIFOWR = data | control;
}

static void SPI_Write_Words( SPI_Type *base, uint16_t *data, uint32_t size )
{

    uint32_t      control = 0U;
    spi_config_t *g_config;
    uint32_t      i = 0;

    uint32_t configFlags = 0;
    g_config             = SPI_GetConfig( base );
    /* set data width */
    control |= (uint32_t)SPI_FIFOWR_LEN( kSPI_Data16Bits );
    /* set sssel */
    control |= ( SPI_DEASSERT_ALL & ( ~SPI_DEASSERTNUM_SSEL( (uint32_t)( g_config->sselNum ) ) ) );
    /* mask configFlags */
    control |= ( configFlags & (uint32_t)SPI_FIFOWR_FLAGS_MASK );
    /* ignore RX */
    control |= ( 1 << 22 );

    for ( i = 0; i < size - 1; i++ )
    {

        while ( !( base->FIFOSTAT & kSPI_TxNotFullFlag ) )
            ;
        base->FIFOWR = data[i] | control;
    }
    configFlags = kSPI_FrameAssert;
    control |= ( configFlags & (uint32_t)SPI_FIFOWR_FLAGS_MASK );
    while ( !( base->FIFOSTAT & kSPI_TxNotFullFlag ) )
        ;
    base->FIFOWR = data[i] | control;
}

static void LCD_WriteReg( uint8_t Reg )
{

    LCD_DC_0;
    SPI_Write_Byte( SPI_base, Reg );
    for ( volatile int i = 0; i < 10; i++ )
        ;
}

static void LCD_WriteData_8Bit( uint8_t Data )
{

    LCD_DC_1;
    SPI_Write_Byte( SPI_base, Data );
    for ( volatile int i = 0; i < 10; i++ )
        ;
}

static void LCD_InitReg( void )
{

    // ST7735R Frame Rate
    LCD_WriteReg( 0xB1 );
    LCD_WriteData_8Bit( 0x01 );
    LCD_WriteData_8Bit( 0x2C );
    LCD_WriteData_8Bit( 0x2D );

    LCD_WriteReg( 0xB2 );
    LCD_WriteData_8Bit( 0x01 );
    LCD_WriteData_8Bit( 0x2C );
    LCD_WriteData_8Bit( 0x2D );

    LCD_WriteReg( 0xB3 );
    LCD_WriteData_8Bit( 0x01 );
    LCD_WriteData_8Bit( 0x2C );
    LCD_WriteData_8Bit( 0x2D );
    LCD_WriteData_8Bit( 0x01 );
    LCD_WriteData_8Bit( 0x2C );
    LCD_WriteData_8Bit( 0x2D );

    LCD_WriteReg( 0xB4 ); // Column inversion
    LCD_WriteData_8Bit( 0x07 );

    // ST7735R Power Sequence
    LCD_WriteReg( 0xC0 );
    LCD_WriteData_8Bit( 0xA2 );
    LCD_WriteData_8Bit( 0x02 );
    LCD_WriteData_8Bit( 0x84 );
    LCD_WriteReg( 0xC1 );
    LCD_WriteData_8Bit( 0xC5 );

    LCD_WriteReg( 0xC2 );
    LCD_WriteData_8Bit( 0x0A );
    LCD_WriteData_8Bit( 0x00 );

    LCD_WriteReg( 0xC3 );
    LCD_WriteData_8Bit( 0x8A );
    LCD_WriteData_8Bit( 0x2A );
    LCD_WriteReg( 0xC4 );
    LCD_WriteData_8Bit( 0x8A );
    LCD_WriteData_8Bit( 0xEE );

    LCD_WriteReg( 0xC5 ); // VCOM
    LCD_WriteData_8Bit( 0x0E );

    // ST7735R Gamma Sequence
    LCD_WriteReg( 0xE0 );
    LCD_WriteData_8Bit( 0x0f );
    LCD_WriteData_8Bit( 0x1a );
    LCD_WriteData_8Bit( 0x0f );
    LCD_WriteData_8Bit( 0x18 );
    LCD_WriteData_8Bit( 0x2f );
    LCD_WriteData_8Bit( 0x28 );
    LCD_WriteData_8Bit( 0x20 );
    LCD_WriteData_8Bit( 0x22 );
    LCD_WriteData_8Bit( 0x1f );
    LCD_WriteData_8Bit( 0x1b );
    LCD_WriteData_8Bit( 0x23 );
    LCD_WriteData_8Bit( 0x37 );
    LCD_WriteData_8Bit( 0x00 );
    LCD_WriteData_8Bit( 0x07 );
    LCD_WriteData_8Bit( 0x02 );
    LCD_WriteData_8Bit( 0x10 );

    LCD_WriteReg( 0xE1 );
    LCD_WriteData_8Bit( 0x0f );
    LCD_WriteData_8Bit( 0x1b );
    LCD_WriteData_8Bit( 0x0f );
    LCD_WriteData_8Bit( 0x17 );
    LCD_WriteData_8Bit( 0x33 );
    LCD_WriteData_8Bit( 0x2c );
    LCD_WriteData_8Bit( 0x29 );
    LCD_WriteData_8Bit( 0x2e );
    LCD_WriteData_8Bit( 0x30 );
    LCD_WriteData_8Bit( 0x30 );
    LCD_WriteData_8Bit( 0x39 );
    LCD_WriteData_8Bit( 0x3f );
    LCD_WriteData_8Bit( 0x00 );
    LCD_WriteData_8Bit( 0x07 );
    LCD_WriteData_8Bit( 0x03 );
    LCD_WriteData_8Bit( 0x10 );

    LCD_WriteReg( 0xF0 );       // Enable test command
    LCD_WriteData_8Bit( 0x01 );

    LCD_WriteReg( 0xF6 );       // Disable ram power save mode
    LCD_WriteData_8Bit( 0x00 );

    LCD_WriteReg( 0x3A );       // 65k mode
    LCD_WriteData_8Bit( 0x05 );

    LCD_WriteReg( 0x36 );       // MX, MY, RGB mode
    LCD_WriteData_8Bit( 0x60 ); // RGB color filter panel
}

static void LCD_SetHome( void )
{

    // set the X coordinates
    LCD_WriteReg( 0x2A );
    LCD_WriteData_8Bit( 0x00 );
    LCD_WriteData_8Bit( 0x01 );
    LCD_WriteData_8Bit( 0x00 );
    LCD_WriteData_8Bit( LCD_WIDTH );
    // set the Y coordinates
    LCD_WriteReg( 0x2B );
    LCD_WriteData_8Bit( 0x00 );
    LCD_WriteData_8Bit( 0x02 );
    LCD_WriteData_8Bit( 0x00 );
    LCD_WriteData_8Bit( LCD_HEIGHT + 1 );

    LCD_WriteReg( 0x2C );
}

void LCD_GramRefresh()
{

    LCD_SetHome();
    LCD_DC_1;
    SPI_Write_Words( SPI_base, linearFrameBuffer, LCD_WIDTH * LCD_HEIGHT );
}

void LCD_Init( SPI_Type *base )
{

    SPI_base          = base;
    linearFrameBuffer = (uint16_t *)frameBuffer;
    // Hardware reset
    LCD_RST_1;
    Driver_Delay_ms( 10 );
    LCD_RST_0;
    Driver_Delay_ms( 10 );
    LCD_RST_1;
    Driver_Delay_ms( 10 );
    // Set the initialization register
    LCD_InitReg();
    Driver_Delay_ms( 20 );
    // Turn off sleep mode
    LCD_WriteReg( 0x11 );
    Driver_Delay_ms( 20 );
    // Turn on the LCD display
    LCD_WriteReg( 0x29 );

    LCD_Clear( 0xFFFF );
    LCD_GramRefresh();
}

void LCD_Clear( uint16_t color )
{

    for ( int i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++ )
    {

        linearFrameBuffer[i] = color;
    }
}

void LCD_Set_Bitmap( uint16_t *data )
{

    for ( int i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++ )
    {

        linearFrameBuffer[i] = data[i];
    }
}

void LCD_Set_Icon( uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t *data )
{

    for ( int i = 0; i < w; i++ )
    {
        for ( int j = 0; j < h; j++ )
        {
            // if(data[i+(j*w)]!=0x0841)
            frameBuffer[y + j][x + i] = data[i + ( j * w )];
        }
    }
}

void LCD_Draw_Point( uint16_t x, uint16_t y, uint16_t color )
{

    if ( x >= LCD_WIDTH )
    {
        return;
    }

    if ( y >= LCD_HEIGHT )
    {
        return;
    }
    frameBuffer[y][x] = color;
}

uint16_t LCD_Get_Point( uint16_t x, uint16_t y )
{

    if ( x >= LCD_WIDTH )
    {
        x = LCD_WIDTH - 1;
    }

    if ( y >= LCD_HEIGHT )
    {
        y = LCD_HEIGHT - 1;
    }
    return frameBuffer[y][x];
}

void LCD_Draw_Line( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color )
{

    uint8_t x, y;
    int16_t addx, addy, dx, dy, P;

    dx   = abs( x2 - x1 );
    dy   = abs( y2 - y1 );
    x    = x1;
    y    = y1;
    addx = ( x1 > x2 ) ? -1 : 1;
    addy = ( y1 > y2 ) ? -1 : 1;

    if ( dx >= dy )
    {

        P = 2 * dy - dx;
        for ( int16_t i = 0; i <= dx; i++ )
        {
            LCD_Draw_Point( x, y, color );

            if ( P < 0 )
            {
                P += 2 * dy;
                x += addx;
            }
            else
            {
                P += 2 * ( dy - dx );
                x += addx;
                y += addy;
            }
        }
    }
    else
    {

        P = 2 * dx - dy;
        for ( int16_t i = 0; i <= dy; i++ )
        {
            LCD_Draw_Point( x, y, color );
            if ( P < 0 )
            {

                P += 2 * dx;
                y += addy;
            }
            else
            {

                P += 2 * ( dx - dy );
                x += addx;
                y += addy;
            }
        }
    }
}

void LCD_Draw_DotLine( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color )
{

    uint8_t x, y;
    int16_t addx, addy, dx, dy, P;

    dx   = abs( x2 - x1 );
    dy   = abs( y2 - y1 );
    x    = x1;
    y    = y1;
    addx = ( x1 > x2 ) ? -1 : 1;
    addy = ( y1 > y2 ) ? -1 : 1;

    if ( dx >= dy )
    {

        P = 2 * dy - dx;
        for ( int16_t i = 0; i <= dx; i++ )
        {
            LCD_Draw_Point( x, y, color * ( i % 2 ) );

            if ( P < 0 )
            {
                P += 2 * dy;
                x += addx;
            }
            else
            {
                P += 2 * ( dy - dx );
                x += addx;
                y += addy;
            }
        }
    }
    else
    {

        P = 2 * dx - dy;
        for ( int16_t i = 0; i <= dy; i++ )
        {
            LCD_Draw_Point( x, y, color * ( i % 2 ) );
            if ( P < 0 )
            {

                P += 2 * dx;
                y += addy;
            }
            else
            {

                P += 2 * ( dx - dy );
                x += addx;
                y += addy;
            }
        }
    }
}

void LCD_Draw_Rect( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color )
{

    LCD_Draw_Line( x1, y1, x2, y1, color );
    LCD_Draw_Line( x1, y2, x2, y2, color );
    LCD_Draw_Line( x1, y1, x1, y2, color );
    LCD_Draw_Line( x2, y1, x2, y2, color );
}

void LCD_Draw_FillRect( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color )
{

    for ( uint16_t i = x1; i <= x2; i++ )
    {
        for ( uint16_t j = y1; j <= y2; j++ )
        {

            LCD_Draw_Point( i, j, color );
        }
    }
}

void LCD_Draw_Circle( uint16_t x, uint16_t y, uint16_t radius, uint16_t color )
{

    int16_t x1  = radius - 1;
    int16_t y1  = 0;
    int16_t dx  = 1;
    int16_t dy  = 1;
    int16_t err = dx - ( radius << 1 );

    while ( x1 >= y1 )
    {

        LCD_Draw_Point( x + x1, y + y1, color );
        LCD_Draw_Point( x + y1, y + x1, color );
        LCD_Draw_Point( x - y1, y + x1, color );
        LCD_Draw_Point( x - x1, y + y1, color );
        LCD_Draw_Point( x - x1, y - y1, color );
        LCD_Draw_Point( x - y1, y - x1, color );
        LCD_Draw_Point( x + y1, y - x1, color );
        LCD_Draw_Point( x + x1, y - y1, color );

        if ( err <= 0 )
        {

            y1++;
            err += dy;
            dy += 2;
        }
        else
        {

            x1--;
            dx += 2;
            err += dx - ( radius << 1 );
        }
    }
}

void LCD_Draw_Arc( uint16_t x, uint16_t y, uint16_t radius, uint16_t angle1, uint16_t angle2, uint16_t color )
{

    float p;
    for ( int i = angle1; i < angle2; i++ )
    {

        p = ( 2 * PI * i ) / 360.0;
        LCD_Draw_Point( x + radius * sinf( p ), y - radius * cosf( p ), color );
    }
}

void LCD_Puts( uint16_t x, uint16_t y, char *text, uint16_t color )
{

    uint32_t n = 0, m = 0;
    ;
    char c_offset;

    while ( text[n] )
    {

        c_offset = text[n] - ' ';
        //------ UTF-8 polskie znaki ------
        if ( text[n] == 0xC3 && text[n + 1] == 0xB3 )
        {
            // Ó
            c_offset = 98;
            n++;
        }
        if ( text[n] == 0xC3 && text[n + 1] == 0x93 )
        {
            // ó
            c_offset = 97;
            n++;
        }
        // --------------------------
        for ( int j = 0; j < 5; j++ )
        {
            for ( int i = 0; i < 8; i++ )
            {

                if ( fontSystem5x8[j + ( c_offset * 5 )] & ( 1 << i ) )
                    LCD_Draw_Point( ( m * 6 ) + x + j, y + i, color );
            }
        }
        n++;
        m++;
    }
}

void LCD_7seg( uint16_t x, uint16_t y, int32_t value, int8_t digits, uint16_t color )
{

    int8_t disp, neg = 1;

    if ( value < 0 )
    {
        value = -value;
        neg   = 0;
    }

    for ( int k = digits - 1; k >= neg; k-- )
    {
        disp = value % 10;
        value /= 10;
        if ( neg == 0 && k == 0 )
        {
            disp = 10;
        }
        for ( int i = 0; i < 24; i++ )
        {
            for ( int j = 0; j < 4; j++ )
            {
                for ( int n = 0; n < 8; n++ )
                {

                    if ( fontSevenSeg32x24[96 * disp + ( 4 * i ) + j] & ( 1 << n ) )
                        LCD_Draw_Point( x + ( 24 * k ) + i, y + ( 8 * j ) + n, color );
                }
            }
        }
    }
}

void LCD_7dot( uint16_t x, uint16_t y, uint16_t color )
{

    for ( int i = 0; i < 4; i++ )
    {
        for ( int j = 0; j < 4; j++ )
        {
            for ( int n = 0; n < 8; n++ )
            {
                if ( dots[( 4 * i ) + j] & ( 1 << n ) )
                    LCD_Draw_Point( x + i, y + ( 8 * j ) + n, color );
            }
        }
    }
}
