#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "SMlib.h"

int main(void)
{
    printf("\nSTART\n");

    stateMachineCTX* SM_CTX = SMlib_createCTX();

    SMlib_initCTX(SM_CTX);

    SMlib_printCurrentState(SM_CTX);
    SMlib_printStatesGraphEdge(SM_CTX);
    SMlib_printLunsAccessTable(SM_CTX);

    stateIndexCode newState   = S1;
    switchResult   resultCode = SMlib_switchState(SM_CTX, newState);

    switch (resultCode)
    {
        case SWITCH_SUCCESS:
            printf("\nSUCCESS: now state is %i\n", newState);
            break;

        case SWITCH_ERROR_STATE_DOES_NOT_EXIST:
            printf("\nERROR: state with index %i does not exist\n", newState);
            break; 

        case SWITCH_ERROR_NO_DIRECT_WAY:
            printf("\nERROR: no direct way from %i to %i\n", SMlib_getCurrentState(SM_CTX), newState);
            break;

        case SWITCH_ERROR_EXTERNAL_ACTION_FAIL:
            printf("\nERROR: external action failed, switching from %i to %i has been stopped\n",
                    SMlib_getCurrentState(SM_CTX), newState);
            break;

        // TODO: implement in future
        // case SWITCH_ERROR_HMAC_ERROR:
        //     break;

        default:
            break;            

    }

    SMlib_destroyCTX(SM_CTX);

    printf("\nSize %i bytes\n", SMlib_getSizeofCTX());

    printf("\nEND\n");

    return 0;       
}