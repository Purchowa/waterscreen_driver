#ifndef OLED_PANEL_H_
#define OLED_PANEL_H_

#include "external_communication/http_return_codes.h"
#include "waterscreen_state/waterscreen_state_context.h"
#include "config/waterscreen_config.h"


void drawInfoPanel( const WaterscreenContext_t *, const WaterscreenMode_t, const HttpReturnCodes_t,
                    bool isModeSelection );

#endif /* OLED_PANEL_H_ */
