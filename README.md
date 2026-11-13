# Arena

A lightweight, performance focused arena allocator written in **ANSI C (C89)**, specifically designed for **x86_64 Windows**. 

---

## Features

* **Header-Only Friendly**: Single-header design (`arena.h`).
* **Win32 Native**: Leverages direct OS memory management via `VirtualAlloc` and `VirtualFree`.
* **Strict Alignment Support**: Allows custom power-of-two alignments for safe SIMD and data structuring.
* **Debug Assertions**: Custom `ASSERT` macro that triggers a native Windows message box (`MessageBoxA`), logs the exact file and line number, and halts execution via `DebugBreak()` in debug builds.

---

## API Overview

| Function | Description |
| :--- | :--- |
| `Arena arena_init(u64 capacity)` | Reserves and commits a fixed block of virtual memory. |
| `void* arena_push(Arena *arena, u64 size, u64 alignment)` | Allocates memory with a given alignment and zero-initializes it. |
| `void arena_reset(Arena *arena)` | Resets the arena pointer back to zero without releasing OS memory. |
| `void arena_destroy(Arena *arena)` | Frees the underlying virtual memory back to the OS. |
| `arena.pos = checkpoint;` | Rolls back the arena to a previously saved byte position. |

---

## Quick Start

```c
#include <stdio.h>
#include "arena.h"

int main(void) {
    /* Initialize a 4 KB arena */
    Arena arena = arena_init(4096);

    /* Push memory with 8-byte alignment */
    int *numbers = arena_push(&arena, sizeof(int) * 10, sizeof(int));

    /* Use memory (automatically zero-initialized) */
    numbers[0] = 42;

    /* Save a checkpoint for temporary allocations */
    u64 checkpoint = arena.pos;
    void *temp_block = arena_push(&arena, 256, 8);

    /* Roll back the temporary allocation instantly */
    arena.pos = checkpoint;

    /* Clean up when done */
    arena_destroy(&arena);
    return 0;
}