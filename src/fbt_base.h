// Copyright (C) Frances Barnard Telfar. All Rights Reserved.

#ifndef FBT_BASE_H
#define FBT_BASE_H

///////////////////////
// Foreign Includes

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

////////////////////////
// Codebase Keywords

#define internal      static
#define global        static
#define local_persist static

//////////////////////
// Misc. Helper Macros

#define Stringify_(S) #S
#define Stringify(S) Stringify_(S)

#define Concat_(A,B) A##B
#define Concat(A,B) Concat_(A,B)

#define ArrayCount(a) (sizeof(a) / sizeof((a)[0]))
#define StaticAssert(C, ID) global u8 Concat(ID, __LINE__)[(C)?1:-1]

#define AlignPow2(x,b)     (((x) + (b) - 1)&(~((b) - 1)))

/////////////////
// Clamps

#define Min(A,B) (((A)<(B))?(A):(B))
#define Max(A,B) (((A)>(B))?(A):(B))
#define ClampTop(A,X) Min(A,X)
#define ClampBot(X,B) Max(X,B)
#define Clamp(A,X,B) (((X)<(A))?(A):((X)>(B))?(B):(X))

//////////////////
// Type -> Alignment

#ifdef _WIN32
# define AlignOf(T) __alignof(T)
#else
# error No AlignOf intrinsic implementation for this platform.
#endif // _WIN32

//////////////////////////////
// Memory operation macros

#define MemoryCopy(dst, src, size)     memmove((dst), (src), (size))
#define MemorySet(dst, byte, size)     memset((dst), (byte), (size))
#define MemoryCompare(a, b, size)      memcmp((a), (b), (size))

#define MemoryCopyStruct(d,s)  MemoryCopy((d),(s),sizeof(*(d)))
#define MemoryCopyArray(d,s)   MemoryCopy((d),(s),sizeof(d))
#define MemoryCopyTyped(d,s,c) MemoryCopy((d),(s),sizeof(*(d))*(c))
#define MemoryCopyStr8(dst, s) MemoryCopy(dst, (s).str, (s).size)

#define MemoryZero(s,z)       memset((s),0,(z))
#define MemoryZeroStruct(s)   MemoryZero((s),sizeof(*(s)))
#define MemoryZeroArray(a)    MemoryZero((a),sizeof(a))
#define MemoryZeroTyped(m,c)  MemoryZero((m),sizeof(*(m))*(c))

#define MemoryMatch(a,b,z)     (MemoryCompare((a),(b),(z)) == 0)
#define MemoryMatchStruct(a,b)  MemoryMatch((a),(b),sizeof(*(a)))
#define MemoryMatchArray(a,b)   MemoryMatch((a),(b),sizeof(a))

#define MemoryIsZeroStruct(ptr) memory_is_zero((ptr), sizeof(*(ptr)))

/////////////////
// Base types

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;
typedef u8       byte;
typedef s8       bool8;
typedef s16      bool16;
typedef s32      bool32;
typedef s64      bool64;
typedef float    f32;
typedef double   f64;
typedef void Void_Proc(void);
typedef union u128 u128;
union u128 {
    u8  u8[16];
    u16 u16[8];
    u32 u32[4];
    u64 u64[2];
    f32 f32[4];
    f64 f64[2];
};

/////////////////
// Units

#define Kilobytes(n) (((u64)(n)) << 10)
#define Megabytes(n) (((u64)(n)) << 20)
#define Gigabytes(n) (((u64)(n)) << 30)
#define Terabytes(n) (((u64)(n)) << 40)
#define Thousand(n)  ((n)*1000)
#define Million(n)   ((n)*1000000)
#define Billion(n)   ((n)*1000000000)

////////////////////////////////
// String Types

typedef struct String8 String8;
struct String8
{
    u8 *str;
    u64 size;
};

typedef struct String16 String16;
struct String16
{
    u16 *str;
    u64 size;
};

typedef struct String32 String32;
struct String32
{
    u32 *str;
    u64 size;
};

typedef struct String64 String64;
struct String64
{
    u64 *str;
    u64 size;
};

#define s(S) str8_lit(S)

#define str8_lit(S)  str8((u8*)(S), sizeof(S) - 1)
#define str8_lit_comp(S) {(u8*)(S), sizeof(S) - 1,}
#define str8_lit_cstr(S) str8((u8*)(S), sizeof(S))
#define str8_varg(S) (int)((S).size), ((S).str)

#define str8_array(S,C) str8((u8*)(S), sizeof(*(S))*(C))
#define str8_array_fixed(S) str8((u8*)(S), sizeof(S))
#define str8_struct(S) str8((u8*)(S), sizeof(*(S)))

internal String8  str8(u8 *str, u64 size)   { String8 result = {str, size}; return result; }
internal String8  str8_zero(void)           { String8 result = {0}; return result; }
internal String16 str16(u16 *str, u64 size) { String16 result = {str, size}; return result; }
internal String16 str16_zero(void)          { String16 result = {0}; return result; }
internal String32 str32(u32 *str, u64 size) { String32 result = {str, size}; return result; }
internal String32 str32_zero(void)          { String32 result = {0}; return result; }
internal String64 str64(u64 *str, u64 size) { String64 result = {str, size}; return result; }
internal String64 str64_zero(void)          { String64 result = {0}; return result; }

//////////////////
// Arenas

#define ARENA_MAX         Gigabytes(4)
#define ARENA_COMMIT_SIZE Kilobytes(4)
#define ARENA_HEADER_SIZE 32

typedef struct Arena Arena;
struct Arena
{
    void *base;
    u64 max;
    u64 alloc_position;
    u64 commit_position;
};
StaticAssert(sizeof(Arena) <= ARENA_HEADER_SIZE, arena_header_size_check);

internal Arena *arena_alloc(void);
internal void arena_release(Arena *arena);

internal void *arena_push(Arena *arena, u64 size, u64 align, bool32 zero);
internal u64 arena_pos(Arena *arena);
internal void arena_pop_to(Arena *arena, u64 pos);

internal void arena_clear(Arena *arena);
internal void arena_pop(Arena *arena, u64 amount);

#define push_array_no_zero_aligned(a,T,c,align) (T *)arena_push((a), sizeof(T) * (c), (align), (0))
#define push_array_aligned(a,T,c,align) (T *)arena_push((a), sizeof(T) * (c), (align), (1))
#define push_array_no_zero(a,T,c) push_array_no_zero_aligned(a,T,c,Max(8, AlignOf(T)))
#define push_array(a,T,c) push_array_aligned(a,T,c,Max(8, AlignOf(T)))

////////////////////////
// Operating System Primitives

internal void *reserve_memory(u64 size);
internal void release_memory(void *memory);
internal void commit_memory(void *memory, u64 size);
internal void decommit_memory(void *memory, u64 size);
internal String8 load_entire_file(Arena *arena, String8 path);

///////////////////
// Arena Function Implementations

internal Arena *
arena_alloc(void)
{
    void *base = reserve_memory(ARENA_MAX);
    commit_memory(base, ARENA_COMMIT_SIZE);

    Arena *arena           = base;
    arena->base            = base;
    arena->max             = ARENA_MAX;
    arena->alloc_position  = ARENA_HEADER_SIZE;
    arena->commit_position = ARENA_COMMIT_SIZE;

    return arena;
}

internal void
arena_release(Arena *arena)
{
    release_memory(arena);
}

internal void *
arena_push(Arena *arena, u64 size, u64 align, bool32 zero)
{
    u64 alloc_pos_before = AlignPow2(arena->alloc_position, align);
    u64 alloc_pos_after  = alloc_pos_before + size;

    u64 size_to_zero = 0;
    if(zero)
    {
        size_to_zero = Min(arena->commit_position, alloc_pos_after) - alloc_pos_before;
    }

    if(arena->commit_position < alloc_pos_after)
    {
        u64 commit_pos_after = alloc_pos_after + ARENA_COMMIT_SIZE - 1;
        commit_pos_after -= commit_pos_after & ARENA_COMMIT_SIZE;
        commit_pos_after = ClampTop(commit_pos_after, arena->max);
        u64 commit_size = commit_pos_after - arena->commit_position;
        u8 *committed_bytes = (u8 *)arena + arena->commit_position;
        commit_memory(committed_bytes, commit_size);
        arena->commit_position = commit_pos_after;
    }

    void *memory = 0;
    if(arena->commit_position >= alloc_pos_after);
    {
        memory = (u8 *)arena + alloc_pos_before;
        arena->alloc_position = alloc_pos_after;
        MemoryZero(memory, size_to_zero);
    }
    
    return memory;
}

internal u64
arena_pos(Arena *arena)
{
    return arena->alloc_position;
}

internal void
arena_pop_to(Arena *arena, u64 pos)
{
    u64 new_pos = ClampBot(ARENA_HEADER_SIZE, pos);
    arena->alloc_position = new_pos;
}

internal void
arena_clear(Arena *arena)
{
    arena_pop_to(arena, 0);
}

internal void
arena_pop(Arena *arena, u64 amount)
{
    u64 pos_old = arena_pos(arena);
    u64 pos_new = pos_old;
    if(amount < pos_old)
    {
        pos_new = pos_old - amount;
    }
    arena_pop_to(arena, pos_new);
}

//////////////////////
// Operating System Function Imeplementions

#ifdef _WIN32

#include <windows.h>

internal void *reserve_memory(u64 size) { return VirtualAlloc(0, size, MEM_RESERVE, PAGE_NOACCESS); }
internal void release_memory(void *memory) { VirtualFree(memory, 0, MEM_RELEASE); }
internal void commit_memory(void *memory, u64 size) { VirtualAlloc(memory, size, MEM_COMMIT, PAGE_READWRITE); }
internal void decommit_memory(void *memory, u64 size) { VirtualFree(memory, size, MEM_DECOMMIT); }

internal String8
load_entire_file(Arena *arena, String8 path)
{
    String8 data = {0};
    HANDLE file = {0};

    {
        DWORD desired_access = GENERIC_READ | GENERIC_WRITE;
        DWORD share_mode = 0;
        SECURITY_ATTRIBUTES security_attributes =
        {
            (DWORD)sizeof(SECURITY_ATTRIBUTES),
            0,
            0,
        };
        DWORD creation_disposition = OPEN_EXISTING;
        DWORD flags_and_attribytes = 0;
        HANDLE template_file = 0;

        if((file = CreateFile(path.str, desired_access, share_mode, &security_attributes, creation_disposition, flags_and_attribytes, template_file)) != INVALID_HANDLE_VALUE)
        {
            DWORD read_bytes = GetFileSize(file, 0);
            if(read_bytes)
            {
                void *read_data = push_array(arena, u8, read_bytes+1);
                DWORD bytes_read = 0;
                OVERLAPPED overlapped = {0};

                ReadFile(file, read_data, read_bytes, &bytes_read, &overlapped);

                ((u8 *)read_data)[read_bytes] = 0;
                data.str = read_data;
                data.size = (u64)bytes_read;
            }
            CloseHandle(file);
        }
    }
}


#else
# error No operating system primitives defined for the current target.
#endif // _WIN32

#endif // FBT_BASE_H
