#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include "ZObject.h"

// this is only necessary if you put this in a header
Z_DECLARE_TYPE(TestObj)

struct TestObj {
  struct ZObject    super;
  bool              *destructorIsCalled;
};

struct TestObjType {
  struct ZType      super;
};

void TestObj_init(void *_self, va_list arg) {
  struct TestObj *self = ZCast(TestObj(), _self);
  self->destructorIsCalled = va_arg(arg, bool *);
}

void TestObj_finalize(void *_self) {
  struct TestObj *self = ZCast(TestObj(), _self);
  *self->destructorIsCalled = true;
}

void TestObjType_init(void *_self, va_list arg) {
  ZType_init(_self, arg);
}

void TestObjType_finalize(void *_self) {
  ZType_finalize(_self);
}

// must put this *after* your type's structs and methods
Z_DEFINE_TYPE(TestObj, ZObject)

int main(int argc, char *argv[])
{
  void *obj = ZNew(ZObject());
  void *type = ZTypeOf(obj);
  void *typeType = ZTypeOf(type);

  // Object
  assert(ZObject_getType(obj) == ZObject());
  assert(ZObject_getType(ZCast(ZObject(), type)) == ZType());
  assert(ZObject_getType(ZCast(ZObject(), typeType)) == ZType());
  assert(ZObject_getSuperType(obj) == ZObject());
  assert(ZObject_getSuperType(ZCast(ZObject(), type)) == ZObject());
  assert(ZObject_getSuperType(ZCast(ZObject(), typeType)) == ZObject());

  ZDelete(obj);

  // Type
  assert(strcmp(ZType_getName(type), "Object") == 0);
  assert(strcmp(ZType_getName(typeType), "Type") == 0);
  assert(ZType_getSuperType(type) == ZObject());
  assert(ZType_getSuperType(typeType) == ZObject());

  bool destructorIsCalled = false;
  void *testObj = ZNew(TestObj(), &destructorIsCalled);
  assert(ZTypeOf(testObj) == TestObj());
  assert(strcmp(ZType_getName(TestObj()), "TestObj") == 0);
  assert(ZTypeOf(TestObj()) == TestObjType());
  assert(ZType_getSuperType(TestObj()) == ZObject());
  ZDelete(testObj);

  assert(destructorIsCalled);

  return 0;
}
