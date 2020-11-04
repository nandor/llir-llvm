//
// RUN: %clang_cc1 -E -dM -ffreestanding -fgnuc-version=4.2.1 -triple=llir_x86_64-none-none < /dev/null | FileCheck -match-full-lines -check-prefix X86_64 %s
// RUN: %clang_cc1 -x c++ -E -dM -ffreestanding -fgnuc-version=4.2.1 -triple=llir_x86_64-none-none < /dev/null | FileCheck -match-full-lines -check-prefix X86_64 -check-prefix X86_64-CXX %s
//
// X86_64:#define _LP64 1
// X86_64-NOT:#define _LP32 1
// X86_64:#define __BIGGEST_ALIGNMENT__ 16
// X86_64:#define __BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__
// X86_64:#define __CHAR16_TYPE__ unsigned short
// X86_64:#define __CHAR32_TYPE__ unsigned int
// X86_64:#define __CHAR_BIT__ 8
// X86_64:#define __DBL_DENORM_MIN__ 4.9406564584124654e-324
// X86_64:#define __DBL_DIG__ 15
// X86_64:#define __DBL_EPSILON__ 2.2204460492503131e-16
// X86_64:#define __DBL_HAS_DENORM__ 1
// X86_64:#define __DBL_HAS_INFINITY__ 1
// X86_64:#define __DBL_HAS_QUIET_NAN__ 1
// X86_64:#define __DBL_MANT_DIG__ 53
// X86_64:#define __DBL_MAX_10_EXP__ 308
// X86_64:#define __DBL_MAX_EXP__ 1024
// X86_64:#define __DBL_MAX__ 1.7976931348623157e+308
// X86_64:#define __DBL_MIN_10_EXP__ (-307)
// X86_64:#define __DBL_MIN_EXP__ (-1021)
// X86_64:#define __DBL_MIN__ 2.2250738585072014e-308
// X86_64:#define __DECIMAL_DIG__ __LDBL_DECIMAL_DIG__
// X86_64:#define __FLT_DENORM_MIN__ 1.40129846e-45F
// X86_64:#define __FLT_DIG__ 6
// X86_64:#define __FLT_EPSILON__ 1.19209290e-7F
// X86_64:#define __FLT_EVAL_METHOD__ 0
// X86_64:#define __FLT_HAS_DENORM__ 1
// X86_64:#define __FLT_HAS_INFINITY__ 1
// X86_64:#define __FLT_HAS_QUIET_NAN__ 1
// X86_64:#define __FLT_MANT_DIG__ 24
// X86_64:#define __FLT_MAX_10_EXP__ 38
// X86_64:#define __FLT_MAX_EXP__ 128
// X86_64:#define __FLT_MAX__ 3.40282347e+38F
// X86_64:#define __FLT_MIN_10_EXP__ (-37)
// X86_64:#define __FLT_MIN_EXP__ (-125)
// X86_64:#define __FLT_MIN__ 1.17549435e-38F
// X86_64:#define __FLT_RADIX__ 2
// X86_64:#define __INT16_C_SUFFIX__
// X86_64:#define __INT16_FMTd__ "hd"
// X86_64:#define __INT16_FMTi__ "hi"
// X86_64:#define __INT16_MAX__ 32767
// X86_64:#define __INT16_TYPE__ short
// X86_64:#define __INT32_C_SUFFIX__
// X86_64:#define __INT32_FMTd__ "d"
// X86_64:#define __INT32_FMTi__ "i"
// X86_64:#define __INT32_MAX__ 2147483647
// X86_64:#define __INT32_TYPE__ int
// X86_64:#define __INT64_C_SUFFIX__ L
// X86_64:#define __INT64_FMTd__ "ld"
// X86_64:#define __INT64_FMTi__ "li"
// X86_64:#define __INT64_MAX__ 9223372036854775807L
// X86_64:#define __INT64_TYPE__ long int
// X86_64:#define __INT8_C_SUFFIX__
// X86_64:#define __INT8_FMTd__ "hhd"
// X86_64:#define __INT8_FMTi__ "hhi"
// X86_64:#define __INT8_MAX__ 127
// X86_64:#define __INT8_TYPE__ signed char
// X86_64:#define __INTMAX_C_SUFFIX__ L
// X86_64:#define __INTMAX_FMTd__ "ld"
// X86_64:#define __INTMAX_FMTi__ "li"
// X86_64:#define __INTMAX_MAX__ 9223372036854775807L
// X86_64:#define __INTMAX_TYPE__ long int
// X86_64:#define __INTMAX_WIDTH__ 64
// X86_64:#define __INTPTR_FMTd__ "ld"
// X86_64:#define __INTPTR_FMTi__ "li"
// X86_64:#define __INTPTR_MAX__ 9223372036854775807L
// X86_64:#define __INTPTR_TYPE__ long int
// X86_64:#define __INTPTR_WIDTH__ 64
// X86_64:#define __INT_FAST16_FMTd__ "hd"
// X86_64:#define __INT_FAST16_FMTi__ "hi"
// X86_64:#define __INT_FAST16_MAX__ 32767
// X86_64:#define __INT_FAST16_TYPE__ short
// X86_64:#define __INT_FAST32_FMTd__ "d"
// X86_64:#define __INT_FAST32_FMTi__ "i"
// X86_64:#define __INT_FAST32_MAX__ 2147483647
// X86_64:#define __INT_FAST32_TYPE__ int
// X86_64:#define __INT_FAST64_FMTd__ "ld"
// X86_64:#define __INT_FAST64_FMTi__ "li"
// X86_64:#define __INT_FAST64_MAX__ 9223372036854775807L
// X86_64:#define __INT_FAST64_TYPE__ long int
// X86_64:#define __INT_FAST8_FMTd__ "hhd"
// X86_64:#define __INT_FAST8_FMTi__ "hhi"
// X86_64:#define __INT_FAST8_MAX__ 127
// X86_64:#define __INT_FAST8_TYPE__ signed char
// X86_64:#define __INT_LEAST16_FMTd__ "hd"
// X86_64:#define __INT_LEAST16_FMTi__ "hi"
// X86_64:#define __INT_LEAST16_MAX__ 32767
// X86_64:#define __INT_LEAST16_TYPE__ short
// X86_64:#define __INT_LEAST32_FMTd__ "d"
// X86_64:#define __INT_LEAST32_FMTi__ "i"
// X86_64:#define __INT_LEAST32_MAX__ 2147483647
// X86_64:#define __INT_LEAST32_TYPE__ int
// X86_64:#define __INT_LEAST64_FMTd__ "ld"
// X86_64:#define __INT_LEAST64_FMTi__ "li"
// X86_64:#define __INT_LEAST64_MAX__ 9223372036854775807L
// X86_64:#define __INT_LEAST64_TYPE__ long int
// X86_64:#define __INT_LEAST8_FMTd__ "hhd"
// X86_64:#define __INT_LEAST8_FMTi__ "hhi"
// X86_64:#define __INT_LEAST8_MAX__ 127
// X86_64:#define __INT_LEAST8_TYPE__ signed char
// X86_64:#define __INT_MAX__ 2147483647
// X86_64:#define __LDBL_DENORM_MIN__ 3.64519953188247460253e-4951L
// X86_64:#define __LDBL_DIG__ 18
// X86_64:#define __LDBL_EPSILON__ 1.08420217248550443401e-19L
// X86_64:#define __LDBL_HAS_DENORM__ 1
// X86_64:#define __LDBL_HAS_INFINITY__ 1
// X86_64:#define __LDBL_HAS_QUIET_NAN__ 1
// X86_64:#define __LDBL_MANT_DIG__ 64
// X86_64:#define __LDBL_MAX_10_EXP__ 4932
// X86_64:#define __LDBL_MAX_EXP__ 16384
// X86_64:#define __LDBL_MAX__ 1.18973149535723176502e+4932L
// X86_64:#define __LDBL_MIN_10_EXP__ (-4931)
// X86_64:#define __LDBL_MIN_EXP__ (-16381)
// X86_64:#define __LDBL_MIN__ 3.36210314311209350626e-4932L
// X86_64:#define __LITTLE_ENDIAN__ 1
// X86_64:#define __LONG_LONG_MAX__ 9223372036854775807LL
// X86_64:#define __LONG_MAX__ 9223372036854775807L
// X86_64:#define __LP64__ 1
// X86_64-NOT:#define __ILP32__ 1
// X86_64:#define __MMX__ 1
// X86_64:#define __NO_MATH_INLINES 1
// X86_64:#define __POINTER_WIDTH__ 64
// X86_64:#define __PTRDIFF_TYPE__ long int
// X86_64:#define __PTRDIFF_WIDTH__ 64
// X86_64:#define __REGISTER_PREFIX__
// X86_64:#define __SCHAR_MAX__ 127
// X86_64:#define __SHRT_MAX__ 32767
// X86_64:#define __SIG_ATOMIC_MAX__ 2147483647
// X86_64:#define __SIG_ATOMIC_WIDTH__ 32
// X86_64:#define __SIZEOF_DOUBLE__ 8
// X86_64:#define __SIZEOF_FLOAT__ 4
// X86_64:#define __SIZEOF_INT__ 4
// X86_64:#define __SIZEOF_LONG_DOUBLE__ 16
// X86_64:#define __SIZEOF_LONG_LONG__ 8
// X86_64:#define __SIZEOF_LONG__ 8
// X86_64:#define __SIZEOF_POINTER__ 8
// X86_64:#define __SIZEOF_PTRDIFF_T__ 8
// X86_64:#define __SIZEOF_SHORT__ 2
// X86_64:#define __SIZEOF_SIZE_T__ 8
// X86_64:#define __SIZEOF_WCHAR_T__ 4
// X86_64:#define __SIZEOF_WINT_T__ 4
// X86_64:#define __SIZE_MAX__ 18446744073709551615UL
// X86_64:#define __SIZE_TYPE__ long unsigned int
// X86_64:#define __SIZE_WIDTH__ 64
// X86_64:#define __SSE_MATH__ 1
// X86_64:#define __SSE__ 1
// X86_64-CXX:#define __STDCPP_DEFAULT_NEW_ALIGNMENT__ 16UL
// X86_64:#define __UINT16_C_SUFFIX__
// X86_64:#define __UINT16_MAX__ 65535
// X86_64:#define __UINT16_TYPE__ unsigned short
// X86_64:#define __UINT32_C_SUFFIX__ U
// X86_64:#define __UINT32_MAX__ 4294967295U
// X86_64:#define __UINT32_TYPE__ unsigned int
// X86_64:#define __UINT64_C_SUFFIX__ UL
// X86_64:#define __UINT64_MAX__ 18446744073709551615UL
// X86_64:#define __UINT64_TYPE__ long unsigned int
// X86_64:#define __UINT8_C_SUFFIX__
// X86_64:#define __UINT8_MAX__ 255
// X86_64:#define __UINT8_TYPE__ unsigned char
// X86_64:#define __UINTMAX_C_SUFFIX__ UL
// X86_64:#define __UINTMAX_MAX__ 18446744073709551615UL
// X86_64:#define __UINTMAX_TYPE__ long unsigned int
// X86_64:#define __UINTMAX_WIDTH__ 64
// X86_64:#define __UINTPTR_MAX__ 18446744073709551615UL
// X86_64:#define __UINTPTR_TYPE__ long unsigned int
// X86_64:#define __UINTPTR_WIDTH__ 64
// X86_64:#define __UINT_FAST16_MAX__ 65535
// X86_64:#define __UINT_FAST16_TYPE__ unsigned short
// X86_64:#define __UINT_FAST32_MAX__ 4294967295U
// X86_64:#define __UINT_FAST32_TYPE__ unsigned int
// X86_64:#define __UINT_FAST64_MAX__ 18446744073709551615UL
// X86_64:#define __UINT_FAST64_TYPE__ long unsigned int
// X86_64:#define __UINT_FAST8_MAX__ 255
// X86_64:#define __UINT_FAST8_TYPE__ unsigned char
// X86_64:#define __UINT_LEAST16_MAX__ 65535
// X86_64:#define __UINT_LEAST16_TYPE__ unsigned short
// X86_64:#define __UINT_LEAST32_MAX__ 4294967295U
// X86_64:#define __UINT_LEAST32_TYPE__ unsigned int
// X86_64:#define __UINT_LEAST64_MAX__ 18446744073709551615UL
// X86_64:#define __UINT_LEAST64_TYPE__ long unsigned int
// X86_64:#define __UINT_LEAST8_MAX__ 255
// X86_64:#define __UINT_LEAST8_TYPE__ unsigned char
// X86_64:#define __USER_LABEL_PREFIX__
// X86_64:#define __WCHAR_MAX__ 2147483647
// X86_64:#define __WCHAR_TYPE__ int
// X86_64:#define __WCHAR_WIDTH__ 32
// X86_64:#define __WINT_TYPE__ int
// X86_64:#define __WINT_WIDTH__ 32
// X86_64:#define __amd64 1
// X86_64:#define __amd64__ 1
// X86_64:#define __code_model_small__ 1
// X86_64:#define __x86_64 1
// X86_64:#define __x86_64__ 1
