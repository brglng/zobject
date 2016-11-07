#include <stddef.h>
#include <stdint.h>
#include "ZGeneric.h"

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
  self->data = Z_cast(ZGeneric_getArg(Z_typeOf(self), 0), va_arg(args, void *));
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

void *TestGenericObjectType(size_t numArgs, void **args)
{
  return Z_new(ZGeneric(),
               "TestGenericObjectType",
               ZGeneric(),
               sizeof(struct TestGenericObjectType),
               TestGenericObjectType_init,
               TestGenericObjectType_finalize,
               numArgs,
               args);
}

void *TestGenericObject(size_t numArgs, void **args)
{
  return Z_new(TestGenericObjectType(numArgs, args),
               "TestGenericObject",
               ZObject(),
               sizeof(struct TestGenericObject),
               TestGenericObject_init,
               TestGenericObject_finalize,
               numArgs,
               args);
}

int main(void)
{
  ZRaii void *obj = Z_new(TestGenericObject(1, (void *[]){ ZType() }), ZObject());
  return 0;
}
