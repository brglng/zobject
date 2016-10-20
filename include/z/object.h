#ifndef __Z_OBJECT_H__
#define __Z_OBJECT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>

struct ZObject {
  size_t            refcount;
  struct ZClass*    klass;
};

typedef void (*ZConstructor)(void *self, va_list args);
typedef void (*ZDestructor)(void *self);

struct ZClass {
  struct ZObject    super;
  char*             name;
  struct ZClass*    super_class;
  size_t            object_size;
  ZConstructor      object_init;
  ZDestructor       object_finalize;
};

extern struct ZClass *ZObject;
extern struct ZClass *ZClass;

void*   z_new(struct ZClass *type, ...);
void    z_delete(void *self);

void*   z_ref(void *self);
void    z_unref(void *self);

__attribute__((always_inline))
static inline void __z_cleanup(void *self)
{
  z_unref(*(void**)self);
}

void            ZObject_init(struct ZObject *self);
void            ZObject_finalize(struct ZObject *self);

void            ZClass_init(struct ZClass *self);
void            ZClass_finalize(struct ZClass *self);
char*           ZClass_get_name(struct ZClass *self);
struct ZClass*  ZClass_get_super_class(struct ZClass *self);

struct ZClass*  ZObject_get_class(void *self);
struct ZClass*  ZObject_get_super_class(void *self);

#define ZVar __attribute__((cleanup(__z_cleanup))) void *

#ifdef  __cplusplus
}
#endif

#endif /* end of include guard: __Z_OBJECT_H__ */
