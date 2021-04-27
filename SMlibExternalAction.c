#include <stdio.h>
#include "SMlib.h"
#include "SMlibExternalAction.h"

static stateMachineCTX* castAbstractCTXtoStateMachineCTX(void* abstract_CTX);

externalActionResult printSwitchingInfo(void* abstract_CTX, stateIndexCode newState)
{
    stateMachineCTX* SM_CTX = castAbstractCTXtoStateMachineCTX(abstract_CTX);
    printf("\nEXTERNAL FUNCTION: Switching from %i to %i\n", SMlib_getCurrentState(SM_CTX), newState);
    return EXTERNAL_ACTION_SUCCESS;
}

static stateMachineCTX* castAbstractCTXtoStateMachineCTX(void* abstract_CTX)
{
    return (stateMachineCTX*)abstract_CTX;
}