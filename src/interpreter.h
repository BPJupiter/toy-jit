// Copyright (C) Frances Barnard Telfar. All Rights Reserved.

#ifndef INTERPRETER_H
#define INTERPRETER_H

typedef struct Code_Block Code_Block;
struct Code_Block
{
    String8 instructions;
    String64 constants;
};

typedef struct Interpreter Interpreter;
struct Interpreter
{
    u64 stack[4096];
    
    u64 code_block_count;
    Code_Block *program;
    
    u64 current_block_index;

    u64 stack_pos;
    u64 pc;
};

///////////////
// Helpers

internal u64 vm_load_const(Interpreter *interpreter);
internal u64 vm_top(Interpreter *interpreter);
internal void vm_push(Interpreter *interpreter, u64 value);
internal u64 vm_pop(Interpreter *interpreter);
internal u8 vm_next_op(Interpreter *interpreter);
internal bool32 vm_jump(Interpreter *interpreter);

////////////////////////
// Top-Level Functions

internal void vm_load_code_blocks(Interpreter *interpreter, Code_Block *code_blocks, u64 code_block_count);
internal void vm_run(Interpreter *interpreter);

#endif // INTERPRETER_H
