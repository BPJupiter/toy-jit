// Copyright (C) Frances Barnard Telfar. All Rights Reserved.

#include "fbt_base.h"
#include "opcode.h"
#include "interpreter.h"

#include "interpreter.c"

int
main(int argc, char **argv)
{
    // 0 is x, 1 is i
    u64 setup_const_data[] = {0};
    u8 setup_code_data[] =
    {
        OpCode_Push, 0, // x = 0
        OpCode_Push, 0, // i = 0
        OpCode_Jump, 1, // jump to while loop (block 1)
    };
    u64 loop_const_data[] = {1000000, 1};
    u8 loop_code_data[] =
    {
        // while i < 1,000,000
        OpCode_LoadVar, 1, // load i
        OpCode_Push, 0,    // load 1000000
        OpCode_Eq,
        OpCode_JumpNZ, 2,  // jump to exit if i == 1,000,000

        // x = x + 1
        OpCode_LoadVar, 0, // load x
        OpCode_LoadVar, 1, // load i
        OpCode_Add,
        OpCode_StoreVar, 0, // x = x + i

        // i += 1
        OpCode_LoadVar, 1, // load i
        OpCode_Push, 1,    // load 1
        OpCode_Add,
        OpCode_StoreVar, 1, // i = i + 1
        OpCode_Jump, 1, // jump to start
    };
    u64 end_const_data[] = {0};
    u8 end_code_data[] =
    {
        OpCode_LoadVar, 0, // load x
    };
    String8 setup_code    = str8(setup_code_data, ArrayCount(setup_code_data));
    String64 setup_consts = str64(setup_const_data, ArrayCount(setup_const_data));
    String8 loop_code    = str8(loop_code_data, ArrayCount(loop_code_data));
    String64 loop_consts = str64(loop_const_data, ArrayCount(loop_const_data));
    String8 end_code    = str8(end_code_data, ArrayCount(end_code_data));
    String64 end_consts = str64(end_const_data, ArrayCount(end_const_data));

    Code_Block program[] =
    {
        {.instructions = setup_code, .constants    = setup_consts},
        {.instructions = loop_code,  .constants    = loop_consts},
        {.instructions = end_code,   .constants    = end_consts},
    };

    Interpreter vm = {0};
    vm_load_code_blocks(&vm, program, ArrayCount(program));
    vm_run(&vm);
    printf("%I64u\n", vm_top(&vm));
    
    return 0;
}
