#include "storage.h"

Storage *open_storage(int page_size){
    Storage *storage;

    storage = malloc(sizeof(Storage));
    storage->page_list = NULL;
    if(page_size > 0){
        storage->current_page_size = page_size;
    } else{
        storage->current_page_size = DEFAULT_PAGE_SIZE;
    }

    return storage;
}

void *storage_malloc(Storage *storage, size_t size){
    int cell_num, alloc_cell_num;
    MemoryPage *new_page;
    void *p;

    cell_num = ((size - 1) / sizeof(Cell)) + 1;

    if(storage->page_list && (storage->page_list->use_cell_num + cell_num) < storage->page_list->cell_num){
        p = &(storage->page_list->cell[storage->page_list->use_cell_num]);
        storage->page_list->use_cell_num += cell_num;
    } else{
        alloc_cell_num = max(cell_num, storage->current_page_size);
        new_page = malloc(sizeof(MemoryPage) + sizeof(Cell) * (alloc_cell_num - 1));
        new_page->next = storage->page_list;
        new_page->cell_num = alloc_cell_num;
        storage->page_list = new_page;
        p = &(new_page->cell[0]);
        new_page->use_cell_num = cell_num;
    }

    return p;
}

void dispose_storage(Storage *storage){
    MemoryPage *temp;
    
    while(storage->page_list){
        temp = storage->page_list->next;
        free(storage->page_list);
        storage->page_list = temp;
    }
    free(storage);
}
