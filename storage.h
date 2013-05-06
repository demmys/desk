#ifndef STORAGE_H_INCLUDE
#define STORAGE_H_INCLUDE
#include <stdio.h>
#include <stdlib.h>
#define DEFAULT_PAGE_SIZE 1024
#define max(x, y) (((x) > (y)) ? (x) : (y))

// set biggest type
typedef union{
    long l_dummy;
    double d_dummy;
    void *p_dummy;
} Cell;

typedef struct MemoryPage_tag{
    int cell_num;
    int use_cell_num;
    struct MemoryPage_tag *next;
    Cell cell[1];
} MemoryPage;

typedef struct {
    MemoryPage *page_list;
    int current_page_size;
} Storage;

/*
 * storage.c function prototype
 */
Storage *open_storage(int page_size);
void *storage_malloc(Storage *storage, size_t size);
void dispose_storage(Storage *storage);

#endif // STORAGE_H_INCLUDE
