#ifndef SM_LIB_EXTERNAL_ACTION_H
#define SM_LIB_EXTERNAL_ACTION_H

#include "SMlib.h"

#define EXTERNAL_ACTION_NAMES [S0][S0] = printSwitchingInfo,  \
                              [S0][S1] = printSwitchingInfo,  \
                              [S0][S2] = printSwitchingInfo,  \
                              \
                              [S1][S0] = printSwitchingInfo,  \
                              [S1][S1] = printSwitchingInfo,  \
                              [S1][S2] = printSwitchingInfo,  \
                              \
                              [S2][S0] = printSwitchingInfo,  \
                              [S2][S1] = printSwitchingInfo,  \
                              [S2][S2] = printSwitchingInfo

externalActionResult printSwitchingInfo(void* abstract_CTX, stateIndexCode newState);

#endif // SM_LIB_EXTERNAL_ACTION_H