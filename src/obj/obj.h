
#ifndef ECGEN_OBJ_H
#define ECGEN_OBJ_H

#include "misc/types.h"
#include "util/memory.h"

#define OBJ(obj_name, obj_type, copy_func, clone_func)                  \
	obj_type *obj_name##_new() { return try_calloc(sizeof(obj_type)); } \
	obj_type *obj_name##_new_copy(const obj_type *src) {                \
		obj_type *result = obj_name##_new();                            \
		return copy_func(src, result);                                  \
	}                                                                   \
	obj_type *obj_name##_new_clone(const obj_type *src) {               \
		obj_type *result = obj_name##_new();                            \
		return clone_func(src, result);                                 \
	}

#define OBJS(obj_name, obj_type, copy_func, clone_func)                  \
	obj_type **obj_name##s_new(size_t num) {                             \
		return try_calloc(num * sizeof(obj_type));                       \
	}                                                                    \
	obj_type **obj_name##s_copy(obj_type **const src, obj_type **dest,   \
	                            size_t num) {                            \
		for (size_t i = 0; i < num; ++i) {                               \
			dest[i] = obj_name##_new_copy(src[i]);                       \
		}                                                                \
		return dest;                                                     \
	}                                                                    \
	obj_type **obj_name##s_clone(obj_type **const src, obj_type **dest,  \
	                             size_t num) {                           \
		for (size_t i = 0; i < num; ++i) {                               \
			dest[i] = obj_name##_new_clone(src[i]);                      \
		}                                                                \
		return dest;                                                     \
	}                                                                    \
	obj_type **obj_name##s_new_copy(obj_type **const src, size_t num) {  \
		obj_type **result = obj_name##s_new(num);                        \
		return obj_name##s_copy(src, result, num);                       \
	}                                                                    \
	obj_type **obj_name##s_new_clone(obj_type **const src, size_t num) { \
		obj_type **result = obj_name##s_new(num);                        \
		return obj_name##s_clone(src, result, num);                      \
	}

#define OBJ_H(obj_name, obj_type)                       \
	obj_type *obj_name##_new();                         \
	obj_type *obj_name##_new_copy(const obj_type *src); \
	obj_type *obj_name##_new_clone(const obj_type *src);

#define OBJS_H(obj_name, obj_type)                                             \
	obj_type **obj_name##s_new(size_t num);                                    \
	obj_type **obj_name##s_copy(obj_type **src, obj_type **dest, size_t num);  \
	obj_type **obj_name##s_clone(obj_type **src, obj_type **dest, size_t num); \
	obj_type **obj_name##s_new_copy(obj_type **src, size_t num);               \
	obj_type **obj_name##s_new_clone(obj_type **src, size_t num);

#endif  // ECGEN_OBJ_H
