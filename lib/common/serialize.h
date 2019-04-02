#pragma once

#include <cstring>
#include <stdexcept>

/*
	简单的序列化工具，提供序列化内置类型、数组、聚合类型的方法
    正确有序列化顺序由用户保证
*/
class Serializer
{
public:
    Serializer(char *buffer, size_t len) : m_self_buffer(false), m_len(len), m_buffer(buffer), m_cur_pos(m_buffer) {}
	Serializer(size_t len = 1024) : m_self_buffer(true), m_len(len), m_buffer(new char[m_len]{}), m_cur_pos(m_buffer) {}
	~Serializer()
    {
        if (m_self_buffer) 
            delete []m_buffer;
    }

	size_t Size() const { return m_cur_pos - m_buffer; }	// 当前序列化缓冲区中的数据长度
	char* Ptr() const { return m_buffer; }
		
	template<typename T>
	void Write(const T &v)
	{
		while (m_cur_pos - m_buffer + sizeof(T) > m_len)
		{
            if (m_self_buffer)
			    ReAllocate();
            else
                throw std::overflow_error("Serializer::Write failed: no more space to write");
            
		}
		memcpy(m_cur_pos, &v, sizeof(T));
		m_cur_pos += sizeof(T);
	}

    void Write(const char *data, size_t len)
    {
        while (m_cur_pos - m_buffer + len > m_len)
		{
			if (m_self_buffer)
			    ReAllocate();
            else
                throw std::overflow_error("Serializer::Write failed: no more space to write");
		}
		memcpy(m_cur_pos, data, len);
		m_cur_pos += len;
    }

private:
	// 拒绝拷贝
	Serializer(const Serializer&);
	Serializer& operator=(const Serializer&);

	void ReAllocate()
	{
		char *new_buffer = new char[2 * m_len]{};
		memcpy(new_buffer, m_buffer, m_len);
		m_cur_pos = new_buffer + Size();
		delete[]m_buffer;
		m_buffer = new_buffer;
		m_len *= 2;
	}

    bool m_self_buffer;         // 是否是内部 buffer, 内部的可否增长
	size_t m_len;				// buffer长度
	char *m_buffer;				// 用于存储序列化数据的缓冲区，可自动增长
	char *m_cur_pos;			// 当前序列化的位置
};

class DeSerializer
{
public:
	DeSerializer(const char *buffer, size_t len) : m_max_len(len), m_buffer(buffer), m_cur_pos(m_buffer) {}

	template<typename T>
	void Read(T &v)
	{
		if (m_cur_pos - m_buffer + sizeof(v) > m_max_len)
		{
			throw std::overflow_error("DeSerializer::Read failed: no more data to read");
		}
		memcpy(&v, m_cur_pos, sizeof(v));
		m_cur_pos += sizeof(v);
	}

    void Read(char* data, size_t len)
    {
        if (m_cur_pos - m_buffer + len > m_max_len)
		{
			throw std::overflow_error("DeSerializer::Read failed: no more data to read");
		}
		memcpy(data, m_cur_pos, len);
		m_cur_pos += len;
    }

private:
	// 拒绝拷贝
	DeSerializer(const DeSerializer&);
	DeSerializer& operator=(const DeSerializer&);

	const size_t m_max_len;			// buffer最大长度
	const char *const m_buffer;		// 用于反序列化数据的缓冲区
	const char *m_cur_pos;			// 当前反序列化的位置
};
