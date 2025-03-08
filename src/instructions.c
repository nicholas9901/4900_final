#include "prototypes.h"

void init_instructions(
    instructions_T* instructions,
    direction_T* list,
    int num)
{
    instructions->list    = list;
    instructions->num     = num;
    instructions->current = 0;
}