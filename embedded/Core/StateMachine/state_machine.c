#include "state_machine.h"

static SystemState currentState = STATE_IDLE;

void SM_Init(void)
{
    currentState = STATE_IDLE;
}

void SM_Update(void)
{
    switch(currentState)
    {
        case STATE_IDLE:
            // wait for activation
            break;

        case STATE_ARMED:
            // captors monitoring, ready to trigger alarm
            break;

        case STATE_ALARM:
            // UI alert, siren, notify authorities
            break;

        case STATE_ERROR:
            // fallback security mode
            break;
    }
}

SystemState SM_GetState(void)
{
    return currentState;
}

void SM_SetState(SystemState newState)
{
    currentState = newState;
}