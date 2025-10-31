#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "Data.h"

ListErr_t ListCtor (List_t * lst, FILE * fp)
{
    assert(lst);
    assert(fp);

    lst->data = (int *)    calloc (START_SIZE, sizeof(int)); //func
    lst->next = (int *)    calloc (START_SIZE, sizeof(int));
    lst->prev = (int *)    calloc (START_SIZE, sizeof(int));

    lst->log = fp;                  // logfile

    (lst->data)[0] = CANARY;
    (lst->next)[0] = 0;             // head
    (lst->prev)[0] = 0;             // tail

    lst->free = 1;                  // the first free element of array
    lst->capacity = START_SIZE;

    FillDataWithPoison(lst, 1);
    FillNextWithFreeAddresses(lst, 1);
    FillPrevWithStartValue(lst, 1);

    LIST_VERIFY(lst, After);

    return NoError;
}


ListErr_t ListVerify (List_t * lst)
{
    assert(lst);

    ListErr_t error = NoError;

    if ((error = CheckNullPointers(lst)) != NoError)
        return error;
    if ((error = CheckCanaryValue(lst)) != NoError)
        return error;
    if ((error = CheckIfIndexesValid(lst)) != NoError)
        return error;
    if ((error = CheckNextAndPrevConnections(lst)) != NoError)
        return error;
    if ((error = CheckIfCycles(lst)) != NoError)
        return error;

    return error;
}


ListErr_t GraphDump(List_t * lst, Place_t place, const char * func_name, const char * file_name, int num_of_line)
{
    assert(lst);
    assert(func_name);
    assert(file_name);

    MakeGraphCodeFile(lst);

    char * pic_name = GetPicName();
    char * command  = GetCommand(pic_name);
    const char * place_of_call = GetPlace(place);
    FILE * log = GetLog(lst);

    system(command); // FIXME: check return value
    free(command);

    OutputTitle(log, place_of_call, func_name, file_name, num_of_line);
    OutputIndexes(log, lst);
    OutputData(log, lst);
    OutputNext(log, lst);
    OutputPrev(log, lst);
    OutputFree(log, lst);
    OutputImage(log, lst, pic_name);

    free(pic_name);
    return NoError;
}


void MakeGraphCodeFile (List_t * lst)
{
    assert(lst);

    FILE * fp = fopen("graphdump.txt", "w");
    assert(fp); //FIXME: check

    OutputNullCell         (fp, lst);
    OutputArrayCellsNodes  (fp, lst);
    OutputHeadAndTailNodes (fp, lst);
    OutputInvisEdges       (fp, lst);
    OutputArrayCellsEdges  (fp, lst);
    OutputHeadAndTailEdges (fp, lst);

    fclose(fp);
}


ListErr_t ListRealloc (List_t * lst)
{
    assert(lst);

    lst->capacity *= 2;
    lst->data = (int    *) realloc (lst->data, ((size_t) lst->capacity * sizeof(int)));
    lst->next = (int    *) realloc (lst->next, ((size_t) lst->capacity * sizeof(int)));
    lst->prev = (int    *) realloc (lst->prev, ((size_t) lst->capacity * sizeof(int)));

    if (lst->data == NULL || lst->next == NULL || lst->prev == NULL)
        return MemAllocError;

    FillDataWithPoison(lst, lst->capacity / 2);
    FillPrevWithStartValue(lst, lst->capacity / 2);
    FillNextWithFreeAddresses(lst, lst->capacity / 2);

    return NoError;
}


void ListDtor (List_t * lst)
{
    assert(lst);

    free(lst->data);
    free(lst->next);
    free(lst->prev);
}
