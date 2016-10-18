#ifndef __Z_OBJECT_H__
#define __Z_OBJECT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>

typedef struct ZObject ZObject;
typedef struct ZClass ZClass;

struct ZObject {
  size_t    refcount;
  ZClass*   klass;
};

typedef void (*ZConstructor)(void *self, va_list args);
typedef void (*ZDestructor)(void *self);

struct ZClass {
  ZObject       super;
  char*         name;
  ZClass*       super_class;
  size_t        object_size;
  ZConstructor  object_init;
  ZDestructor   object_finalize;
};

extern ZClass __ZObject;
extern ZClass __ZClass;

#define z_new(type, ...) __z_new(&__##type, ##__VA_ARGS__)
void*   __z_new(ZClass *type, ...);
void    z_delete(void *self_);

void*   z_ref(void *self);
void    z_unref(void *self);

__attribute__((always_inline))
static inline void __z_cleanup(void *self)
{
  z_unref(*(void**)self);
}

void    z_object_init(ZObject *self);
void    z_object_finalize(ZObject *self);

void    z_class_init(ZClass *self);
void    z_class_finalize(ZClass *self);
char*   z_class_get_name(ZClass *self);
ZClass* z_class_get_super_class(ZClass *self);

ZClass* z_object_get_class(void *self);
ZClass* z_object_get_super_class(void *self);

#define ZVar __attribute__((cleanup(__z_cleanup))) void *

#ifdef  __cplusplus
}
#endif

#endif /* end of include guard: __Z_OBJECT_H__ */
