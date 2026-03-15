#ifndef PRINT_H
#define PRINT_H
#include "../../typos.h"
void print(str string, u64 size);
#define s_print(string) {char out[]=string;print(out,sizeof(out));};
#endif
