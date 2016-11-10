#ifndef __Z_OBJECT_H__
#define __Z_OBJECT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#define Z_MAGIC 'ZOBJ'

void *ZObject(void);
void *ZType(void);

struct ZObject {
  uint32_t      magic;
  struct ZType  *type;
};

struct ZType {
  struct ZObject    super;
  char              *name;
  void              *superType;
  size_t            objectSize;
  void              (*init)(void *self, va_list args);
  void              (*finalize)(void *self);
  bool              (*isInstanceOf)(void *self, void *type);
};

static inline bool Z_isObject(void *_self);
static inline void *Z_cast(void *type, void *self);

void ZObject_init(void *self, va_list args);
void ZObject_finalize(void *self);
bool ZObject_isInstanceOf(void *self, void *type);

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

void ZType_init(void *self, va_list args);
void ZType_finalize(void *self);

static inline char* ZType_getName(void *_self) {
  struct ZType *self = Z_cast(ZType(), _self);
  return self->name;
}

static inline void *ZType_getSuperType(void *_self) {
  struct ZType *self = Z_cast(ZType(), _self);
  return self->superType;
}

void *Z_vnew(void *type, va_list args);
void *Z_new(void *type, ...);
void Z_delete(void *self);

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
  return ((struct ZType *)Z_typeOf(self))->isInstanceOf(self, type);
}

static inline void *Z_cast(void *type, void *self) {
  assert(Z_isInstanceOf(self, type));
  return self;
}

#define Z_DECLARE_TYPE(name)    \
  void *name##Type(void);       \
  void *name(void);

#define Z_DEFINE_TYPE_WITH_NAME_STR(name, nameStr, superName)   \
  void *_##name##Type = NULL;                                   \
  void *name##Type(void) {                                      \
    if (!_##name##Type) {                                       \
      _##name##Type = Z_new(ZType(),                            \
                            nameStr "Type",                     \
                            ZType(),                            \
                            sizeof(struct name##Type),          \
                            name##Type_init,                    \
                            name##Type_finalize);               \
    }                                                           \
    return _##name##Type;                                       \
  }                                                             \
  void *_##name = NULL;                                         \
  void *name(void) {                                            \
    if (!_##name) {                                             \
      _##name = Z_new(name##Type(),                             \
                      nameStr,                                  \
                      superName(),                              \
                      sizeof(struct name),                      \
                      name##_init,                              \
                      name##_finalize);                         \
    }                                                           \
    return _##name;                                             \
  }

#define Z_DEFINE_TYPE(name, superName) \
  Z_DEFINE_TYPE_WITH_NAME_STR(name, #name, superName)

#define ZRaii __attribute__((__cleanup__(_Z_cleanup)))

// http://stackoverflow.com/questions/12862966/integer-promotion-for-long-and-size-t-when-sent-through-ellipsis
#define Z_VA_ARG_SIZE_T(args) _Generic((+(sizeof(0))),  \
  int:          (size_t) va_arg((args), unsigned int),  \
  unsigned int: (size_t) va_arg((args), unsigned int),  \
  default:               va_arg((args), size_t))

#ifdef  __cplusplus
}
#endif

#endif /* end of include guard: __Z_OBJECT_H__ */
