#pragma once

// buffer 是否是 utf8 格式的
bool IsUTF8(const char* buffer, long size);

// buffer 是否是 utf8 格式带 BOM 的
bool IsUTF8_Bom(const char* buffer, long size);