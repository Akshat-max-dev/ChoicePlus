#pragma once
#ifdef DEBUG
#include<assert.h>
#define cpassert(_EXPR)  assert(_EXPR)
#elif defined(RELEASE)
#define cpassert(_EXPR)
#endif