#pragma once

typedef signed char          int8;
typedef unsigned char       uint8;
typedef signed short        int16;
typedef unsigned short     uint16;
typedef signed int          int32;
typedef unsigned int       uint32;
typedef signed long long    int64;
typedef unsigned long long uint64;

typedef _Bool bool;
#define false (uint8)0
#define true (uint8)1

#define NULL (void*)0

typedef char* string;
typedef const char* cstring;

typedef struct IDKSize
{
	uint64 w;
	uint64 h;
} IDKSize;

typedef struct IDKPos
{
	int64 x;
	int64 y;
} IDKPos;
