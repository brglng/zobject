#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include "ZGeneric.h"

void *ZGeneric_init(void *_self, char *name, void *superType, size_t objectSize,
                    ZDestructor finalize, size_t numArgs, void **argsTypes,
                    void **args) {
  struct ZGeneric *self = ZCast(ZGeneric(), _self);

  ZType_init(self, name, superType, objectSize, finalize);

  assert(numArgs > 0);
  assert(argsTypes);
  for (size_t i = 0; i < self->numArgs; ++i) {
    assert(ZIsInstanceOf(argsTypes[i], ZType()));
  }
  assert(args);
  for (size_t i = 0; i < self->numArgs; ++i) {
    assert(ZIsInstanceOf(args[i], self->argsTypes[i]));
  }

  self->numArgs = numArgs;
  self->argsTypes = calloc(numArgs, sizeof(void *));
  assert(self->argsTypes);
  self->args = calloc(self->numArgs, sizeof(void *));
  assert(self->args);
  memcpy(self->argsTypes, argsTypes, numArgs * sizeof(void *));
  memcpy(self->args, args, numArgs * sizeof(void *));

  return self;
}

void ZGeneric_finalize(void *_self) {
  struct ZGeneric *self = ZCast(ZGeneric(), _self);
  free(self->argsTypes);
  free(self->args);
  ZType_finalize(self);
}

void *ZGenericType_init(void *self) {
  return self;
}

void ZGenericType_finalize(void *self) {
  ZType_finalize(self);
}

Z_DEFINE_TYPE_WITH_NAME_STR(ZGeneric, "Generic", ZType)
