#ifndef LCD_PANEL_H_
#define LCD_PANEL_H_

#include "http/http_return_codes.h"
#include "waterscreen_state/waterscreen_state_context.h"
#include "config/waterscreen_config.h"


void drawInfoPanel( const WaterscreenContext_t *, const WaterscreenMode_t, const HttpReturnCodes_t,
                    bool isModeSelection );

#endif /* LCD_PANEL_H_ */
