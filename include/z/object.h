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

extern struct ZClass* ZObject(void);
extern struct ZClass* ZClass(void);

void*   z_new(struct ZClass *type, ...);
void    z_delete(void *self);

void*   z_ref(void *self);
void    z_unref(void *self);

__attribute__((always_inline))
static inline void __z_cleanup(void *self)
{
  z_unref(*(void**)self);
}

void            ZObject_init(void *self, va_list args);
void            ZObject_finalize(void *self);

void            ZClass_init(void *self, va_list args);
void            ZClass_finalize(void *self);
char*           ZClass_get_name(struct ZClass *self);
struct ZClass*  ZClass_get_super_class(struct ZClass *self);

struct ZClass*  ZObject_get_class(void *self);
struct ZClass*  ZObject_get_super_class(void *self);

#define Z_DECLARE_CLASS(name) \
  struct ZClass* name##Class(void); \
  struct ZClass* name(void);

#define _Z_TO_STRING(x) #x

#define _Z_DEFINE_CLASS(name, super_name)               \
  struct ZClass *_##name##Class = NULL;                 \
  struct ZClass* name##Class(void) {                    \
    if (!_##name##Class) {                              \
      _##name##Class = z_new(ZClass(),                  \
                           _Z_TO_STRING(name##Class),   \
                           ZClass(),                    \
                           sizeof(struct name##Class),  \
                           name##Class_init,            \
                           name##Class_finalize);       \
    }                                                   \
    return _##name##Class;                              \
  }                                                     \
  struct ZClass *_##name = NULL;                        \
  struct ZClass* name(void) {                           \
    if (!_##name) {                                     \
      _##name = z_new(name##Class(),                    \
                    #name,                              \
                    super_name(),                       \
                    sizeof(struct name),                \
                    name##_init,                        \
                    name##_finalize);                   \
    }                                                   \
    return _##name;                                     \
  }

#define Z_DEFINE_CLASS(name, super_name) _Z_DEFINE_CLASS(name, super_name)

#define ZRef(klass) __attribute__((cleanup(__z_cleanup))) struct klass *

#ifdef  __cplusplus
}
#endif

#endif /* end of include guard: __Z_OBJECT_H__ */
