// Copyright (C) Frances Barnard Telfar. All Rights Reserved.

/////////////////////
// Helpers

internal u64
vm_load_const(Interpreter *interpreter)
{
    u8 index = interpreter->program[interpreter->current_block_index].instructions.str[interpreter->pc];
    u64 constant = interpreter->program[interpreter->current_block_index].constants.str[index];
    interpreter->pc += 1;
    return constant;
}

internal u64
vm_top(Interpreter *interpreter)
{
    return interpreter->stack[interpreter->stack_pos - 1];
}

internal void
vm_push(Interpreter *interpreter, u64 value)
{
    interpreter->stack[interpreter->stack_pos] = value;
    interpreter->stack_pos += 1;
}

internal u64
vm_pop(Interpreter *interpreter)
{
    interpreter->stack_pos -= 1;
    return interpreter->stack[interpreter->stack_pos];
}

internal u8
vm_next_op(Interpreter *interpreter)
{
    u8 op = interpreter->program[interpreter->current_block_index].instructions.str[interpreter->pc];
    interpreter->pc += 1;
    return op;
}

internal bool32
vm_jump(Interpreter *interpreter)
{
    u64 block_index = vm_next_op(interpreter);
    interpreter->pc = 0;
    interpreter->current_block_index = block_index;
    return (block_index < interpreter->code_block_count);
}

//////////////////////
// Top-Level Functions

internal void
vm_load_code_blocks(Interpreter *interpreter, Code_Block *code_blocks, u64 code_block_count)
{
    interpreter->program = code_blocks;
    interpreter->code_block_count = code_block_count;
}

internal void
vm_run(Interpreter *interpreter)
{
    bool32 run = 1;
    for(;run && interpreter->pc < interpreter->program[interpreter->current_block_index].instructions.size;)
    {
        u8 op = interpreter->program[interpreter->current_block_index].instructions.str[interpreter->pc];
        interpreter->pc += 1;
        switch(op)
        {
            default:
            {
                printf("Not implemented!\n");
                return;
            }break;
            case OpCode_Push:
            {
                vm_push(interpreter, vm_load_const(interpreter));
            }break;
            case OpCode_Add:
            {
                u64 a = vm_pop(interpreter);
                u64 b = vm_pop(interpreter);
                vm_push(interpreter, a + b);
            }break;
            case OpCode_Eq:
            {
                u64 a = vm_pop(interpreter);
                u64 b = vm_pop(interpreter);
                vm_push(interpreter, a == b);
            }break;
            case OpCode_Jump:
            {
                run = vm_jump(interpreter);
            }break;
            case OpCode_JumpNZ:
            {
                if(vm_pop(interpreter) != 0)
                {
                    run = vm_jump(interpreter);
                }
                else
                {
                    interpreter->pc += 1;
                }
            }break;
            case OpCode_LoadVar:
            {
                u64 index = vm_next_op(interpreter);
                vm_push(interpreter, interpreter->stack[index]);
            }break;
            case OpCode_StoreVar:
            {
                u64 index = vm_next_op(interpreter);
                interpreter->stack[index] = vm_pop(interpreter);
            }break;
        }
    }
}
