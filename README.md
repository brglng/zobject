# ZObject
This is my experiment on Object-Oriented Programming with C. It is a library
which can be called from any C/C++ program. It currently only supports GCC and
Clang (because the RAII implementation depends on [GCC's extension of `__attribute__(cleanup)`](https://gcc.gnu.org/onlinedocs/gcc-6.1.0/gcc/Common-Variable-Attributes.html)).
This library is in very early development and is not ready for production. Use
at your own risk!

## Why Creating Yet Another Object-Oriented Programming Library for C?
Just as I said before, this is my experiment. I create this library not only for
fun, but also for learning the internals of Object-Oriented Programming.

## Usage and Features

### Defining a class
some_class.h:
```c
#include "z/object.h"

// This is only necessary if you want your class to be accessible from other .c
// files. Put it in the header. The parameter is the name of the type, which
// must be the same as your struct definition.
Z_DECLARE_CLASS(SomeObject)

struct SomeObject {
  // Inherit by putting a super class instance at the first slot. All classes
  // must inherit ZObject if not another class.
  struct ZObject super;

  // Put your instance members here.
  int some_member;                  
};

// Each class has a corresponding metaclass, which must be named *Class.
struct SomeObjectClass {
  // Must inherit a metaclass. ZClass is the default one.
  struct ZClass super;

  // Methods are declared as function pointers in the metaclass struct.
  void (*set_some_member)(void *self, int);
  int (*get_some_member)(void *self);
};

// SomeObject constructor. Must be named *_init and identical to this signature.
void SomeObject_init(void *self, va_list args);

// SomeObject destructor. Must be named *_finalize and identical to this
// signature.
void SomeObject_finalize(void *self);

// SomeObjectClass constructor. Must be named *_init and identical to this
// signatue.
void SomeObjectClass_init(void *self, va_list args);

// SomeObject destructor. Must be named *_finalize and identical to this
// signature.
void SomeObjectClass_finalize(void *self);

// Using the class name to prefix its methods is recommended.
void SomeObject_set_some_member(void *self, int some_member);
int  SomeObject_get_some_member(void *self);
```

some_class.c:
```c
#include "z/object.h"
#include "some_class.h"

// Put this in each class's implementation. The first parameters is the name of
// the class, and the second one is the class which your class inherits.
Z_DEFINE_CLASS(SomeObject, ZObject)

void SomeObject_init(void *_self, va_list args)
{
  SomeObject *self = _self;
  self->some_member = va_arg(args, int);
}

void SomeObject_finalize(void *_self)
{}

void SomeObjectClass_init(void *_self, va_list args)
{
  // Almost certainly you should call the super metaclass's constructor.
  ZClass_init(_self, args);

  // Although declared with `args` parameter, metaclass's constructor should not
  // actually take any parameters. Even if you use `va_arg` to get parameters,
  // they are ignored by `z_new`.

  SomeObjectClass *self = _self;

  // You must set up your overrided methods here.
  self->set_some_member = SomeObject_set_some_member;
  self->get_some_member = SomeObject_get_some_member;
}

void SomeObjectClass_finalize(void *_self)
{}

void SomeObject_set_some_member(void *_self, int some_member)
{
  SomeObject *self = _self;
  self->some_member = some_member;
}

int SomeObject_get_some_member(void *_self)
{
  SomeObject *self = _self;
  return self->some_member;
}
```

### Using a class
```c
#inlucde "some_object.h"

int main(void)
{
  // Use 'z_new' just like 'new' in C++. The first parameter is a function with
  // the name of the class, which is defined automatically in the Z_DEFINE_CLASS
  // macro. 'z_new' calls the constructor.
  void *obj = z_new(SomeObject(), 123);

  // Ordinary objects must be deleted manually. 'z_delete' calls the destructor.
  z_delete(obj);

  // Objects declared as ZRef are reference counted objects with RAII, whose
  // internal reference count is automatically decremented at the end of a
  // function. When the count become zero, it is automatically deleted.
  ZRef obj1 = z_new(SomeObject(), 123);

  // call methods
  SomeObject_set_some_member(obj1, 321);

  // should prints '321'
  printf("%d\n", SomeObject_get_some_member(obj1));

  // z_delete(obj1) is not necessary

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
