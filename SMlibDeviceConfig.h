#ifndef SM_LIB_DEVICE_CONFIG_H
#define SM_LIB_DEVICE_CONFIG_H

/* 
    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    @                                    @
    @ Device serial number and HMAC key  @ 
    @                                    @
    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#define DEVICE_SERIAL_NUMBER 4294967295 // from 0 to UINT64_MAX 

#define DEVICE_HMAC_KEY 0xff,   0xfe,   0xfd,   0xfc, \
                        0xfb,   0xfa,   0xf9,   0xf8, \
                        0xf7,   0xf6,   0xf5,   0xf4, \
                        0xf3,   0xf2,   0xf1,   0xf0, \
                        \
                        0xef,   0xee,   0xed,   0xec, \
                        0xeb,   0xea,   0xe9,   0xe8, \
                        0xe7,   0xe6,   0xe5,   0xe4, \
                        0xe3,   0xe2,   0xe1,   0xe0

/* 
    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    @                                    @
    @ State Machine Directed Graph types @ 
    @                                    @
    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#define STATES_NAMES S0, S1, S2

#define ADJACENCY_MATRIX [S0][S0] = {False},  \
                         [S0][S1] = {True},   \
                         [S0][S2] = {False},  \
                         \
                         [S1][S0] = {False},  \
                         [S1][S1] = {True},   \
                         [S1][S2] = {True},   \
                         \
                         [S2][S0] = {False},  \
                         [S2][S1] = {True},   \
                         [S2][S2] = {False}

/* 
    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    @                                        @
    @ State Machine Luns Access Rights types @
    @                                        @
    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

#define LUNS_NAMES L0, L1, L2, L3

#define LUNS_ACCESS_MATRIX [S0][L0] = RO,  \
                           [S0][L1] = HI,  \
                           [S0][L2] = HI,  \
                           [S0][L3] = HI,  \
                           \
                           [S1][L0] = RO,  \
                           [S1][L1] = RW,  \
                           [S1][L2] = RO,  \
                           [S1][L3] = AO,  \
                           \
                           [S2][L0] = RW,  \
                           [S2][L1] = RW,  \
                           [S2][L2] = RW,  \
                           [S2][L3] = RW

#endif // SM_LIB_DEVICE_CONFIG_H