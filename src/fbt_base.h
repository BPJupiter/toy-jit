// Copyright (C) Frances Barnard Telfar. All Rights Reserved.

#ifndef FBT_BASE_H
#define FBT_BASE_H

///////////////////////
// Foreign Includes

#include <stdint.h>

////////////////////////
// Codebase Keywords

#define internal      static
#define global        static
#define local_persist static

//////////////////////
// Misc. Helper Macros

#define ArrayCount(a) (sizeof(a) / sizeof((a)[0]))

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

#endif // FBT_BASE_H
