#pragma once

#include <string.h>
#include <stdexcept>

// 定长队列，T必须是POD类型
template<typename T, size_t LEN>
class RawQueue
{
public:
    void Push(const T &v)
    {
        memmove(m_q + 1, m_q, sizeof(m_q) - sizeof(m_q[0]));
        m_q[0] = v;

        if (m_len < LEN)
            m_len++;
    }

    int Size() const
    {
        return m_len;
    }

    T& operator[](int index)
    {
        if (index < 0 || index >= m_len) {
            throw std::logic_error("RawQueue out of range");
        }
        return m_q[index];
    }

private:
    int m_len = 0;
    T m_q[LEN] {};
};