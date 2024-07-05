#include <concepts>
#include <type_traits>
#include <utility>

template <typename T>
concept SING_CONCEPT = requires(T t) {
    { T(t) } -> std::same_as<void>; // 检查拷贝构造函数
    { T(std::move(t)) } -> std::same_as<void>; // 检查移动构造函数
    { t = t } -> std::same_as<T&>; // 检查赋值运算符
    {t.getInstance()} -> std::same_as<T&>; // 检查获取实例的方法
};

template<SING_CONCEPT T>
class Single
{
    T instance;
};

