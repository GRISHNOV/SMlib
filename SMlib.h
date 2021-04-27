#ifndef SM_LIB_H
#define SM_LIB_H

#include <stdint.h>
#include "SMlibDeviceConfig.h"

// TODO: write doc
// GCC NOTE: gcc -g -O0 -fshort-enums -Wall -Wpedantic SMlib.c SMlibExternalAction.c main.c
// main.c -- your main program file, SMlibExternalAction.c -- need to implement manually
// don't forget replace example SMlibDeviceConfig.h with your personal configuration file

typedef enum {False, True} Bool;

/* 
    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    @                                                  @
    @ State Machine Directed Graph types and constants @ 
    @                                                  @
    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

typedef enum {STATES_NAMES, StatesQuantity} stateIndexCode;

#define EXTERNAL_ACTION_CUSTOM_ERRORS_CODES CUSTOM_ERROR_CODE_A = 0x10,  \
                                            CUSTOM_ERROR_CODE_B = 0x11,  \
                                            CUSTOM_ERROR_CODE_C = 0x12

typedef enum 
{
    EXTERNAL_ACTION_SUCCESS, 
    EXTERNAL_ACTION_UNDEFINED_ERROR, 
    EXTERNAL_ACTION_CUSTOM_ERRORS_CODES
} externalActionResult;

typedef struct directedEdge directedEdge;

typedef enum 
{
    SWITCH_SUCCESS, 
    SWITCH_ERROR_HMAC_ERROR,
    SWITCH_ERROR_STATE_DOES_NOT_EXIST,
    SWITCH_ERROR_NO_DIRECT_WAY, 
    SWITCH_ERROR_EXTERNAL_ACTION_FAIL
} switchResult;

/* 
    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    @                                                      @
    @ State Machine Luns Access Rights types and constants @
    @                                                      @
    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

typedef enum {LUNS_NAMES, LunsQuantity} lunIndexCode;

typedef enum {RW, RO, HI, AO} lunAccessRight;

typedef struct stateMachineCTX stateMachineCTX;

/* 
    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    @                                       @
    @ HMAC and JSON RPC types and constants @ 
    @                                       @
    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#define CNONCE_LENGTH 16 // 128 bit
#define SNONCE_LENGTH 16 // 128 bit

typedef enum
{
    sessionDurationNotSet,
    onlyGetStateFlagTrue,
    onlyGetStateFlagFalse
} sessionType;

typedef enum 
{
    sessionNotStartedId, 
    initSessionReqestId, 
    initSessionAckId, 
    changeStateRequestId
} jsonRpcId;

#define DEVICE_HMAC_KEY_LENGTH 32 // 256 bit

/* 
    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    @                                            @
    @ State Machine Context management functions @
    @                                            @
    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

stateMachineCTX* SMlib_createCTX(void);

void SMlib_initCTX(stateMachineCTX* SM_CTX);

void SMlib_destroyCTX(stateMachineCTX* SM_CTX);

switchResult SMlib_switchState(stateMachineCTX* SM_CTX, stateIndexCode newState);

/* 
    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    @                                                                          @
    @ State Machine Context getters and auxiliary not-CTX-changeable functions @
    @                                                                          @
    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

stateIndexCode SMlib_getCurrentState(stateMachineCTX* SM_CTX);

Bool SMlib_checkStateExist(stateIndexCode state);

Bool SMlib_checkDirectWayExist(stateIndexCode stateFrom, stateIndexCode stateTo, stateMachineCTX* SM_CTX);

Bool SMlib_checkLunExist(lunIndexCode lun);

lunAccessRight SMlib_getLunAccessRight(stateIndexCode state, lunIndexCode lun, stateMachineCTX* SM_CTX);

uint16_t SMlib_getSizeofCTX(void);

/* 
    @@@@@@@@@@@@@@@@@@@@@@@
    @                     @
    @ Temporary functions @
    @                     @
    @@@@@@@@@@@@@@@@@@@@@@@
*/

void SMlib_printCurrentState(stateMachineCTX* SM_CTX);

void SMlib_printStatesGraphEdge(stateMachineCTX* SM_CTX);

void SMlib_printLunsAccessTable(stateMachineCTX* SM_CTX);

#endif // SM_LIB_H