#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include "ZGeneric.h"

Z_DEFINE_TYPE_WITH_NAME_STR(ZGeneric, "Generic", ZType)

void ZGeneric_init(void *_self, va_list args)
{
  struct ZGeneric *self = Z_cast(ZGeneric(), _self);
  ZType_init(self, args);
  self->numArgs = Z_VA_ARG_SIZE_T(args);
  void **gArgsTypes = va_arg(args, void **);
  void **gArgs = va_arg(args, void **);
  for (size_t i = 0; i < self->numArgs; ++i) {
    assert(Z_isInstanceOf(gArgsTypes[i], ZType()));
    assert(Z_isInstanceOf(gArgs[i], gArgsTypes[i]));
  }
  self->argsTypes = calloc(self->numArgs, sizeof(void *));
  assert(self->argsTypes);
  self->args = calloc(self->numArgs, sizeof(void *));
  assert(self->args);
  memcpy(self->argsTypes, gArgsTypes, self->numArgs * sizeof(void *));
  memcpy(self->args, gArgs, self->numArgs * sizeof(void *));
}

void ZGeneric_finalize(void *_self)
{
  struct ZGeneric *self = Z_cast(ZGeneric(), _self);
  free(self->argsTypes);
  free(self->args);
}

void ZGenericType_init(void *_self, va_list args)
{
  struct ZGenericType *self = Z_cast(ZGenericType(), _self);
  ZType_init(self, args);
}

void ZGenericType_finalize(void *_self)
{}
