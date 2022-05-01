//C language runtime String includes

char* strcat(char* destination, const char* source);
size_t strlen(const char* str);
char * strchr (char * str, int character);
char* strstr(char* str1, const char *str2);
char* strcpy(char *destination, const char *source);
char* strncpy(char *destination, const char *source, size_t num);
char* strupr(char *str);
int strcmp(const char* str1, const char* str2);

void* memset(void *str, int c, size_t n);
void *memcpy(void *dest, const void * src, size_t n);


