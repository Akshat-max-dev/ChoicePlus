#pragma once
#ifdef DEBUG
#include<assert.h>
#define cpassert(_EXPR)\
if(!_EXPR){__debugbreak();}
#elif defined(RELEASE)
#define cpassert(_EXPR)
#endif