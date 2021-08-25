#pragma warning(disable : 28182)

#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <assert.h>

#define vector(type) type *

typedef struct _v_h
{
    size_t size; // Current stored elements
    size_t cap;  // Current max capacity
} _v_h;

#define _vec_get_header(vec) (((_v_h*)vec) - 1)

#define _vec_capacity(vec) (vec ? _vec_get_header(vec)->cap : 0)

#define _vec_set_capacity(vec, cp) _vec_get_header(vec)->cap = cp

#define _vec_set_size(vec, sz) _vec_get_header(vec)->size = sz

#define vec_size(vec) (vec ? _vec_get_header(vec)->size : 0)

#define vec_back(vec) vec[vec_size(vec)]

#define vec_mid(vec) vec[vec_size(vec) / 2]

#define vec_front(vec) vec[0]

#define vec_is_emtpy(vec) !vec_size(vec)

#define vec_begin(vec) vec

#define vec_end(vec) (vec + vec_size(vec))

#define vec_iterator(type) type *

#define vec_pop(vec) vec_erase(vec, vec_size(vec))

#define _vec_create(vec, bytes)                                        \
    do {                                                               \
        _v_h *h = (_v_h*)malloc(bytes * sizeof(*vec) + sizeof(_v_h));  \
        assert(h);                                                     \
        vec = (void*)&h[1];                                            \
        _vec_set_capacity(vec, 1);                                     \
        _vec_set_size(vec, 0);                                         \
    } while (0)

#define _vec_resize(vec, cap)                                          \
    do {                                                               \
        _v_h *h = (_v_h*)realloc(_vec_get_header(vec),                 \
            cap * sizeof(*vec) + sizeof(_v_h));                        \
        assert(h);                                                     \
        vec = (void*)&h[1];                                            \
        _vec_set_capacity(vec, cap);                                   \
    } while (0)

#define vec_free(vec)                                                  \
    do {                                                               \
        free(_vec_get_header(vec));                                    \
        vec = NULL;                                                    \
    } while (0)

#define vec_reserve(vec, bytes)                                        \
    do {                                                               \
        assert(bytes >= 0);                                            \
        if (bytes != 0) {                                              \
            if (!vec)                                                  \
                _vec_create(vec, bytes);                               \
            else                                                       \
                _vec_resize(vec, bytes);                               \
        }                                                              \
    } while (0)

#define vec_push(vec, data)                                            \
    do {                                                               \
        if (vec_size(vec) == _vec_capacity(vec)) {                     \
            if (!vec)                                                  \
                _vec_create(vec, 1);                                   \
            else                                                       \
                _vec_resize(vec, vec_size(vec) * 2);                   \
        }                                                              \
        vec[vec_size(vec)] = data;                                     \
        _vec_set_size(vec, vec_size(vec) + 1);                         \
    } while (0)

#define vec_erase(vec, idx)                                            \
    do {                                                               \
        for (size_t i = idx; i < vec_size(vec) - 1; i++)               \
            vec[i] = vec[i + 1];                                       \
        _vec_set_size(vec, vec_size(vec) - 1);                         \
    } while (0)

#define vec_insert(vec, idx, data)                                     \
    do {                                                               \
        assert(idx >= 0 && idx <= vec_size(vec));                      \
        if (!vec) {                                                    \
            vec_push(vec, data);                                       \
        } else {                                                       \
            if (vec_size(vec) == _vec_capacity(vec))                   \
                _vec_resize(vec, vec_size(vec) * 2);                   \
            for (size_t i = vec_size(vec); i > (size_t)idx; i--)       \
                vec[i] = vec[i - 1];                                   \
            vec[idx] = data;                                           \
            _vec_set_size(vec, vec_size(vec) + 1);                     \
        }                                                              \
    } while (0)

#define vec_insert_many(dst, idx, src, src_len)                        \
    do {                                                               \
        assert(idx >= 0 && idx <= vec_size(dst));                      \
        if (!dst) {                                                    \
            for (size_t i = 0; i < src_len; i++)                       \
                vec_push(dst, src[i]);                                 \
        } else {                                                       \
            if (vec_size(dst) + src_len >= _vec_capacity(dst)) {       \
                size_t resize_by = _vec_capacity(dst) * 2;             \
                while (resize_by < vec_size(dst) + src_len)            \
                    resize_by *= 2;                                    \
                _vec_resize(dst, resize_by);                           \
            }                                                          \
            size_t i;                                                  \
            for (i = vec_size(dst) + src_len - 1; i > idx; i--)        \
                dst[i] = dst[i - src_len];                             \
            for (size_t j = idx, i = 0; j < src_len + idx; j++, i++)   \
                dst[j] = src[i];                                       \
            _vec_set_size(dst, vec_size(dst) + src_len);               \
        }                                                              \
    } while (0)

#endif // VECTOR_H