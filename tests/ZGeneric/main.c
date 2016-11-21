#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include "ZGeneric.h"

Z_DECLARE_GENERIC(TestGeneric, ZObject)

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

void *TestGeneric_init(void *_self, void *data) {
  struct TestGeneric *self = _self;
  void *T = ZGeneric_getArg(ZObject_getType(_self), 0);

  self->data = ZCast(T, data);

  return self;
}

void *TestGeneric_getData(void *_self) {
  struct TestGeneric *self = _self;

  return self->data;
}

void TestGeneric_finalize(void *self) {
}

void *TestGenericType_init(void *self) {
  // ZGeneric_init() is not necessary as it is called in Z_DEFINE_GENERIC macro
  return self;
}

void TestGenericType_finalize(void *self) {
  // super type's destructor must be called if it is not ZObject or ZType
  ZGeneric_finalize(self);
}

// class TestGeneric<Type T> extends Object
Z_DEFINE_GENERIC(TestGeneric, ZObject, ZType())

int main(void)
{
  // new TestGeneric<Type>(Object)
  void *t = TestGeneric(ZType());
  void *obj = TestGeneric_init(ZNew(t), ZObject());
  assert(ZGeneric_getNumArgs(t) == 1);
  assert(ZGeneric_getArgType(t, 0) == ZType());
  assert(ZGeneric_getArg(t, 0) == ZType());
  assert(TestGeneric_getData(obj) == ZObject());
  ZDelete(obj);
  ZDelete(t);
  return 0;
}
