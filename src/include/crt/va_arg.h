#define va_arg(AP, TYPE)	\
	(AP += VA_SIZE(TYPE), *((TYPE *)(AP - VA_SIZE(TYPE))))
