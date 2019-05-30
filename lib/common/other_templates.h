#pragma once

// 将动态对象的生命周期绑定至此类型实例
// 现在只支持new出来的动态对象
template<typename T>
class AutoDeletor
{
public:
    AutoDeletor(T* p) : p_(p) {}
    ~AutoDeletor() { delete p_; }
private:
    T *p_;
};