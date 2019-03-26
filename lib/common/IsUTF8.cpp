#include "IsUTF8.h"

// 注: 此代码是 copy 网络上的, 具体出处不详

bool IsUTF8(const char* buffer, long size)   
{
    bool is_utf8 = true;
    unsigned char* start = (unsigned char*)buffer;
    unsigned char* end = (unsigned char*)buffer + size;
    while (start < end)
    {
        if (*start < 0x80) // (10000000): 值小于0x80的为ASCII字符   
        {
            start++;
        }
        else if (*start < (0xC0)) // (11000000): 值介于0x80与0xC0之间的为无效UTF-8字符   
        {
            is_utf8 = false;
            break;
        }
        else if (*start < (0xE0)) // (11100000): 此范围内为2字节UTF-8字符   
        {
            if (start >= end - 1)
                break;
            if ((start[1] & (0xC0)) != 0x80)
            {
                is_utf8 = false;
                break;
            }
            start += 2;
        }
        else if (*start < (0xF0)) // (11110000): 此范围内为3字节UTF-8字符   
        {
            if (start >= end - 2)
                break;
            if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80)
            {
                is_utf8 = false;
                break;
            }
            start += 3;
        }
        else
        {
            is_utf8 = false;
            break;
        }
    }

    return is_utf8;
}

bool IsUTF8_Bom(const char* buffer, long size)
{
	if (!IsUTF8(buffer, size)) return false;

	if (static_cast<unsigned char>(buffer[0]) == 0xEF && 
	    static_cast<unsigned char>(buffer[1]) == 0xBB && 
		static_cast<unsigned char>(buffer[2]) == 0xBF)
	{
		return true;
	}
	else
	{
		return false;
	}
}
