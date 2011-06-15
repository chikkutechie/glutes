#include <iostream>
#include <fstream>
#include <string>
#include "rmk.h"

using namespace std;

extern "C" int yyparse ();

extern "C" FILE *yyin;

int main(int argc, char **argv)
{
    int ret = 1;

    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        ret = yyparse();
        if (ret == 0) {
            generateOutput();
        }
    }
    
    return ret;
}
