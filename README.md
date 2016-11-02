# ZObject
This is my experiment on Object-Oriented Programming with C. It is a library
which can be called from any C/C++ program. It currently only supports GCC and
Clang (because the implementation heavily depends on [GCC extensions](https://gcc.gnu.org/onlinedocs/gcc-6.1.0/gcc/Common-Variable-Attributes.html)).
This library is in very early development and is not ready for production. Use
at your own risk!

## Why Bother Creating Yet Another C Object-Oriented Programming Library?
Just as I said before, this is my experiment. I create this library not only for
fun, but also for learning the internals of Object-Oriented Programming.

## Usage and Features

### Defining a Type
SomeObject.h:
```c
#include "ZObject.h"

Z_DECLARE_TYPE(SomeObject)

struct SomeObject {
  struct ZObject    super;
  int               someMember;                  
};

struct SomeObjectType {
  struct ZType  super;
  void          (*setSomeMember)(void *self, int);
  int           (*getSomeMember)(void *self);
};

void SomeObject_init(void *self, va_list args);
void SomeObject_finalize(void *self);
void SomeObjectClass_init(void *self, va_list args);
void SomeObjectClass_finalize(void *self);
void SomeObject_setSomeMember(void *self, int someMember);
int  SomeObject_getSomeMember(void *self);
```

SomeObject.c:
```c
#include "ZObject.h"
#include "SomeObject.h"

Z_DEFINE_TYPE(SomeObject, ZObject)

void SomeObject_init(void *_self, va_list args)
{
  SomeObject *self = _self;
  self->someMember = va_arg(args, int);
}

void SomeObject_finalize(void *_self)
{}

void SomeObjectType_init(void *_self, va_list args)
{
  ZType_init(_self, args);

  SomeObjectType *self = _self;

  self->setSomeMember = SomeObject_setSomeMember;
  self->getSomeMember = SomeObject_getSomeMember;
}

void SomeObjectType_finalize(void *_self)
{}

void SomeObject_setSomeMember(void *_self, int someMember)
{
  SomeObject *self = _self;
  self->someMember = someMember;
}

int SomeObject_getSomeMember(void *_self)
{
  SomeObject *self = _self;
  return self->someMember;
}
```

### Using a Type
```c
#include "SomeObject.h"

int main(void)
{
  // stack object, RAII is supported
  ZAutoVar obj1 = ZInit(SomeObject, 123);
  // or use the following style
  // ZVar ZAuto obj1 = ZInit(SomeObject, 123);
  // or use
  // ZVar struct SomeObject obj1 = ZInit(SomeObject, 123);
  // or use
  // Z_VAR(obj1, SomeObject, 123);

  // heap object, which must be deleted manually
  void *obj2 = ZNew(SomeObject(), 123);

  // call methods
  SomeObject_setSomeMember(obj2, 321);

  printf("%d\n", SomeObject_getSomeMember(obj2));

  ZDelete(obj2);

  return 0;
}
```

## References
- [Object-Oriented Programming with ANSI C](https://www.cs.rit.edu/~ats/books/ooc.pdf)
- [GObject](https://developer.gnome.org/gobject/stable/)
- [Inside the C++ Object Model](https://www.amazon.com/Inside-Object-Model-Stanley-Lippman/dp/0201834545)
- [Cello](http://libcello.org/)
- [Viola](https://github.com/eatonphil/Viola)
- [Implementing smart pointers for the C programming language](https://snai.pe/c/c-smart-pointers/)
- [libcsptr](https://github.com/Snaipe/libcsptr)
