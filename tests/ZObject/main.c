#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include "ZObject.h"

// this is only necessary if you put this in a header
Z_DECLARE_TYPE(RaiiTest)

struct RaiiTest {
  struct ZObject    super;
  bool*             destructorIsCalled;
};

struct RaiiTestType {
  struct ZType     super;
};

void RaiiTest_init(void *_self, va_list args)
{
  struct RaiiTest *self = _self;
  self->destructorIsCalled = va_arg(args, bool*);
}

void RaiiTest_finalize(void *_self)
{
  struct RaiiTest *self = _self;
  *self->destructorIsCalled = true;
}

void RaiiTestType_init(void *_self, va_list args)
{
  // almost certainly you should call the parent meta type's constructor
  ZType_init(_self, args);
}

void RaiiTestType_finalize(void *_self)
{
}

// must put this *after* your type's structs and methods
Z_DEFINE_TYPE(RaiiTest, ZObject)

void testRaii(bool *destructorIsCalled)
{
  *destructorIsCalled = false;

  /* either style of definition is ok */
  /* ZVar RaiiTest raiiObj = ZInit(RaiiTest, destructorIsCalled); */
  Z_VAR(raiiObj, RaiiTest, destructorIsCalled);
}

int main(int argc, char *argv[])
{
  void *obj = ZNew(ZObject());
  void *type = ZObject_getType(obj);
  void *typeType = ZObject_getType(type);

  // Object
  assert(ZObject_getType(obj) == ZObject());
  assert(ZObject_getType(type) == ZType());
  assert(ZObject_getType(typeType) == ZType());
  assert(ZObject_getSuperType(obj) == ZObject());
  assert(ZObject_getSuperType(type) == ZObject());
  assert(ZObject_getSuperType(typeType) == ZObject());

  // Type
  assert(strcmp(ZType_getName(type), "Object") == 0);
  assert(strcmp(ZType_getName(typeType), "Type") == 0);
  assert(ZType_getSuperType(type) == ZObject());
  assert(ZType_getSuperType(typeType) == ZObject());

  // RAII
  bool destructorIsCalled = false;
  testRaii(&destructorIsCalled);
  assert(destructorIsCalled);

  return 0;
}
