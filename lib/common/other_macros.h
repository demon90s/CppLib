#pragma once

// 忽略掉 noused warning
#define IGNORE_NOUSED_WARNING(identifier) ((void)identifier)

// 对齐 example: DOALIGH(10000, 1024) -> 10240
#define DOALIGN(x,b)   (((x) + (b) - 1) & ~((b) - 1))