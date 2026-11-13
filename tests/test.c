#include <stdio.h>
#include "arena.h"

int main(void) {
    Arena arena;
    int *numbers;
    int i;
    void *aligned_ptr;
    u64 checkpoint;
    void *temp_block;

    printf("--- Running Arena Allocator Tests ---\n");

    arena = arena_init(4096); /* 4 KB arena */
    ASSERT(arena.data != NULL);
    ASSERT(arena.cap == 4096);
    ASSERT(arena.pos == 0);
    printf("[PASS] Arena initialization\n");

    /* Test Basic Push & Zero-Initialization */
    numbers = arena_push(&arena, sizeof(int) * 10, sizeof(int));
    ASSERT(numbers != NULL);
    
    for (i = 0; i < 10; ++i) {
        ASSERT(numbers[i] == 0); /* Verify ZeroMemory worked */
        numbers[i] = (i + 1) * 10;
    }
    printf("[PASS] Basic push & zero-initialization\n");

    /* Test Strict Alignment */
    aligned_ptr = arena_push(&arena, 64, 16);
    ASSERT(aligned_ptr != NULL);
    ASSERT(((u64)aligned_ptr % 16) == 0);
    printf("[PASS] Alignment verification (16-byte)\n");

    /* Test Checkpoint Rollback / Pop */
    checkpoint = arena.pos;
    
    temp_block = arena_push(&arena, 512, 8);
    ASSERT(temp_block != NULL);
    ASSERT(arena.pos > checkpoint);
    
    arena.pos = checkpoint;
    ASSERT(arena.pos == checkpoint);
    printf("[PASS] Checkpoint rollback / pop\n");

    /* Test Full Reset */
    arena_reset(&arena);
    ASSERT(arena.pos == 0);
    printf("[PASS] Arena reset\n");

    /* Test Destruction */
    arena_destroy(&arena);
    ASSERT(arena.data == NULL);
    ASSERT(arena.cap == 0);
    ASSERT(arena.pos == 0);
    printf("[PASS] Arena destruction\n");

    printf("\nAll tests passed successfully!\n");
    return 0;
}