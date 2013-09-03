#include <stdint.h>

typedef const char * Token;
typedef int8_t (*loaderCallback)(Token);

int8_t loadFromFile(const char * path, loaderCallback fnCallback);
