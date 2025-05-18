#ifndef OPS_H
#define OPS_H

typedef enum
{
    NULL_OP = 0,
    INC_DPTR,
    DEC_DPTR,
    INC_VAL,
    DEC_VAL,
    WRITE_OUT,
    READ_IN,
    JMP_IF_ZERO,
    JMP_IF_NOT_ZERO
} BF_Opcode;

#endif