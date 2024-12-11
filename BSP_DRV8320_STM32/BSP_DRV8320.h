#ifndef __BSP_DRV8320_H
#define __BSP_DRV8320_H

#include <stdint.h>
typedef struct{
    void (*Ctor)(void);
    void (*SetEnable)(uint8_t);
    void (*SetCS)(uint8_t);
    void (*Transmit)(uint16_t);
    uint16_t (*Receive)(uint16_t);
    
}DRV8320Functions;

extern DRV8320Functions FuncList;
#endif 