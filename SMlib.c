#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "SMlib.h"
#include "SMlibExternalAction.h"

typedef externalActionResult (*externalActionPtr) (void* abstract_CTX, stateIndexCode newState);

#pragma pack(push, 1)
struct directedEdge 
{
    Bool              edgeExist;
    externalActionPtr externalAction;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct stateMachineCTX
{
    directedEdge**   statesGraph;
    lunAccessRight** lunsAccessTable;

    stateIndexCode   currentState;

    sessionType      sessionDuration; // need init!
    jsonRpcId        lastJsonRpcId;

    Bool             isCnonceSet;
    uint8_t*         cnonce;

    Bool             isSnonceSet;
    uint8_t*         snonce;

    uint64_t         deviceSerialNumber;
    uint8_t*         deviceHmacKey;
};
#pragma pack(pop)

typedef directedEdge   statesGraphMatrix[StatesQuantity][StatesQuantity];
typedef lunAccessRight lunsAccessMatrix[StatesQuantity][LunsQuantity];

/* 
    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    @                                  @
    @ State Machine Context management @
    @                                  @
    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

stateMachineCTX* SMlib_createCTX(void)
{
    stateMachineCTX* SM_CTX = (stateMachineCTX*) malloc(sizeof(stateMachineCTX));

    SM_CTX->statesGraph = (directedEdge**) malloc(StatesQuantity * sizeof(directedEdge*));
    for(uint8_t i = 0; i < StatesQuantity; i++)
        SM_CTX->statesGraph[i] = (directedEdge*) malloc(StatesQuantity * sizeof(directedEdge));

    SM_CTX->lunsAccessTable = (lunAccessRight**) malloc(StatesQuantity * sizeof(lunAccessRight*));
    for(uint8_t i = 0; i < StatesQuantity; i++)
        SM_CTX->lunsAccessTable[i] = (lunAccessRight*) malloc(LunsQuantity * sizeof(lunAccessRight));

    SM_CTX->deviceHmacKey = (uint8_t*) malloc(DEVICE_HMAC_KEY_LENGTH * sizeof(uint8_t)); 

    SM_CTX->cnonce = (uint8_t*) malloc(CNONCE_LENGTH * sizeof(uint8_t));
    SM_CTX->snonce = (uint8_t*) malloc(SNONCE_LENGTH * sizeof(uint8_t));

    return SM_CTX;
}

void SMlib_initCTX(stateMachineCTX* SM_CTX)
{
    statesGraphMatrix statesGraph = {ADJACENCY_MATRIX};
        for (uint8_t i = 0; i < StatesQuantity; i++)
            for (uint8_t j = 0; j < StatesQuantity; j++)
                SM_CTX->statesGraph[i][j].edgeExist = statesGraph[i][j].edgeExist;

    lunsAccessMatrix lunsAccessTable = {LUNS_ACCESS_MATRIX};
        for (uint8_t i = 0; i < StatesQuantity; i++)
            for (uint8_t j = 0; j < LunsQuantity; j++)
                SM_CTX->lunsAccessTable[i][j] = lunsAccessTable[i][j];

    externalActionPtr externalActionPtrMatrix[StatesQuantity][StatesQuantity] = {EXTERNAL_ACTION_NAMES};
    for (uint8_t i = 0; i < StatesQuantity; i++)
        for (uint8_t j = 0; j < StatesQuantity; j++)
        {
            if (NULL == externalActionPtrMatrix[i][j])
            {
                printf("\nINIT ERROR: function ptr for external action from %i to %i not set\n", i, j);
                exit(-1); // TODO: think about this error code semantic and remove printf above
            }

            SM_CTX->statesGraph[i][j].externalAction = externalActionPtrMatrix[i][j];
        }
    
    uint8_t deviceHmacKey[DEVICE_HMAC_KEY_LENGTH] = {DEVICE_HMAC_KEY};
    for (uint8_t i = 0; i < DEVICE_HMAC_KEY_LENGTH; i++)
        SM_CTX->deviceHmacKey[i] = deviceHmacKey[i];

    SM_CTX->currentState    = 0; // initial state has index code zero
    SM_CTX->sessionDuration = sessionDurationNotSet;
    SM_CTX->lastJsonRpcId   = sessionNotStartedId;

    SM_CTX->isCnonceSet = False;
    for (uint8_t i = 0; i < CNONCE_LENGTH; i++)
        SM_CTX->cnonce[i] = 0;

    SM_CTX->isSnonceSet = False;
    for (uint8_t i = 0; i < SNONCE_LENGTH; i++)
        SM_CTX->snonce[i] = 0;

    SM_CTX->deviceSerialNumber = DEVICE_SERIAL_NUMBER;

}

void SMlib_destroyCTX(stateMachineCTX* SM_CTX)
{
    for(uint8_t i = 0; i < StatesQuantity; i++)
        free(SM_CTX->statesGraph[i]);

    free(SM_CTX->statesGraph);

    for(uint8_t i = 0; i < StatesQuantity; i++)
        free(SM_CTX->lunsAccessTable[i]);

    free(SM_CTX->lunsAccessTable);

    free(SM_CTX->deviceHmacKey);

    free(SM_CTX->cnonce);
    free(SM_CTX->snonce);

    free(SM_CTX);
}

switchResult SMlib_switchState(stateMachineCTX* SM_CTX, stateIndexCode newState)
{
    // TODO: implement hmac processing

    if (!SMlib_checkStateExist(newState))
        return SWITCH_ERROR_STATE_DOES_NOT_EXIST;

    if (!SMlib_checkDirectWayExist(SM_CTX->currentState, newState, SM_CTX))
        return SWITCH_ERROR_NO_DIRECT_WAY;

    externalActionResult resultCode = SM_CTX->statesGraph[SM_CTX->currentState][newState].externalAction(SM_CTX, newState);    
    switch(resultCode)
    {
        case EXTERNAL_ACTION_SUCCESS:
            SM_CTX->currentState = newState;
            return SWITCH_SUCCESS;
            break;

        // TODO: write description for with block

        // case CUSTOM_ERROR_CODE_A:
        //     return SWITCH_ERROR_EXTERNAL_ACTION_FAIL;
        //     break;

        // case CUSTOM_ERROR_CODE_B:
        //     return SWITCH_ERROR_EXTERNAL_ACTION_FAIL;
        //     break;

        // case CUSTOM_ERROR_CODE_C:
        //     return SWITCH_ERROR_EXTERNAL_ACTION_FAIL;
        //     break;

        case EXTERNAL_ACTION_UNDEFINED_ERROR:
        default:
            return SWITCH_ERROR_EXTERNAL_ACTION_FAIL;
            break;
    }    
}

/* 
    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    @                                                                          @
    @ State Machine Context getters and auxiliary not-CTX-changeable functions @
    @                                                                          @
    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

stateIndexCode SMlib_getCurrentState(stateMachineCTX* SM_CTX)
{
    return SM_CTX->currentState;
}

Bool SMlib_checkStateExist(stateIndexCode state)
{
    if(state >= 0 && state < StatesQuantity)
        return True;
    
    return False;
}

Bool SMlib_checkDirectWayExist(stateIndexCode stateFrom, stateIndexCode stateTo, stateMachineCTX* SM_CTX)
{
    // TODO: create doc note that user must call SMlib_checkStateExist() for stateFrom and stateTo before
    if(SM_CTX->statesGraph[stateFrom][stateTo].edgeExist)
        return True;

    return False;    
}

Bool SMlib_checkLunExist(lunIndexCode lun)
{
    if(lun >= 0 && lun < LunsQuantity)
        return True;

    return False;    
}

lunAccessRight SMlib_getLunAccessRight(stateIndexCode state, lunIndexCode lun, stateMachineCTX* SM_CTX)
{
    // TODO: create doc note that user must call SMlib_checkLunExist() for lun and SMlib_checkStateExist for state before
    return SM_CTX->lunsAccessTable[state][lun];
}

uint16_t SMlib_getSizeofCTX(void)
{
    uint16_t statesGraphSize     = StatesQuantity * sizeof(directedEdge*)   + StatesQuantity * (StatesQuantity * sizeof(directedEdge));
    uint16_t lunsAccessTableSize = StatesQuantity * sizeof(lunAccessRight*) + StatesQuantity * (LunsQuantity   * sizeof(lunAccessRight));
    uint16_t cnonceSize          = CNONCE_LENGTH  * sizeof(uint8_t);
    uint16_t snonceSize          = SNONCE_LENGTH  * sizeof(uint8_t);
    uint16_t deviceHmacKeySize   = DEVICE_HMAC_KEY_LENGTH * sizeof(uint8_t);
    uint16_t restOfStructureSize = sizeof(stateMachineCTX);

    return statesGraphSize + lunsAccessTableSize + cnonceSize + snonceSize + deviceHmacKeySize + restOfStructureSize;
}

/* 
    @@@@@@@@@@@@@@@@@@@@@@@
    @                     @
    @ Temporary functions @
    @                     @
    @@@@@@@@@@@@@@@@@@@@@@@
*/

void SMlib_printCurrentState(stateMachineCTX* SM_CTX)
{
    printf("\nCurrent state code is => %i\n", SM_CTX->currentState);
}

void SMlib_printStatesGraphEdge(stateMachineCTX* SM_CTX)
{
    printf("\nFormat [stateFrom][stateTo] => isEdgeExist\n");
    for (uint8_t i = 0; i < StatesQuantity; i++)
        for (uint8_t j = 0; j < StatesQuantity; j++)
            printf("[%i][%i] => %s\n", i, j, SMlib_checkDirectWayExist(i, j, SM_CTX) ? "TRUE" : "FALSE");
    printf("\n");
}

void SMlib_printLunsAccessTable(stateMachineCTX* SM_CTX)
{
    printf("\nFormat [state][lun] => [RW=0, RO=1, HI=2, AO=3]\n");
    for (uint8_t i = 0; i < StatesQuantity; i++)
        for (uint8_t j = 0; j < LunsQuantity; j++)
            printf("[%i][%i] => %i\n", i, j, SMlib_getLunAccessRight(i, j, SM_CTX));
    printf("\n");
}