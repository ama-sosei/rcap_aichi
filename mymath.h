#ifndef __MATH_H
#define __MATH_H

#if！defined（___ DEFS_H）
#include <_defs.h>
#endif

#define HUGE_VAL _huge_dble
extern double _Cdecl _huge_dble;
#define _LHUGE_VAL _huge_ldble
extern long double _Cdecl _huge_ldble;

#ifdef __cplusplus
extern "C" {
#endif
double _Cdecl _FARFUNC acos（double __x）;
double _Cdecl _FARFUNC asin（double __x）;
double _Cdecl atan（double __x）;
double _Cdecl _FARFUNC atan2（double __y、double __x）;
double _Cdecl _FARFUNC ceil（double __x）;
double _Cdecl _FARFUNC cos（double __x）;
double _Cdecl _FARFUNC cosh（double __x）;
double _Cdecl _FARFUNC exp（double __x）;
double _Cdecl fabs（double __x）;
double _Cdecl __fabs __（double __x）; / *本質的* /
double _Cdecl _FARFUNCフロア（double __x）;
double _Cdecl _FARFUNC fmod（double __x、double __y）;
double _Cdecl _FARFUNC frexp（double __x、int _FAR * __ exponent）;
double _Cdecl _FARFUNC ldexp（double __x、int __exponent）;
double _Cdecl _FARFUNCログ（double __x）;
double _Cdecl _FARFUNC log10（double __x）;
double _Cdecl _FARFUNC modf（double __x、double _FAR * __ ipart）;
double _Cdecl _FARFUNC pow（double __x、double __y）;
double _Cdecl _FARFUNC sin（double __x）;
double _Cdecl _FARFUNC sinh（double __x）;
double _Cdecl _FARFUNC sqrt（double __x）;
double _Cdecl _FARFUNC tan（double __x）;
double _Cdecl _FARFUNC tanh（double __x）;

long double _Cdecl _FARFUNC acosl（long double __x）;
long double _Cdecl _FARFUNC asinl（long double __x）;
long double _Cdecl _FARFUNC atan2l（long double __x、long double __y）;
long double _Cdecl atanl（long double __x）;
long double _Cdecl _FARFUNC ceill（long double __x）;
long double _Cdecl _FARFUNC coshl（long double __x）;
long double _Cdecl _FARFUNC cosl（long double __x）;
long double _Cdecl _FARFUNC expl（long double __x）;
long double _Cdecl fabsl（long double __x）;
long double _Cdecl _FARFUNC floorl（long double __x）;
long double _Cdecl _FARFUNC fmodl（long double __x、long double __y）;
long double _Cdecl _FARFUNC frexpl（long double __x、int _FAR * __ exponent）;
long double _Cdecl _FARFUNC ldexpl（long double __x、int __exponent）;
long double _Cdecl _FARFUNC log10l（long double __x）;
long double _Cdecl _FARFUNC logl（long double __x）;
long double _Cdecl _FARFUNC modfl（long double __x、long double _FAR * __ ipart）;
long double _Cdecl _FARFUNC powl（long double __x、long double __y）;
long double _Cdecl _FARFUNC sinhl（long double __x）;
long double _Cdecl _FARFUNC sinl（long double __x）;
long double _Cdecl _FARFUNC sqrtl（long double __x）;
long double _Cdecl _FARFUNC tanhl（long double __x）;
long double _Cdecl _FARFUNC tanl（long double __x）;

typedef列挙型
{{
    DOMAIN = 1、/ *引数ドメインエラー-ログ（-1）* /
    SING、/ *引数の特異点--pow（0、-2））* /
    オーバーフロー、/ *オーバーフロー範囲エラー-exp（1000）* /
    アンダーフロー、/ *アンダーフロー範囲エラー-exp（-1000）* /
    TLOSS、/ *有意性の完全な喪失-sin（10e70）* /
    PLOSS、/ *重要性の部分的な喪失。 -  使用されていない */
    STACKFAULT / *浮動小数点ユニットスタックオーバーフロー* /
} _mexcep;

#ifdef __cplusplus
}
#endif


#if！__ STDC__

構造体例外
{{
    int型;
    char _FAR * name;
    double arg1、arg2、retval;
};

struct _exceptionl
{{
    int型;
    char _FAR * name;
    long double arg1、arg2、retval;
};

#ifdef __cplusplus
extern "C" {
#endif
int _Cdecl abs（int __x）;
double _Cdecl atof（const char _FAR * __ s）;
double _Cdecl _FARFUNC hypot（double __x、double __y）;
long _Cdecl labs（long __x）;
int _Cdecl _FARFUNC matherr（構造体例外_FAR * __ e）;
double _Cdecl _FARFUNC poly（double __x、int __degree、double _FAR * __ coeffs）;
double _Cdecl _FARFUNC pow10（int __p）;
int _Cdecl _FARFUNC _matherrl（struct _exceptionl _FAR * __ e）;

long double _Cdecl _atold（const char _FAR * __ s）;
long double _Cdecl _FARFUNC hypotl（long double __x、long double __y）;
long double _Cdecl _FARFUNC polyl（long double __x、int __degree、long double _FAR * __ coeffs）;
long double _Cdecl _FARFUNC pow10l（int __p）;

#ifdef __cplusplus
    / * C ++ではcabの代わりにクラスcomplexを使用します* /
＃そうしないと
struct complex / *「cabs」関数で使用される* /
{{
    ダブルx、y;
};

struct _complexl / * "cabsl"関数で使用される* /
{{
    ロングダブルx、y;
};

#define cabs（z）（hypot（（z）.x、（z）.y））
#define cabsl（z）（hypotl（（z）.x、（z）.y））
#endif

#ifdef __cplusplus
}
#endif

/ *定数は小数点以下21桁に丸められます。* /
#define M_E 2.71828182845904523536
#define M_LOG2E 1.44269504088896340736
#define M_LOG10E 0.434294481903251827651
#define M_LN2 0.693147180559945309417
#define M_LN10 2.30258509299404568402
#define M_PI 3.14159265358979323846
#define M_PI_2 1.57079632679489661923
#define M_PI_4 0.785398163397448309616
#define M_1_PI 0.318309886183790671538
#define M_2_PI 0.636619772367581343076
#define M_1_SQRTPI 0.564189583547756286948
#define M_2_SQRTPI 1.12837916709551257390
#define M_SQRT2 1.41421356237309504880
#define M_SQRT_2 0.707106781186547524401

#define EDOM 33 / *数学の引数* /
#define ERANGE 34 / *結果が大きすぎます* /

#endif / *！__ STDC__ * /

#endif