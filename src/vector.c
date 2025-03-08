#include "prototypes.h"

void init_vector(vector_T* vector, float x, float y)
{
    vector->x = x;
    vector->y = y;
}

#if GUI
void print_vector(vector_T* vector)
{
    printf("(%f,%f)", vector->x, vector->y);
}
#endif