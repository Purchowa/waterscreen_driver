#ifndef POWER_CONTROL_H_
#define POWER_CONTROL_H_

// Logical 1 turns off, logical 0 turns on.
typedef enum
{
    OnDeviceState,
    OffDeviceState
} DeviceState_t;

void manageValvePower( DeviceState_t state );

void manageWaterPump( DeviceState_t state );

#endif /* POWER_CONTROL_H_ */
