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

void *TestObj_init(void *_self, bool *destructorIsCalled) {
  struct TestObj *self = ZCast(TestObj(), _self);
  self->destructorIsCalled = destructorIsCalled;
  return self;
}

void TestObj_finalize(void *_self) {
  struct TestObj *self = ZCast(TestObj(), _self);
  *self->destructorIsCalled = true;
}

void *TestObjType_init(void *self) {
  return self;
}

void TestObjType_finalize(void *_self) {
}

// must put this *after* your type's structs and methods
Z_DEFINE_TYPE(TestObj, ZObject)

int main(int argc, char *argv[]) {
  void *obj = ZNew(ZObject());
  void *type = ZObject_getType(obj);
  void *typeType = ZObject_getType(type);

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

  void *t = TestObj();
  bool destructorIsCalled = false;
  void *testObj = TestObj_init(ZNew(t), &destructorIsCalled);
  assert(ZObject_getType(testObj) == TestObj());
  assert(strcmp(ZType_getName(TestObj()), "TestObj") == 0);
  assert(ZObject_getType(TestObj()) == TestObjType());
  assert(ZType_getSuperType(TestObj()) == ZObject());
  ZDelete(testObj);
  assert(destructorIsCalled);
  ZDelete(t);

  return 0;
}
