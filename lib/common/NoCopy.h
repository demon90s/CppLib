#pragma once

// 无法拷贝的对象, 目的在于阻止类型的拷贝

class NoCopy {
public:
    NoCopy() {}
    ~NoCopy() {}
    NoCopy(const NoCopy&) = delete;
    NoCopy& operator=(const NoCopy&) = delete;
};