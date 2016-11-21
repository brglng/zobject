#ifndef __Z_OBJECT_H__
#define __Z_OBJECT_H__

#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define Z_MAGIC ((uint32_t)'ZOBJ')

void *ZObject(void);
void *ZType(void);

typedef void (*ZDestructor)(void *self);
typedef bool (*ZEqualFunc)(void *self, void *other);

struct ZObject {
  uint32_t  magic;
  void      *type;
};

struct ZType {
  struct ZObject    super;
  char              *name;
  void              *superType;
  size_t            objectSize;
  ZDestructor       finalize;
  ZEqualFunc        equal;
};

static inline bool ZIsObject(void *_self);
static inline void *ZCast(void *type, void *self);

void *ZObject_init(void *self);
void ZObject_finalize(void *self);

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

void *ZType_init(void *self, char *name, void *superType, size_t objectSize,
                 ZDestructor finalize);
void ZType_finalize(void *self);

static inline char* ZType_getName(void *_self) {
  struct ZType *self = ZCast(ZType(), _self);
  return self->name;
}

static inline void *ZType_getSuperType(void *_self) {
  struct ZType *self = ZCast(ZType(), _self);
  return self->superType;
}

static inline bool ZIsObject(void *_self) {
  assert(_self);
  struct ZObject *self = _self;
  return self->magic == Z_MAGIC;
}

static inline bool ZObject_equal(void *self, void *other) {
  struct ZType *type = ZObject_getType(self);
  return type->equal(self, other);
}

void *ZNew(void *_type);
void ZDelete(void *_self);

static inline bool ZIsInstanceOf(void *obj, void *type) {
  if (type == ZObject()) {
    return ZIsObject(obj);
  } else if (ZIsObject(obj)) {
    struct ZType *t = ((struct ZObject *)obj)->type;
    while (t != ZObject()) {
      if (t == type) {
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
      _##name##Type = ZType_init(ZNew(ZType()),                 \
                                 nameStr "Type",                \
                                 ZType(),                       \
                                 sizeof(struct name##Type),     \
                                 name##Type_finalize);          \
    }                                                           \
    return _##name##Type;                                       \
  }                                                             \
  void *_##name = NULL;                                         \
  void *name(void) {                                            \
    if (!_##name) {                                             \
      _##name = name##Type_init(ZType_init(ZNew(name##Type()),  \
                                           nameStr,             \
                                           superName(),         \
                                           sizeof(struct name), \
                                           name##_finalize));   \
    }                                                           \
    return _##name;                                             \
  }

#define Z_DEFINE_TYPE(name, superName) \
  Z_DEFINE_TYPE_WITH_NAME_STR(name, #name, superName)

#ifdef  __cplusplus
}
#endif

#endif /* end of include guard: __Z_OBJECT_H__ */
