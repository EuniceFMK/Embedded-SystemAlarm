#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>
#include <stdbool.h>

/* Keys possibles */
typedef enum {
    KEY_NONE = 0,
    KEY_0, KEY_1, KEY_2, KEY_3,
    KEY_4, KEY_5, KEY_6,
    KEY_7, KEY_8, KEY_9,
    KEY_A,   // ARM
    KEY_D    // DISARM
} Keypad_Key;

/* Init hardware */
void KEYPAD_Init(void);

/* Lecture non bloquante */
Keypad_Key KEYPAD_Scan(void);

/* Helper: convert key to char */
char KEYPAD_KeyToChar(Keypad_Key key);

#endif 