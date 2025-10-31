#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "Data.h"


void OutputNullCell (FILE * fp, List_t * lst)
{
    assert(fp);
    assert(lst);

    fprintf(fp, "digraph G{\nrankdir=LR;\nf0 [shape=Mrecord; style = filled; ");
    fprintf(fp, "fillcolor = \"#C0FFC0\"; color = \"#008000\"; ");
    fprintf(fp, "label = \" head = %d ", (lst->next)[0]);
    fprintf(fp, "| tail = %d | free = %d\"];\n", (lst->prev)[0], lst->free);

}


void OutputArrayCellsNodes (FILE * fp, List_t * lst)  //FIXME: define for colors
{
    assert(fp);
    assert(lst);

    int capacity = GetCapacity(lst);

    for (int count = 1; count < capacity; count++)
    {
        fprintf(fp, "f%d [shape=Mrecord; style = filled; fillcolor = \"#C0FFC0\"; color = \"#008000\"; label = \"index = %d | value =  %d | {prev = %d | next = %d} \"]; \n", count, count, (lst->data)[count], (lst->prev)[count], (lst->next)[count]);
    }
}


void OutputHeadAndTailNodes (FILE * fp, List_t * lst)
{
    assert(fp);
    assert(lst);

    fprintf(fp, "head [shape=Mrecord; style = filled; fillcolor = \"#ffc0f6ff\"; ");
    fprintf(fp, "color = \"#db36b7ff\"; label = \"HEAD\"; weight=\"100000\"]; \n");
    fprintf(fp, "tail [shape=Mrecord; style = filled; fillcolor = \"#ffc0f6ff\"; ");
    fprintf(fp, "color = \"#db36b7ff\"; label = \"TAIL\"; weight=\"100000\"]; \n");
}


void OutputInvisEdges (FILE * fp, List_t * lst)
{
    assert(fp);
    assert(lst);

    int capacity = GetCapacity(lst);

    for (int count = 0; count < capacity - 1; count++)
    {
        fprintf(fp, "f%d -> f%d [weight = 100000, style = invis];\n", count, count + 1);
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
                fprintf(fp, "f%d -> f%d [color = \"#f63f6aff\", dir = both];\n", count, (lst->next)[count]);
            else
                fprintf(fp, "f%d -> f%d [color = \"#ff0000ff\"];\n", count, (lst->next)[count]);
        }
    }
}


void OutputHeadAndTailEdges (FILE * fp, List_t * lst)
{
    assert(fp);
    assert(lst);

    fprintf(fp, "head -> f%d [weight = 1; color = \"#ffc0f6ff\"];\n", (lst->next)[0]);
    fprintf(fp, "tail -> f%d [weight = 1; color = \"#ffc0f6ff\"];\n", (lst->prev)[0]);
    fprintf(fp, "}");
}
