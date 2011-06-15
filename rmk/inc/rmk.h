
#ifndef _RMK_H_
#define _RMK_H_

#undef YYSTYPE

#include "rmk.tab.h"

void generateOutput();

#ifdef __cplusplus
extern "C"
{
#endif


struct NameValPair
{
    int name;
    void * data;
};

struct NameValPair * createNVPair(int name, char * value);
struct NameValPair * addNVPair(struct NameValPair * pair, int name, char * value);

void addNameValPairProperty(int name, struct NameValPair * values);


#ifdef __cplusplus
}
#endif

#endif

