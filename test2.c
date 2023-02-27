#include <stdio.h>
#include <string.h>
#include "cwc.h"

// 定义Person类
def_class(Person, Object)
{
    int age;
    char name[16];
}
end_class(Person);

// Person类虚函数表
def_vtable(Person, Object)
{
    decl_vfunc(Person, SayHello, void);
}
end_vtable(Person);

// Person类构造函数
def_ctor(Person, int age, const char *name)
{
    super(Object);       // 调用父类构造函数
    bind_vtable(Person); // 类中含有虚函数时需要在构造函数中绑定虚函数表
    self->age = age;
    strcpy(self->name, name);
}

// 使用默认析构函数
def_default_dtor(Person, Object);

// 定义虚函数SayHello
def_vfunc(Person, SayHello, void)
{
    int age = field(self, Person, age); // 虚函数需中使用field宏获取字段
    char *name = field(self, Person, name);
    printf("Hello! I am %s, I am %d years old.\n", name, age);
}

// 定义虚函数表初始化器
def_vtable_initializer(Person)
{
    vtable_super(Object);          // 初始化虚函数表的父类部分
    vtable_bind(Person, SayHello); // 绑定虚函数
}

/*============================================================*/

// 定义Student类，继承自Person类
def_class(Student, Person)
{
    int id;
}
end_class(Student);

// 没有新的虚函数，使用默认的虚函数表定义即可
def_default_vtable(Student, Person);

// 使用默认的析构函数
def_default_dtor(Student, Person);

// Student类构造函数
def_ctor(Student, int id, int age, const char *name)
{
    super(Person, age, name);
    bind_vtable(Student);
    self->id = id;
}

// 重写SayHello函数
def_vfunc(Student, SayHello, void)
{
    int id = field(self, Student, id);
    int age = field(self, Person, age);
    char *name = field(self, Person, name);
    printf("Hello! My name is %s, I am a student, my id is %d, I am %d years old.\n", name, id, age);
}

// 定义Student类的虚函数表初始化器
def_vtable_initializer(Student)
{
    vtable_super(Person);                       // 初始化虚函数表的父类部分
    vtable_override(Student, Person, SayHello); // 重写SayHello函数
}

/*============================================================*/

int main()
{
    // main函数中先初始化虚函数表
    init_vtable(Person);
    init_vtable(Student);

    Person *p = NULL;
    new_obj(p, Person, 10, "Jerry"); // 使用new_obj创建一个对象
    call_vfunc(p, Person, SayHello); // 使用call_vfunc调用虚函数
    delete_obj(p, Person);           // 销毁对象

    Student *p2 = NULL;
    new_obj(p2, Student, 1001, 10, "Tom");
    call_vfunc(p2, Person, SayHello); // 这里第二个参数需要写Person，因为SayHello函数最先是在Person类中定义的

    Person *p3 = (Person *)p2;
    call_vfunc(p3, Person, SayHello); // 这里调用的是重写的版本
    delete_obj(p2, Student);

    return 0;
}
