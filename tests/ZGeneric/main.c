#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include "ZGeneric.h"

Z_DECLARE_GENERIC(TestGeneric, ZObject, 1)

// these two macros are for convenience, usually necessary
#define TestGenericType(...)    (_TestGenericType(Z_GENERIC_ARGS(__VA_ARGS__)))
#define TestGeneric(...)        (_TestGeneric(Z_GENERIC_ARGS(__VA_ARGS__)))

struct TestGeneric {
  struct ZObject    super;
  void              *data;
};

struct TestGenericType {
  struct ZGeneric   super;
};

void TestGeneric_init(void *_self, va_list args)
{
  void *T = ZGeneric_getArg(ZTypeOf(_self), 0);
  struct TestGeneric *self = ZCast(TestGeneric(T), _self);

  self->data = ZCast(T, va_arg(args, void *));
}

void TestGeneric_finalize(void *_self)
{
}

void TestGenericType_init(void *_self, va_list args)
{
  ZGeneric_init(_self, args);
}

void TestGenericType_finalize(void *_self)
{
  ZGeneric_finalize(_self);
}

// class TestGeneric<Type T> extends Object
Z_DEFINE_GENERIC(TestGeneric, ZObject, 1, ZType())

int main(void)
{
  // new TestGeneric<Type>(Object)
  void *obj = ZNew(TestGeneric(ZType()), ZObject());
  ZDelete(obj);
  return 0;
}
