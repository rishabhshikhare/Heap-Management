#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>
#define HeapSize 10000
char Heap[HeapSize];




typedef struct MetaData
{
    size_t size; // size_t is used specifically in memory allocation,deletion,array sizes,indexing,etc. So basically it stores size of each block.
    char status; //'f' for free || 'a' for allocated
    struct MetaData *next; 
}MetaBlock;




MetaBlock *heap_block_ptr = (void *)Heap; //A pointer to the first MetaBlock in the heap, initialized to the start of the Heap array.




void Initialize()
{
    
    heap_block_ptr->size = (HeapSize-(sizeof(MetaBlock)));
    printf("Size of Meta Data structure is %zu bytes \n",sizeof(MetaBlock));
    heap_block_ptr->status = 'f';
    heap_block_ptr->next = NULL;
    printf("Heap block initialized \n");
    printf("Address of starting of heap: %p \n",heap_block_ptr);
    printf("Ending address: %p \n",((heap_block_ptr->next)-1));
}




void * Allocate(size_t size_to_be_allocated)
{
    MetaBlock *trail,*lead;
    void *ret_ptr;
    trail = lead = ret_ptr = (void *)Heap;
    if((lead->next == NULL) && (lead->size < size_to_be_allocated))
    {
        printf("No free space left to allocate %zu bytes of memory \n",size_to_be_allocated);
        ret_ptr=NULL;
    }
    else
    {
        while((lead != NULL) && ((lead->size < size_to_be_allocated) || (lead->status == 'a')))
        {
            trail = lead;
            lead = lead->next;
        }
        if(lead->size == size_to_be_allocated)
        {
            lead->status = 'a';
            trail = lead;
            lead = lead->next;
            ret_ptr = lead;
            printf("Heap block having size exactly equal to %zu bytes is allocated \n",size_to_be_allocated);
        }
        else
        {
            MetaBlock *new_block = (void *)(((void *)lead) + (sizeof(MetaBlock)) + (size_to_be_allocated));
            new_block->size = ((lead->size) - (size_to_be_allocated) - (sizeof(MetaBlock)));
            new_block->next = lead->next;
            new_block->status = 'f';
            lead->size = size_to_be_allocated;
            lead->next = new_block;
            lead->status = 'a';
            ret_ptr = new_block;
        }
    }
    return ret_ptr;
}



void Merge()
{
    MetaBlock *merge_ptr, *temp;
    merge_ptr = heap_block_ptr;

    while (merge_ptr->next != NULL)
    {
        if ((merge_ptr->status == 'f') && ((merge_ptr->next)->status == 'f'))
        {
            merge_ptr->size += (merge_ptr->next->size) + sizeof(MetaBlock);
            merge_ptr->next = merge_ptr->next->next;
        }
        else
        {
            temp = merge_ptr;
            merge_ptr = merge_ptr->next;
        }
    }
}





void Free(void *free_ptr)
{
    if((((void *)Heap) <= (free_ptr)) && (((void *)(Heap + 10000)) >= (free_ptr)))
    {
        MetaBlock *ptr = (void *)Heap;
        while(ptr->next != free_ptr)
        {
            ptr = ptr->next;
        }
        ptr->status = 'f';
        Merge();
    }
    else
    {
        printf("Entered block of address to be freed is invalid \n");
    }
}




void DisplayHeap()
{
    MetaBlock *traverse_ptr = (void *)Heap;
    size_t index = 0;

    while (traverse_ptr != NULL)
    {
        printf("%zu - %zu \t", index, index + (traverse_ptr->size) - 1);
        printf("%c \n", traverse_ptr->status);
        index += (traverse_ptr->size);
        traverse_ptr = traverse_ptr->next;
    }
}





int main()
{
    Initialize();
    int *p = (int *)Allocate(40 * sizeof(int));
    // char *c = (char *)Allocate(200 * sizeof(char));
    // int *i = (int *)Allocate(100 * sizeof(int));
    // printf("Heap memory status before freeing is as follows: \n");
    // DisplayHeap();
    // Free(i);
    // printf("Heap memory status after freeing i is as follows: \n");
    // DisplayHeap();
    // Free(p);
    // printf("Heap memory status after freeing p is as follows: \n");
    // DisplayHeap();
    // Free(c);
    // printf("Heap memory status after freeing c is as follows: \n");
    printf("After allocation of memory: \n");
    DisplayHeap();
    Free(p);
    printf("After freeing memory: \n");
    DisplayHeap();
    return 0;
}
