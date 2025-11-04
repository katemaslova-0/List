#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "Data.h"

#define light_green "\"#C0FFC0\""
#define dark_green  "\"#008000\""
#define light_pink  "\"#ffc0f6ff\""
#define dark_pink   "\"#fa5d82ff\""
#define light_red   "\"#ff0000ff\""
#define dark_red    "\"#bd2222ff\""
#define white       "\"#ffffffff\""


void OutputNullCell (FILE * fp, List_t * lst)
{
    assert(fp);
    assert(lst);

    fprintf(fp, "digraph G{\nrankdir=LR;\nf0 [shape=Mrecord; style = filled; ");
    fprintf(fp, "fillcolor = "light_red"; color = "dark_red"; ");
    fprintf(fp, "label = \" head = %d ", (lst->next)[0]);
    fprintf(fp, "| tail = %d | free = %d\", fontcolor = "white"];\n", (lst->prev)[0], lst->free);
}


void OutputArrayCellsNodes (FILE * fp, List_t * lst)
{
    assert(fp);
    assert(lst);

    int capacity = GetCapacity(lst);

    for (int count = 1; count < capacity; count++)
    {
        if ((lst->data)[count] == POISON)
            fprintf(fp, "f%d [shape=Mrecord; style = filled; fillcolor = "light_green"; ", count);
        else
            fprintf(fp, "f%d [shape=Mrecord; style = filled; fillcolor = "light_pink"; ", count);
        fprintf(fp, "color = "dark_green"; label = \"index = %d ", count);
        fprintf(fp, "| value =  %d ", (lst->data)[count]);
        fprintf(fp, "| {prev = %d | next = %d} \"]; \n", (lst->prev)[count], (lst->next)[count]);
    }
}


void OutputHeadAndTailNodes (FILE * fp, List_t * lst) // change name(add free)
{
    assert(fp);
    assert(lst);

    fprintf(fp, "head [shape=Mrecord; style = filled; fillcolor = "light_pink"; ");
    fprintf(fp, "color = "dark_pink"; label = \"HEAD\"; weight=\"100000\"]; \n");
    fprintf(fp, "tail [shape=Mrecord; style = filled; fillcolor = "light_pink"; ");
    fprintf(fp, "color = "dark_pink"; label = \"TAIL\"; weight=\"100000\"]; \n");
    fprintf(fp, "free [shape=Mrecord; style = filled; fillcolor = "light_green"; ");
    fprintf(fp, "color = "dark_green"; label = \"FREE\"; weight=\"100000\"]; \n");
}


void OutputAllEdges (FILE * fp, List_t * lst)
{
    assert(fp);
    assert(lst);

    int capacity = GetCapacity(lst);

    for (int count = 0; count < capacity - 1; count++)
    {
        fprintf(fp, "f%d -> f%d [weight = 100000, style = invis];\n", count, count + 1);

        if ((lst->prev)[count] == -1)
            fprintf(fp, "f%d -> f%d [weight = 100];\n", count, lst->next[count]);
    }
}


void OutputArrayCellsEdges (FILE * fp, List_t * lst)
{
    assert(fp);
    assert(lst);

    int capacity = GetCapacity(lst);

    for (int count = 1; count < capacity; count++)
    {
        if ((lst->data)[count] != POISON && (lst->next)[count] != 0)
        {
            if ((lst->next)[count] <= capacity && (lst->prev)[(lst->next)[count]] == count)
                fprintf(fp, "f%d -> f%d [color = "dark_pink", dir = both];\n", count, (lst->next)[count]);
            else
                fprintf(fp, "f%d -> f%d [penwidth = 5, color = "light_red"];\n", count, (lst->next)[count]);
        }
    }
}


void OutputHeadAndTailEdges (FILE * fp, List_t * lst) // change name (add free)
{
    assert(fp);
    assert(lst);

    fprintf(fp, "head -> f%d [weight = 1; color = "light_pink"];\n", (lst->next)[0]);
    fprintf(fp, "tail -> f%d [weight = 1; color = "light_pink"];\n", (lst->prev)[0]);
    fprintf(fp, "free -> f%d [weight = 1; color = "light_green"];\n", lst->free);
    fprintf(fp, "}");
}
