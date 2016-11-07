#ifndef __Z_GENERIC_H__
#define __Z_GENERIC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include "ZObject.h"

Z_DECLARE_TYPE(ZGeneric)

struct ZGeneric {
  struct ZType  super;
  void          **args;
  size_t        numArgs;
};

struct ZGenericType {
  struct ZType  super;
};

void ZGeneric_init(void *_self, va_list args);
void ZGeneric_finalize(void *_self);
void ZGenericType_init(void *_self, va_list args);
void ZGenericType_finalize(void *_self);

static inline void *ZGeneric_getTypeArg(void *_self, size_t index) {
  struct ZGeneric *self = Z_cast(ZGeneric(), _self);
  assert(index < self->numArgs);
  return self->args[index];
}

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: __Z_GENERIC_H__ */
