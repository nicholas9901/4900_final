#include "prototypes.h"

void init_instructions(
    instructions_T* instructions,
    direction_T* list,
    int num)
{
    instructions->list    = list;
    instructions->num     = num;
    instructions->current = 0;
    instructions->next    = (instructions->current + 1) % instructions->num;
}

void next_instruction(instructions_T* instructions)
{
    instructions->current = (instructions->current + 1) % instructions->num;
    instructions->next    = (instructions->current + 1) % instructions->num;
}