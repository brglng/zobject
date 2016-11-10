#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include "ZGeneric.h"

Z_DECLARE_GENERIC(TestGenericObject, ZObject, 1)

struct TestGenericObject {
  struct ZObject    super;
  void              *data;
};

struct TestGenericObjectType {
  struct ZGeneric   super;
};

void TestGenericObject_init(void *_self, va_list args)
{
  struct TestGenericObject *self = _self;
  self->data = Z_cast(Z_GENERIC_ARG(self, 0), va_arg(args, void *));
}

void TestGenericObject_finalize(void *_self)
{
}

void TestGenericObjectType_init(void *_self, va_list args)
{
  ZGeneric_init(_self, args);
}

void TestGenericObjectType_finalize(void *_self)
{}

// Name, SuperTypeName, numArgs, TypeOfArg[0], TypeOfArg[1], ...
Z_DEFINE_GENERIC(TestGenericObject, ZObject, 1, ZType())

int main(void)
{
  /* ZRaii void *x = Z_new(ZObject()); */
  // ZRaii void *obj = Z_new(TestGenericObject(1, ZType(), ZType()), x);

  // refer to a generic type using: Type(arg0, arg1, ...)
  ZRaii void *obj = Z_new(TestGenericObject(Z_GENERIC_MAKE_ARGS(ZType())), ZObject());
  return 0;
}
