// Copyright (C) Frances Barnard Telfar. All Rights Reserved.

#ifndef OPCODE_H
#define OPCODE_H

typedef u8 OpCode;
enum OpCodeEnum
{
    OpCode_Null,
    
    OpCode_Push,
    OpCode_Add,
    OpCode_Eq,
    OpCode_JumpNZ,
    OpCode_Jump,
    OpCode_LoadVar,
    OpCode_StoreVar,

    OpCode_COUNT
};

#endif // OPCODE_H
