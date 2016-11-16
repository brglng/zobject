#ifndef __Z_GENERIC_H__
#define __Z_GENERIC_H__

#include <memory.h>
#include <stddef.h>
#include "ZObject.h"

#ifdef __cplusplus
extern "C" {
#endif

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

void ZGeneric_init(void *self, va_list args);
void ZGeneric_finalize(void *self);

void ZGenericType_init(void *self, va_list args);
void ZGenericType_finalize(void *self);

#define Z_GENERIC_ARG(obj, i) \
  (((struct ZGeneric *)ZCast(ZGeneric(), ZTypeOf(obj)))->args[i])
#define Z_GENERIC_ARG_TYPE(obj, i) \
  (((struct ZGeneric *)ZCast(ZGeneric(), ZTypeOf(obj)))->argsTypes[i])

#define Z_GENERIC_ARGS(...) ((void *[]){__VA_ARGS__})

#define Z_DECLARE_GENERIC(name, superName, numArgs)   \
  void *_##name##Type(void *gArgs[numArgs]);        \
  void *_##name(void *gArgs[numArgs]); 

#define Z_DEFINE_GENERIC_WITH_NAME_STR(name, nameStr, superName, numArgs, ...)  \
  void *_##name##Type(void *gArgs[numArgs]) {                                   \
    return ZNew(ZGeneric(),                                                     \
                nameStr "Type",                                                 \
                ZGeneric(),                                                     \
                sizeof(struct name##Type),                                      \
                name##Type_init,                                                \
                name##Type_finalize,                                            \
                (size_t)numArgs,                                                \
                __VA_ARGS__,                                                    \
                gArgs);                                                         \
  }                                                                             \
  void *_##name(void *gArgs[numArgs]) {                                         \
    return ZNew(_##name##Type(gArgs),                                           \
                nameStr,                                                        \
                superName(),                                                    \
                sizeof(struct name),                                            \
                name##_init,                                                    \
                name##_finalize,                                                \
                (size_t)numArgs,                                                \
                __VA_ARGS__,                                                    \
                gArgs);                                                         \
  }

#define Z_DEFINE_GENERIC(name, superName, numArgs, ...) \
  Z_DEFINE_GENERIC_WITH_NAME_STR(name, #name, superName, numArgs, __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __Z_GENERIC_H__ */
