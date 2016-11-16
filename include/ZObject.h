#ifndef __Z_OBJECT_H__
#define __Z_OBJECT_H__

#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#define Z_MAGIC ((uint32_t)'ZOBJ')

void *ZObject(void);
void *ZType(void);

typedef void (*ZConstructor)(void *self, va_list args);
typedef void (*ZDestructor)(void *self);

struct ZObject {
  uint32_t  magic;
  void      *type;
};

struct ZType {
  struct ZObject    super;
  char              *name;
  void              *superType;
  size_t            objectSize;
  void              (*init)(void *self, va_list args);
  void              (*finalize)(void *self);
  bool              (*isEqual)(void *type, void *other);
};

static inline bool ZIsObject(void *_self);
static inline void *ZCast(void *type, void *self);

void ZObject_init(void *self, va_list args);
void ZObject_finalize(void *self);
bool ZObject_isEqual(void *type, void *other);

static inline void *ZObject_getType(void *_self) {
  assert(ZIsObject(_self));
  struct ZObject *self = _self;
  return self->type;
}

static inline void *ZObject_getSuperType(void *_self) {
  assert(ZIsObject(_self));
  struct ZObject *self = _self;
  return ((struct ZType *)self->type)->superType;
}

void ZType_init(void *self, va_list args);
void ZType_finalize(void *self);

static inline char* ZType_getName(void *_self) {
  struct ZType *self = ZCast(ZType(), _self);
  return self->name;
}

static inline void *ZType_getSuperType(void *_self) {
  struct ZType *self = ZCast(ZType(), _self);
  return self->superType;
}

void *ZNew(void *type, ...);
void ZDelete(void *self);

static inline bool ZIsObject(void *_self) {
  assert(_self);
  struct ZObject *self = _self;
  return self->magic == Z_MAGIC;
}

static inline void *ZTypeOf(void *self) {
  return ZObject_getType(self);
}

static inline bool ZIsEqual(void *self, void *other) {
  struct ZType *type = ZTypeOf(self);
  return type->isEqual(self, other);
}

static inline bool ZIsInstanceOf(void *obj, void *type) {
  if (type == ZObject()) {
    return ZIsObject(obj);
  } else if (ZIsObject(obj)) {
    struct ZType *t = ((struct ZObject *)obj)->type;
    while (t != ZObject()) {
      if (ZIsEqual(t, type)) {
        return true;
      }
      t = t->superType;
    }
    return false;
  } else {
    return false;
  }
}

static inline void *ZCast(void *type, void *obj) {
  assert(ZIsInstanceOf(obj, type));
  return obj;
}

#define Z_DECLARE_TYPE(name)    \
  void *name##Type();           \
  void *name();

#define Z_DEFINE_TYPE_WITH_NAME_STR(name, nameStr, superName)   \
  void *_##name##Type = NULL;                                   \
  void *name##Type(void) {                                      \
    if (!_##name##Type) {                                       \
      _##name##Type = ZNew(ZType(),                             \
                           nameStr "Type",                      \
                           ZType(),                             \
                           sizeof(struct name##Type),           \
                           name##Type_init,                     \
                           name##Type_finalize);                \
    }                                                           \
    return _##name##Type;                                       \
  }                                                             \
  void *_##name = NULL;                                         \
  void *name(void) {                                            \
    if (!_##name) {                                             \
      _##name = ZNew(name##Type(),                              \
                     nameStr,                                   \
                     superName(),                               \
                     sizeof(struct name),                       \
                     name##_init,                               \
                     name##_finalize);                          \
    }                                                           \
    return _##name;                                             \
  }

#define Z_DEFINE_TYPE(name, superName) \
  Z_DEFINE_TYPE_WITH_NAME_STR(name, #name, superName)

#define Z_VA_ARG_SIZE_T(args) _Generic((+(sizeof(0))),  \
  int:          (size_t)va_arg((args), unsigned int),   \
  unsigned int: (size_t)va_arg((args), unsigned int),   \
  default:              va_arg((args), size_t))

#ifdef  __cplusplus
}
#endif

#endif /* end of include guard: __Z_OBJECT_H__ */
