#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include "ZGeneric.h"

Z_DEFINE_TYPE_WITH_NAME_STR(ZGeneric, "Generic", ZType)

void ZGeneric_init(void *_self, va_list args)
{
  struct ZGeneric *self = ZCast(ZGeneric(), _self);
  ZType_init(self, args);
  self->numArgs = Z_VA_ARG_SIZE_T(args);
  self->argsTypes = calloc(self->numArgs, sizeof(void *));
  assert(self->argsTypes);
  for (size_t i = 0; i < self->numArgs; ++i) {
    self->argsTypes[i] = ZCast(ZType(), va_arg(args, void *));
  }
  void **gArgs = va_arg(args, void **);
  for (size_t i = 0; i < self->numArgs; ++i) {
    ZIsInstanceOf(gArgs[i], self->argsTypes[i]);
  }
  self->args = calloc(self->numArgs, sizeof(void *));
  assert(self->args);
  memcpy(self->args, gArgs, self->numArgs * sizeof(struct ZObject *));
}

void ZGeneric_finalize(void *_self)
{
  struct ZGeneric *self = ZCast(ZGeneric(), _self);
  free(self->argsTypes);
  free(self->args);
  ZType_finalize(self);
}

void ZGenericType_init(void *self, va_list args)
{
  ZType_init(self, args);
}

void ZGenericType_finalize(void *self)
{
  ZType_finalize(self);
}
