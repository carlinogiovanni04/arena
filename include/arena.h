/*
// Arena, an arena allocator written in ANSI C
// Created by Giovanni Carlino - 13/11/2026 
*/

#ifndef ARENA_H
#define ARENA_H

#if !defined (_M_AMD64) || !defined (_WIN32)
#error "Only x86_64 Windows is supported right now"
#endif

#define WIN32_LEAN_AND_MEAN
#define UNICODE
#include <Windows.h>

typedef char       i8;
typedef short      i16;
typedef int        i32;
typedef long long  i64;

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;

#ifdef _DEBUG
#define ASSERT(c) \
    do { \
        if (!(c)) { \
            char _msg[256]; \
            snprintf(_msg, sizeof(_msg), "Assertion Failed: %s\nFile: %s\nLine: %d", #c, __FILE__, __LINE__); \
            MessageBoxA(NULL, _msg, "Fatal Error", MB_OK | MB_ICONERROR); \
            DebugBreak(); \
            PostQuitMessage(0xff); \
        } \
    } while(0)
#else
#define ASSERT(c) ((void)0)
#endif

typedef struct {
    u64  pos;
    u64  cap;
    void *data;
} Arena;

static Arena arena_init(u64 capacity) {
    Arena arena;
    arena.pos = 0;
    arena.cap = 0;
    arena.data = NULL;

    /* Reserve and commit memory chunks */
    arena.data = VirtualAlloc(NULL, capacity, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    ASSERT(arena.data != NULL && "Arena VirtualAlloc failed");
    arena.cap = capacity;
    return arena;
}

static void* arena_push(Arena *arena, u64 size, u64 alignment) {
    u64 current_address;
    u64 padding;
    u64 total_size;
    void *ptr;

    current_address = (u64)((char *)arena->data + arena->pos);
    padding = (alignment - (current_address % alignment)) % alignment;
    
    total_size = size + padding;
    ASSERT(arena->pos + total_size <= arena->cap && "Arena Out of Memory!");
    
    ptr = (char *)arena->data + arena->pos + padding;
    arena->pos += total_size;
    
    ZeroMemory(ptr, size); 
    
    return ptr;
}

static void arena_reset(Arena *arena) {
    arena->pos = 0;
}

static void arena_destroy(Arena *arena) {
    if (arena->data) {
        VirtualFree(arena->data, 0, MEM_RELEASE);
        arena->data = NULL;
        arena->cap = 0;
        arena->pos = 0;
    }
}

static void arena_pop_to(Arena *arena, u64 pos) {
    ASSERT(pos <= arena->pos && "Invalid arena pop position!");
    arena->pos = pos;
}

static void arena_pop(Arena *arena, u64 size) {
    ASSERT(arena->pos >= size && "Arena underflow during pop!");
    arena->pos -= size;
}

#endif