/*
 * lcd.h
 *
 *  Created on: 12 gru 2021
 *      Author: daniel
 */

#ifndef LCD_H_
#define LCD_H_

#include "fsl_spi.h"
#include <stdint.h>


#define LCD_WIDTH       160
#define LCD_HEIGHT      128
#define LCD_FONT_HEIGHT 8
#define LCD_FONT_WIDTH  5
#define LCD_TXT_LINES   ( ( LCD_HEIGHT / LCD_FONT_HEIGHT ) - 1 ) // Describes how many lines of text can be printed

void LCD_Init( SPI_Type *base );
void LCD_GramRefresh( void );

void     LCD_Clear( uint16_t color );
void     LCD_Set_Bitmap( uint16_t *data );
void     LCD_Set_Icon( uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t *data );
void     LCD_Draw_Point( uint16_t x, uint16_t y, uint16_t color );
uint16_t LCD_Get_Point( uint16_t x, uint16_t y );
void     LCD_Draw_Line( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color );
void     LCD_Draw_DotLine( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color );
void     LCD_Draw_Rect( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color );
void     LCD_Draw_FillRect( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color );
void     LCD_Draw_Circle( uint16_t x, uint16_t y, uint16_t radius, uint16_t color );
void     LCD_Draw_Arc( uint16_t x, uint16_t y, uint16_t radius, uint16_t angle1, uint16_t angle2, uint16_t color );
void     LCD_Puts( uint16_t x, uint16_t y, char *text, uint16_t color );

void LCD_7seg( uint16_t x, uint16_t y, int32_t value, int8_t digits, uint16_t color );
void LCD_7dot( uint16_t x, uint16_t y, uint16_t color );

#endif /* LCD_H_ */
