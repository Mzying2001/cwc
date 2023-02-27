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

// 构造函数
def_ctor(Person, int age, const char *name)
{
    super(Object);   // 调用父类构造函数，写在构造函数第一行
    self->age = age; // self即指向当前对象的指针
    strcpy(self->name, name);
    puts("Person::ctor called!");
}

// 析构函数
def_dtor(Person)
{
    puts("Person::dtor called!");
    dtor_continue(Object); // 析构父类，写在析构函数最后，也可以使用此语句退出函数
}

// 定义成员函数
def_mfunc(Person, SayHello, void)
{
    printf("Hello! My name is %s, I am %d years old.\n", self->name, self->age);
}

int main()
{
    Person *p = NULL;
    new_obj(p, Person, 10, "Jerry"); // 使用new_obj创建一个对象
    call_mfunc(p, Person, SayHello); // 使用call_mfunc调用成员函数
    delete_obj(p, Person);           // 销毁对象
    return 0;
}
