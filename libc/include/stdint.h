#ifndef STDINT_H
#define STDINT_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef char int8_t;
typedef short int16_t;
typedef int int32_t;

#define UINT8_MIN 0
#define UINT8_MAX 255u
#define UINT16_MIN 0
#define UINT16_MAX 65535u
#define UINT32_MIN 0
#define UINT32_MAX 4294967295u

#define INT8_MIN (-128)
#define INT8_MAX 127
#define INT16_MIN (-32768)
#define INT16_MAX 32767
#define INT32_MIN (-2147483648)
#define INT32_MAX 2147483647

#endif
