# `cwc.h`

让C语言支持OOP，支持多态

*注：这是个整活项目，没什么实际用处，看看就行。*

## OOP支持

|       内容       | 支持  |
| :--------------: | :---: |
|       继承       |  是   |
|  构造/析构函数   |  是   |
|     成员函数     |  是   |
|      虚函数      |  是   |
| public / private |  否   |

## 使用方式

引用如下头文件

```c
#include "cwc.h"
```

### 定义类

定义类的方式如下，这里定义了一个`Person`类，该类继承自`Object`类，包含`age`和`name`两个字段

```c
def_class(Person, Object)
{
    int age;
    char name[16];
}
end_class(Person);
```

### 构造/析构函数

每个类都必须定义构造函数和析构函数，在创建和销毁对象时会自动调用，每个类只能有一个构造函数和一个析构函数，构造函数可以有参数

```c
// 构造函数
def_ctor(Person, int age, const char *name)
{
    super(Object);   // 调用父类构造函数，写在构造函数第一行
    self->age = age; // self即指向当前对象的指针
    strcpy(self->name, name);
}

// 析构函数
def_dtor(Person)
{
    // do someting...
    dtor_continue(Object); // 析构父类，写在析构函数最后，也可以使用此语句退出函数
}
```

也可以使用`def_default_ctor`和`def_default_dtor`宏创建默认的构造函数和析构函数

### 成员函数

使用`def_mfunc`宏创建成员函数，参数的前三个分别是类名、函数名和返回值类型，从第四个参数起是函数的参数，无参数的时候不用写，函数中使用`self`表示调用该函数的对象

```c
// 定义成员函数
def_mfunc(Person, SayHello, void)
{
    printf("Hello! My name is %s, I am %d years old.\n", self->name, self->age);
}
```

### 创建/销毁对象

如下代码展示了类的基本使用方法，使用`new_obj`创建并初始化对象，`delete_obj`会调用析构函数并释放内存，使用`call_mfunc`调用成员函数

```c
Person *p = NULL;
new_obj(p, Person, 10, "Jerry"); // 使用new_obj创建一个对象
call_mfunc(p, Person, SayHello); // 使用call_mfunc调用成员函数
delete_obj(p, Person);           // 销毁对象
```

### 虚函数

若要使用虚函数，则需要定义虚函数表和虚函数表初始化器，此外，在`main`函数中需要使用`init_vtable`来初始化虚函数表，定义这些有顺序要求，顺序如下

1. 定义类
2. 定义类的虚函数表
3. 定义类的构造/析构函数
4. 定义成员函数/虚函数
5. 定义虚函数表初始化器

定义虚函数表的方式如下，在虚函数表中使用`decl_vfunc`声明虚函数

```c
// Person类虚函数表
def_vtable(Person, Object)
{
    decl_vfunc(Person, SayHello, void);
}
end_vtable(Person);
```

定义虚函数的方式与成员函数类似，可以使用`def_vfunc`定义虚函数，需要注意的是，在虚函数里`self`指针的类型为`void*`，但可以使用`field`宏来获取类的字段，一下为将上面的`SayHello`函数改为虚函数

```c
// 定义虚函数SayHello
def_vfunc(Person, SayHello, void)
{
    int age = field(self, Person, age); // 虚函数需中使用field宏获取字段
    char *name = field(self, Person, name);
    printf("Hello! I am %s, I am %d years old.\n", name, age);
}
```

在定义完虚函数表和虚函数后，需要使用`def_vtable_initializer`定义虚函数表初始化器，用于初始化虚函数表，使用`vtable_bind`绑定虚函数，若需要重写函数，则需使用`vtable_override`

```c
// 定义虚函数表初始化器
def_vtable_initializer(Person)
{
    vtable_super(Object);          // 初始化虚函数表的父类部分
    vtable_bind(Person, SayHello); // 绑定虚函数
}
```

需要注意的是，当一个类中使用了虚函数，需要在构造函数中使用`bind_vtable`来为对象设置虚函数表，该语句应该放在`super`之后，以下为有使用虚函数时的构造函数定义

```c
// Person类构造函数
def_ctor(Person, int age, const char *name)
{
    super(Object);       // 调用父类构造函数
    bind_vtable(Person); // 类中含有虚函数时需要在构造函数中绑定虚函数表
    self->age = age;
    strcpy(self->name, name);
}
```

最后在main函数的最开始使用`init_vtable`来初始化虚函数表，至此就可以正常使用虚函数了

与成员函数类似，使用`call_vfunc`调用虚函数，与成员函数不同的是调用虚函数时第二个参数的类应该写的是最开始定义该函数的类而不是当前对象的类型，如下例子体现出了多态的特性（这里`Student`类是继承于`Person`类的类型，该类重写了`SayHello`方法，详见文件`test2.c`）

```c
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
```
