# c-vector
### Dynamic array in C
A clean, simple to use, macro-based vector library written in C.<br>
### How to use it
```c
int main(void)
{
    // Define the vector object
    vector(int) my_vec = NULL;
    
    // Add an element
    vec_push(my_vec, 3);
    
    // Access elements like normal arrays
    printf("%d\n", my_vec[0]);

    // Done with it, free memory
    vec_free(my_vec);
}
```
### How it works
The vector object created with `vector(int) my_vec = NULL;` is just a pointer to the specified type (`int*` in this case).
The vector metadata (size and capacity) is stored in a block of memory (called header) before the user's pointer.
The user shouldn't use the header to access or modify the vector metadata but use the provided macros instead.
```
header -> +--+--+--+--+--+
          |     size     |        /* Current stored elements */
          +--+--+--+--+--+
          |   capacity   |        /* Current maximum capacity */
user's -> +--+--+--+--+--+
pointer   |              |
          |              |
          |     array    |        /* Array containing data */
          |              |
          |              |
          +--+--+--+--+--+
```
### Examples
##### Non-primitive types
```c
typedef struct { ... } my_struct;

int main(void)
{
    vector(my_struct*) my_vec = NULL;
    
    my_struct *obj = malloc(...);
    
    // Just like primitive types!
    vec_push(my_vec, obj);
}
```
##### Function arguments
```c
void foo(vector(my_struct*) my_vec)
{
    printf(..., my_vec[0]->example);
}
```
##### Memory optimization
```c
int main(void)
{
    vector(int) my_vec = NULL;
    
    // Pre-allocate memory
    vec_reserve(my_vec, 1024);
    
    for (int i = 0; i < 256; i++)
      vec_push(my_vec, i);
    // Vector was already big enough to store 256 ints
    // no reallocations were made!
}
```
##### Insert from another vector/array
```c
int main(void)
{
    vector(int) my_vec = NULL;
    vector(int) another_vec = NULL;
    
    vec_push(another_vec, 1);
    vec_push(another_vec, 2);
    
    vec_insert_many(my_vec, 0, another_vec, vec_size(another_vec));
    // my_vec now contains 1 and 2
}
```
For other functionalities check vector.h.
