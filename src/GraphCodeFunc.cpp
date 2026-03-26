#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

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

    fprintf(fp, "digraph G{\nrankdir=LR;\nf%p [shape=Mrecord; style = filled; ", lst->root);
    fprintf(fp, "fillcolor = "light_red"; color = "dark_red"; ");
    fprintf(fp, "label = \" address = %p | data = %d | head = %p ", lst->root, (lst->root)->data, (lst->root)->next);
    fprintf(fp, "| tail = %p \", fontcolor = "white"];\n", (lst->root)->prev);
}


void OutputArrayCellsNodes (FILE * fp, List_t * lst)
{
    assert(fp);
    assert(lst);

    for (ListElem_t * ptr = (lst->root)->next; ptr != lst->root; ptr = ptr->next)
    {
        fprintf(fp, "f%p [shape=Mrecord; style = filled; fillcolor = "light_pink"; ", ptr);
        fprintf(fp, "color = "dark_green"; label = \"address = %p ", ptr);
        fprintf(fp, "| value =  %d ", ptr->data);
        fprintf(fp, "| {prev = %p | next = %p} \"]; \n", ptr->prev, ptr->next);
    }
}


void OutputHeadAndTailNodes (FILE * fp) // change name
{
    assert(fp);

    fprintf(fp, "head [shape=Mrecord; style = filled; fillcolor = "light_pink"; ");
    fprintf(fp, "color = "dark_pink"; label = \"HEAD\"; weight=\"100000\"]; \n");
    fprintf(fp, "tail [shape=Mrecord; style = filled; fillcolor = "light_pink"; ");
    fprintf(fp, "color = "dark_pink"; label = \"TAIL\"; weight=\"100000\"]; \n");
}


void OutputAllEdges (FILE * fp, List_t * lst)
{
    assert(fp);
    assert(lst);

    for (ListElem_t * ptr = (lst->root)->next; ptr != (lst->root)->prev; ptr = ptr->next)
        fprintf(fp, "f%p -> f%p [weight = 100000, style = invis];\n", ptr, ptr->next);
}


void OutputArrayCellsEdges (FILE * fp, List_t * lst)
{
    assert(fp);
    assert(lst);

    for (ListElem_t * ptr = (lst->root)->next; ptr != (lst->root)->prev; ptr = ptr->next)
    {
        if ((ptr->next)->prev == ptr)
            fprintf(fp, "f%p -> f%p [color = "dark_pink", dir = both];\n", ptr, ptr->next);
        else
            fprintf(fp, "f%p -> f%p [penwidth = 5, color = "light_red"];\n", ptr, ptr->next);
    }
}


void OutputHeadAndTailEdges (FILE * fp, List_t * lst) // change name (add free)
{
    assert(fp);
    assert(lst);

    fprintf(fp, "head -> f%p [weight = 1; color = "light_pink"];\n", (lst->root)->next);
    fprintf(fp, "tail -> f%p [weight = 1; color = "light_pink"];\n", (lst->root)->prev);
    fprintf(fp, "}");
}
