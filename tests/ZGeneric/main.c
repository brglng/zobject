#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include "ZGeneric.h"

Z_DECLARE_GENERIC(TestGeneric, ZObject, 1)

struct TestGeneric {
  struct ZObject    super;
  void              *data;
};

struct TestGenericType {
  struct ZGeneric   super;
};

void TestGeneric_init(void *_self, va_list args)
{
  struct TestGeneric *self = _self;
  self->data = Z_cast(Z_GENERIC_ARG(self, 0), va_arg(args, void *));
}

void TestGeneric_finalize(void *_self)
{
}

void TestGenericType_init(void *_self, va_list args)
{
  ZGeneric_init(_self, args);
}

void TestGenericType_finalize(void *_self)
{}

// Name, SuperTypeName, numArgs, TypeOfArg[0], TypeOfArg[1], ...
Z_DEFINE_GENERIC(TestGeneric, ZObject, 1, ZType())

int main(void)
{
  // refer to a generic type using: Type(Z_GENERIC_MAKE_ARGS(arg0, arg1, ...))
  ZRaii void *obj = Z_new(TestGeneric(Z_GENERIC_MAKE_ARGS(ZType())), ZObject());
  return 0;
}
