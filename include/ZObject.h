#ifndef __Z_OBJECT_H__
#define __Z_OBJECT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

#define Z_MAGIC 'ZOBJ'

typedef void (*ZConstructor)(void *self, va_list args);
typedef void (*ZDestructor)(void *self);

extern void *ZObject;
extern void *ZType;
void *ZObject_initType(void);
void *ZType_initType(void);

struct ZObject {
  uint32_t       magic;
  struct ZType  *type;
};

struct ZType {
  struct ZObject     super;
  char              *name;
  void              *superType;
  size_t             objectSize;
  ZConstructor       objectInit;
  ZDestructor        objectFinalize;
};

static inline bool Z_isObject(void *_self);
static inline void *Z_cast(void *type, void *self);

void ZObject_init(void *self, va_list args);
void ZObject_finalize(void *self);

static inline void *ZObject_getType(void *_self) {
  assert(Z_isObject(_self));
  struct ZObject *self = _self;
  return self->type;
}

static inline void *ZObject_getSuperType(void *_self) {
  assert(Z_isObject(_self));
  struct ZObject *self = _self;
  return self->type->superType;
}

void     ZType_init(void *self, va_list args);
void     ZType_finalize(void *self);

static inline char* ZType_getName(void *_self) {
  struct ZType *self = Z_cast(ZType, _self);
  return self->name;
}

static inline void *ZType_getSuperType(void *_self) {
  struct ZType *self = Z_cast(ZType, _self);
  return self->superType;
}

void    *_Z_new(void *type, ...);
#define  Z_new(type, ...) _Z_new(type##_initType(), ##__VA_ARGS__)
void     Z_delete(void *self);

static inline void _Z_cleanup(void *pSelf) {
  assert(pSelf);
  void *self = *(void**)pSelf;
  Z_delete(self);
}

static inline bool Z_isObject(void *_self) {
  assert(_self);
  struct ZObject *self = _self;
  return self->magic == Z_MAGIC;
}

static inline void *Z_typeOf(void *_self) {
  return ZObject_getType(_self);
}

static inline bool Z_isInstanceOf(void *self, void *type) {
  return Z_typeOf(self) == type;
}

static inline void *Z_cast(void *type, void *self) {
  assert(Z_isInstanceOf(type, ZType));
  if (Z_typeOf(self) == type) {
    return self;
  } else {
    void *t = ZObject_getSuperType(self);
    while (t != type && t != ZObject) {
      t = ZObject_getSuperType(self);
    }
    if (t == type) {
      return self;
    } else {
      return NULL;
    }
  }
}

#define Z_DECLARE_TYPE(name)            \
  extern void *name##Type;              \
  void *name##Type_initType(void);      \
  extern void *name;                    \
  void *name##_initType(void);

#define _Z_TO_STRING(x) #x

#define _Z_DEFINE_TYPE(name, superName, ...)        \
  void *name##Type = NULL;                          \
  void *name##Type_initType(void) {                 \
    if (!name##Type) {                              \
      name##Type = Z_new(ZType,                     \
                         _Z_TO_STRING(name##Type),  \
                         ZType,                     \
                         sizeof(struct name##Type), \
                         name##Type_init,           \
                         name##Type_finalize);      \
    }                                               \
    return name##Type;                              \
  }                                                 \
  void *name = NULL;                                \
  void *name##_initType(void) {                     \
    if (!name) {                                    \
      name = Z_new(name##Type,                      \
                   #name,                           \
                   superName,                       \
                   sizeof(struct name),             \
                   name##_init,                     \
                   name##_finalize,                 \
                   ##__VA_ARGS__);                  \
    }                                               \
    return name;                                    \
  }

#define Z_DEFINE_TYPE(name, superName, ...) \
  _Z_DEFINE_TYPE(name, superName, ##__VA_ARGS__)

#define ZPtr __attribute__((__cleanup__(_Z_cleanup))) void *

#ifdef  __cplusplus
}
#endif

#endif /* end of include guard: __Z_OBJECT_H__ */
