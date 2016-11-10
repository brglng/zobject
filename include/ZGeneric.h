#ifndef __Z_GENERIC_H__
#define __Z_GENERIC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <memory.h>
#include <stddef.h>
#include "ZObject.h"

Z_DECLARE_TYPE(ZGeneric)

/*
 * This is actually not only reified generics, but also parameterized types.
 * That means, each generic arguments does not have to be a type object, but
 * can be any kind of objects. `argsTypes` stores the types of each generic
 * argument, ensuring runtime type safety.
 */
struct ZGeneric {
  struct ZType  super;
  size_t        numArgs;
  void          **argsTypes;
  void          **args;
};

struct ZGenericType {
  struct ZType  super;
};

void ZGeneric_init(void *_self, va_list args);
void ZGeneric_finalize(void *_self);
void ZGenericType_init(void *_self, va_list args);
void ZGenericType_finalize(void *_self);

#define Z_GENERIC_ARG(obj, i) \
  (((struct ZGeneric *)Z_cast(ZGeneric(), Z_typeOf(obj)))->args[i])
#define Z_GENERIC_ARG_TYPE(obj, i) \
  (((struct ZGeneric *)Z_cast(ZGeneric(), Z_typeOf(obj)))->argsTypes[i])

#define Z_GENERIC_MAKE_ARGS(...) ((void *[]){__VA_ARGS__})

#define Z_DECLARE_GENERIC(name, superName, numArgs)     \
  void ** name##_genericArgsTypes(void);        \
  void *name##Type(void *gArgs[numArgs]);               \
  void *name(void *gArgs[numArgs]);

#define Z_DEFINE_GENERIC_WITH_NAME_STR(name, nameStr, superName, numArgs, ...)  \
  void *_name##_genericArgsTypes[numArgs] = {NULL};                             \
  void ** name##_genericArgsTypes(void) {                                       \
    if (_name##_genericArgsTypes[0] == NULL) {                                  \
      memcpy(_name##_genericArgsTypes,                                          \
             (void *[numArgs]){__VA_ARGS__},                                    \
             (numArgs) * sizeof(void *));                                       \
    }                                                                           \
    return _name##_genericArgsTypes;                                            \
  }                                                                             \
  void *name##Type(void *gArgs[numArgs]) {                                      \
    return Z_new(ZGeneric(),                                                    \
                 nameStr "Type",                                                \
                 ZGeneric(),                                                    \
                 sizeof(struct name##Type),                                     \
                 name##Type_init,                                               \
                 name##Type_finalize,                                           \
                 numArgs,                                                       \
                 name##_genericArgsTypes(),                                     \
                 gArgs);                                                        \
  }                                                                             \
  void *name(void *gArgs[numArgs]) {                                            \
    return Z_new(name##Type(gArgs),                                             \
                 nameStr,                                                       \
                 superName(),                                                   \
                 sizeof(struct name),                                           \
                 name##_init,                                                   \
                 name##_finalize,                                               \
                 numArgs,                                                       \
                 name##_genericArgsTypes(),                                     \
                 gArgs);                                                        \
  }

#define Z_DEFINE_GENERIC(name, superName, numArgs, ...) \
  Z_DEFINE_GENERIC_WITH_NAME_STR(name, #name, superName, numArgs, __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __Z_GENERIC_H__ */
