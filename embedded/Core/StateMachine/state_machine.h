#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

typedef enum {
    STATE_IDLE,
    STATE_ARMED,
    STATE_ALARM,
    STATE_ERROR
} SystemState;

void SM_Init(void);
void SM_Update(void);
SystemState SM_GetState(void);
void SM_SetState(SystemState newState);

#endif