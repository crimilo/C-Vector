#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

typedef struct
{
    int i;
    double d;
} my_struct;

void my_add_to_vector(vector(my_struct*) vec, int i, double d)
{
    my_struct *elem = malloc(sizeof(my_struct));
    elem->i = i;
    elem->d = d;
    // Add elem to vec
    vec_push(vec, elem);
    puts("Added!");
}

void my_free_vector(vector(my_struct*) vec)
{
    // Free each heap allocated object
    for (size_t i = 0; i < sizeof(vec); i++)
        free(vec[i]);

    // Free the actual vector
    vec_free(vec);
}

int main(void)
{
    // Create the vector
    vector(my_struct*) vec = NULL;

    // Call to functions that add non-primitive objects to the vector
    my_add_to_vector(vec, 1, 2.0);
    my_add_to_vector(vec, 4, 6.0);

    for (size_t i = 0; i < vec_size(vec); i++)
        printf("%i %g\n", vec[i]->i, vec[i]->d);

    // Done with the vector, freeing it
    my_free_vector(vec);
}