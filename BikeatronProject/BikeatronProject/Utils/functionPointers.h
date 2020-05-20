/*
 * functionPointers.h
 *
 * Created: 23-04-2020 15:29:15
 *  Author: aaron
 */ 


#ifndef FUNCTIONPOINTERS_H_
#define FUNCTIONPOINTERS_H_

#include <stdint.h>
#include <stdbool.h>

typedef void(*void_void_f_t)(void);
typedef void(*void_bool_f_t)(bool);
typedef void(*void_uint8_f_t)(uint8_t);
typedef void(*void_2_uint8_f_t)(uint8_t,uint8_t);
typedef void(*void_uint16_f_t)(uint16_t);
typedef void(*void_2_uint16_f_t)(uint16_t,uint16_t);
typedef void(*void_string_f_t)(char*);
typedef void(*void_float_f_t)(float);
typedef void(*void_2_uint8ptr_f_t)(uint8_t*,uint8_t*);
typedef void(*void_uint16ptr_f_t)(uint16_t*);
typedef void(*void_uint32ptr_f_t)(uint32_t*);
typedef void(*void_doubleptr_f_t)(double*);

typedef bool(*bool_void_f_t)(void);
typedef bool(*bool_bool_f_t)(bool);
typedef uint8_t(*uint8_void_f_t)(void);
typedef uint8_t(*uint8_bool_f_t)(bool);
typedef uint8_t(*uint8_uint8_f_t)(uint8_t);
typedef uint16_t(*uint16_void_f_t)(void);
typedef uint16_t (*uint16_uint32_bool_f_t)(uint32_t,bool);
typedef float(*float_void_f_t)(void);


#endif /* FUNCTIONPOINTERS_H_ */