#ifndef _CWC_H_
#define _CWC_H_

#include <stdlib.h>

/*============================================================*/

// 类结构体名
#define _class_struct_name(class_name) \
    _CLASS_STRUCT_##class_name

// 构造函数名称
#define _ctor_func_name(class_name) \
    _CTOR_FUNC_##class_name

// 析构函数名称
#define _dtor_func_name(class_name) \
    _DTOR_FUNC_##class_name

// 成员函数名称
#define _member_func_name(class_name, func_name) \
    _MEMBER_FUNC_##class_name##_##func_name

// 虚函数名称
#define _virtual_func_name(class_name, func_name) \
    _VIRTUAL_FUNC_##class_name##_##func_name

// 虚函数表对象名
#define _virtual_table_name(class_name) \
    _VIRTUAL_TABLE_##class_name

// 虚函数表结构体名称
#define _virtual_table_struct_name(class_name) \
    _VIRTUAL_TABLE_STRUCT_##class_name

// 虚函数表初始化函数名称
#define _virtual_table_initializer_name(class_name) \
    _VIRTUAL_TABLE_INITIALIZER_##class_name

/*============================================================*/

// 类的指针类型
#define ptr_of(class_name) \
    struct _class_struct_name(class_name) *

// 强制转换指针类型
#define ptr_cast(target_type, ptr) \
    ((target_type)(void *)(ptr))

// 定义类开始
#define def_class(class_name, base_class)         \
    typedef struct _class_struct_name(class_name) \
    {                                             \
        base_class _base;                         \
        struct

// 定义类结束
#define end_class(class_name) \
    ;                         \
    }                         \
    class_name

// 定义成员函数
#define def_mfunc(class_name, func_name, ret_type, ...) \
    ret_type _member_func_name(class_name, func_name)(ptr_of(class_name) self, ##__VA_ARGS__)

// 调用成员函数
#define call_mfunc(ptr, class_name, func_name, ...) \
    (_member_func_name(class_name, func_name)(ptr, ##__VA_ARGS__))

// 定义构造函数
#define def_ctor(class_name, ...) \
    void _ctor_func_name(class_name)(ptr_of(class_name) self, ##__VA_ARGS__)

// 调用父类构造函数
#define super(base_class, ...) \
    _ctor_func_name(base_class)(ptr_cast(ptr_of(base_class), self), ##__VA_ARGS__)

// 定义析构函数
#define def_dtor(class_name) \
    void _dtor_func_name(class_name)(ptr_of(class_name) self)

// 析构父类
#define dtor_continue(base_class)                                        \
    do                                                                   \
    {                                                                    \
        _dtor_func_name(base_class)(ptr_cast(ptr_of(base_class), self)); \
        return;                                                          \
    } while (0)

// 定义默认构造函数
#define def_default_ctor(class_name, base_class) \
    def_ctor(class_name)                         \
    {                                            \
        super(base_class);                       \
    }

// 定义默认析构函数
#define def_default_dtor(class_name, base_class) \
    def_dtor(class_name)                         \
    {                                            \
        dtor_continue(base_class);               \
    }

// 同时定义默认构造函数和默认析构函数
#define def_default_ctor_and_dtor(class_name, base_class) \
    def_default_ctor(class_name, base_class) def_default_dtor(class_name, base_class)

// 初始化对象
#define init_obj(ptr, class_name, ...) \
    _ctor_func_name(class_name)(ptr, ##__VA_ARGS__)

// 调用析构函数
#define dispose_obj(ptr, class_name) \
    _dtor_func_name(class_name)(ptr)

// 创建新对象
#define new_obj(ptr, class_name, ...)                           \
    do                                                          \
    {                                                           \
        (ptr) = (ptr_of(class_name))malloc(sizeof(class_name)); \
        if ((ptr) != NULL)                                      \
            init_obj(ptr, class_name, ##__VA_ARGS__);           \
    } while (0)

// 销毁对象
#define delete_obj(ptr, class_name)   \
    do                                \
    {                                 \
        dispose_obj(ptr, class_name); \
        free(ptr);                    \
    } while (0)

// 获取字段
#define field(ptr, class_name, field_name) \
    (ptr_cast(ptr_of(class_name), ptr)->field_name)

/*============================================================*/

// 获取对象的虚函数表
#define _get_virtual_table(ptr) \
    (ptr_cast(ptr_of(Object), ptr)->_pVirtualTable)

// 定义虚函数表开始
#define def_vtable(class_name, base_class)                                \
    struct _virtual_table_struct_name(class_name)                         \
    {                                                                     \
        struct _virtual_table_struct_name(base_class) _vtableOfBaseClass; \
        struct

// 定义虚函数表结束
#define end_vtable(class_name) \
    ;                          \
    }                          \
    _virtual_table_name(class_name)

// 绑定对象的虚函数表
#define bind_vtable(class_name) \
    (_get_virtual_table(self) = &_virtual_table_name(class_name))

// 没有新的虚函数时可定义默认的虚函数表
#define def_default_vtable(class_name, base_class) \
    def_vtable(class_name, base_class)             \
    {                                              \
        int _;                                     \
    }                                              \
    end_vtable(class_name)

// 定义虚函数表初始化函数
#define def_vtable_initializer(class_name) \
    void _virtual_table_initializer_name(class_name)(struct _virtual_table_struct_name(class_name) * vtable)

// 初始化虚函数表
#define init_vtable(class_name) \
    _virtual_table_initializer_name(class_name)(&_virtual_table_name(class_name))

// 初始化虚函数表的父类部分
#define vtable_super(base_class) \
    _virtual_table_initializer_name(base_class)((struct _virtual_table_struct_name(base_class) *)vtable)

// 无重载虚函数时可定义默认虚函数初始化函数
#define def_default_vtable_initializer(class_name, base_class) \
    def_vtable_initializer(class_name)                         \
    {                                                          \
        vtable_super(base_class);                              \
    }

// 绑定虚函数
#define vtable_bind(class_name, func_name) \
    (vtable->func_name = _virtual_func_name(class_name, func_name))

// 重写虚函数
#define vtable_override(class_name, class_of_func, func_name) \
    (((struct _virtual_table_struct_name(class_of_func) *)vtable)->func_name = _virtual_func_name(class_name, func_name))

// 虚函数表中声明虚函数
#define decl_vfunc(class_name, func_name, ret_type, ...) \
    ret_type (*func_name)(void *self, ##__VA_ARGS__)

// 定义虚函数
#define def_vfunc(class_name, func_name, ret_type, ...) \
    ret_type _virtual_func_name(class_name, func_name)(void *self, ##__VA_ARGS__)

// 调用虚函数
#define call_vfunc(ptr, class_of_vfunc, func_name, ...)                                     \
    (ptr_cast(struct _virtual_table_struct_name(class_of_vfunc) *, _get_virtual_table(ptr)) \
         ->func_name(ptr, ##__VA_ARGS__))

/*============================================================*/

// Object类
typedef struct _class_struct_name(Object)
{
    void *_pVirtualTable;
}
Object;

struct _virtual_table_struct_name(Object) { int _; }
_virtual_table_name(Object);

def_ctor(Object) { bind_vtable(Object); }
def_dtor(Object) {}

def_vtable_initializer(Object) {}

/*============================================================*/

#endif //_CWC_H_
