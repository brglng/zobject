#ifndef __Z_OBJECT_H__
#define __Z_OBJECT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>

struct ZObject {
  struct ZType *type;
};

typedef void (*ZConstructor)(void *self, va_list args);
typedef void (*ZDestructor)(void *self);

struct ZType {
  struct ZObject     super;
  char              *name;
  void              *superType;
  size_t             objectSize;
  ZConstructor       objectInit;
  ZDestructor        objectFinalize;
};

extern void *ZObject(void);
extern void *ZType(void);

void    *ZNew(void *type, ...);
void     ZDelete(void *self);
void    *_ZInit(void *self, void *type, ...);
void     ZFinalize(void *self);

void     ZObject_init(void *self, va_list args);
void     ZObject_finalize(void *self);

void     ZType_init(void *self, va_list args);
void     ZType_finalize(void *self);
char    *ZType_getName(void *self);
void    *ZType_getSuperType(void *self);

void    *ZObject_getType(void *self);
void    *ZObject_getSuperType(void *self);

#define Z_DECLARE_TYPE(name) \
  void *name##Type(void); \
  void *name(void);

#define _Z_TO_STRING(x) #x

#define _Z_DEFINE_TYPE(name, superName, ...)            \
  void *_##name##Type = NULL;                           \
  void *name##Type(void) {                              \
    if (!_##name##Type) {                               \
      _##name##Type = ZNew(ZType(),                     \
                           _Z_TO_STRING(name##Type),    \
                           ZType(),                     \
                           sizeof(struct name##Type),   \
                           name##Type_init,             \
                           name##Type_finalize);        \
    }                                                   \
    return _##name##Type;                               \
  }                                                     \
  void *_##name = NULL;                                 \
  void *name(void) {                                    \
    if (!_##name) {                                     \
      _##name = ZNew(name##Type(),                      \
                    #name,                              \
                    superName(),                        \
                    sizeof(struct name),                \
                    name##_init,                        \
                    name##_finalize,                    \
                    ##__VA_ARGS__);                     \
    }                                                   \
    return _##name;                                     \
  }

#define Z_DEFINE_TYPE(name, superName, ...) \
  _Z_DEFINE_TYPE(name, superName, ##__VA_ARGS__)

#define ZVar __attribute__((__cleanup__(ZFinalize)))
#define ZInit(type, ...) \
  *(struct type *)_ZInit((char[sizeof(struct type)]){0}, type(), ##__VA_ARGS__)
#define Z_VAR(name, type, ...) ZVar struct type name = ZInit(type, ##__VA_ARGS__)
#define ZAuto ZVar __auto_type

#ifdef  __cplusplus
}
#endif

#endif /* end of include guard: __Z_OBJECT_H__ */
