#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <assert.h>

#ifdef MSVC
#define TYPE(x) decltype(x)
#else
#define TYPE(x) typeof(x)
#endif

#define vector(type) type *

/*   header -> +--+--+--+--+--+
               |     size     |
               +--+--+--+--+--+         The header struct "lives" before the pointer
               |   capacity   |         provided to the user and contains the metadata
     user's -> +--+--+--+--+--+         of the vector (a.k.a. size and capacity). The
     pointer   |              |         user should not be using the header to manipulate
               |              |         its values but use macros instead.
               |     array    |
               |              |
               |              |
               +--+--+--+--+--+   */

typedef struct _v_h
{
    size_t size; // Current stored elements
    size_t cap;  // Max capacity, once reached it resizes
} _vec_header;

typedef struct _v_h _v_h;

#define _vec_get_header(vec) (((_v_h*)vec) - 1)

#define _vec_capacity(vec) (vec ? _vec_get_header(vec)->cap : 0)

#define _vec_set_capacity(vec, cp) _vec_get_header(vec)->cap = cp

#define _vec_set_size(vec, sz) _vec_get_header(vec)->size = sz

#define vec_size(vec) (vec ? _vec_get_header(vec)->size : 0)

#define vec_back(vec) vec[vec_size(vec)]

#define vec_mid(vec) vec[vec_size(vec) / 2]

#define vec_front(vec) vec[0]

#define vec_is_emtpy(vec) !vec_size(vec)

#define _vec_create(vec)                                            \
    do {                                                            \
        _v_h *h = (_v_h*)malloc(sizeof(*vec) + sizeof(_v_h));       \
        assert(h);                                                  \
        vec = (void*)&h[1];                                         \
        _vec_set_capacity(vec, 1);                                  \
    } while (0)

#define _vec_resize(vec, cap)                                                                  \
    do {                                                                                       \
        _v_h *h = (_v_h*)realloc(_vec_get_header(vec), cap * sizeof(*vec) + sizeof(_v_h));     \
        assert(h);                                                                             \
        vec = (void*)&h[1];                                                                    \
        _vec_set_capacity(vec, cap);                                                           \
    } while (0)

// Always free a vector once you are done with it.
#define vec_free(vec)                   \
    do {                                \
        free(_vec_get_header(vec));     \
        vec = NULL;                     \
    } while (0)

#define vec_push(vec, data)                             \
    do {                                                \
        if (vec_size(vec) == _vec_capacity(vec)) {      \
            if (!vec) {                                 \
                _vec_create(vec);                       \
            } else {                                    \
                _vec_resize(vec, vec_size(vec) * 2);    \
            }                                           \
        }                                               \
        vec[vec_size(vec)] = data;                      \
        _vec_set_size(vec, vec_size(vec) + 1);          \
    } while (0)

#define vec_erase(vec, idx)                             \
    do {                                                \
        for (int i = idx; i < vec_size(vec) - 1; i++)   \
        vec[i] = vec[i + 1];                            \
        _vec_set_size(vec, vec_size(vec) - 1)           \
    } while (0)

#define vec_pop(vec) ({                                 \
    TYPE(*vec) ret = vec[vec_size(vec)];                \
    vec_erase(vec, vec_size(vec));                      \
    ret;                                                \
})

// Reserving memory before adding elements to the vector
// can eventually increase slightly the performance
// preventing the vector to resize many times.
#define vec_reserve(vec, bytes)                                               \
    if (bytes > 1) {                                                          \
        if (!vec) {                                                           \
            _v_h *h = (_v_h*)malloc(bytes * sizeof(*vec) + sizeof(_v_h));     \
            assert(h);                                                        \
            vec = (void*)&h[1];                                               \
            _vec_set_capacity(vec, bytes);                                    \
        } else {                                                              \
            _vec_resize(vec, bytes);                                          \
        }                                                                     \
    }

#define vec_insert(vec, idx, data)                            \
    if (!vec) {                                               \
        vec_push(vec, data);                                  \
    } else {                                                  \
        if (vec_size(vec) == _vec_capacity(vec))              \
            _vec_resize(vec, vec_size(vec) * 2);              \
        for (size_t i = vec_size(vec); i > (size_t)idx; i--)  \
            vec[i] = vec[i - 1];                              \
        vec[idx] = data;                                      \
        _vec_set_size(vec, vec_size(vec) + 1);                \
    }

// Insert an array or another vector into the destination vector
#define vec_insert_many(vec_dst, idx, src, src_len)                     \
    if (!vec_dst) {                                                     \
        for (size_t i = 0; i < src_len; i++)                            \
            vec_push(vec_dst, src[i]);                                  \
    } else {                                                            \
        if (vec_size(vec_dst) + src_len >= _vec_capacity(vec_dst)) {    \
        size_t resize_by = _vec_capacity(vec_dst) * 2;                  \
        while (resize_by < vec_size(vec_dst) + src_len)                 \
            resize_by *= 2;                                             \
        _vec_resize(vec_dst, resize_by);                                \
        }                                                               \
        for (size_t i = vec_size(vec_dst) + len - 1; i > idx; i--) {    \
            vec_dst[i] = vec_dst[i - src_len];                          \
        }                                                               \
        int i = 0;                                                      \
        for (size_t j = idx; j < src_len + idx; j++, i++)               \
            vec_dst[j] = src[i];                                        \
        _vec_set_size(vec_dst, vec_size(vec_dst) + src_len);            \
    }

#endif // VECTOR_H