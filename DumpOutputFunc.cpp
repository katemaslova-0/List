#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "Data.h"


void OutputTitle (FILE * log, const char * place, const char * func_name, const char * file_name, int num_of_line)
{
    assert(func_name);
    assert(file_name);

    fprintf(log, "<pre>\n<h3> DUMP <font color=red>%s</font> %s </h3>\n", place, func_name);
    fprintf(log, "List {%s:%d}\n\n     ", file_name, num_of_line);
}


void OutputIndexes (FILE * log, List_t * lst)
{
    assert(lst);

    int capacity = GetCapacity(lst);
    for (int count = 0; count < capacity; count++)
        fprintf(log, "%5d", count);
}


void OutputData (FILE * log, List_t * lst)
{
    assert(lst);

    int capacity = GetCapacity(lst);
    fprintf(log, "\n\ndata:");
    for (int count = 0; count < capacity; count++)
        fprintf(log, "%5d", (lst->data)[count]);
}


void OutputNext (FILE * log, List_t * lst)
{
    assert(lst);

    int capacity = GetCapacity(lst);
    fprintf(log, "\n\nnext:");
    for (int count = 0; count < capacity; count++)
        fprintf(log, "%5d", (lst->next)[count]);
}


void OutputPrev (FILE * log, List_t * lst)
{
    assert(lst);

    int capacity = GetCapacity(lst);
    fprintf(log, "\n\nprev:");
    for (int count = 0; count < capacity; count++)
        fprintf(log, "%5d", (lst->prev)[count]);
}


void OutputFree (FILE * log, List_t * lst)
{
    assert(lst);

    fprintf(log, "\n\nfree: %d\n", lst->free);
}


void OutputImage (FILE * log, List_t * lst, char * pic_name)
{
    assert(lst);
    assert(pic_name);

    fprintf(log, "\n<h3></font>IMAGE:<h3> <img src=%s width=1600px>\n\n\n", pic_name);
}
