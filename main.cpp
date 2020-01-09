#include <iostream>
#include <string>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "node.h"
#include "analyzer.h"
#include "analyzerconsts.h"

using namespace std;

char SupDotFile[] = "Sup.dot";

int main(int argc, char* argv[]) {
    //puts("To create a tree graph, enter the file name (.dot extension) as the first command line parameter.");
    puts("To save the graph-tree of an expression with a picture, enter the file name (.png expression) as the first parametr of command line.");
    puts("Enter a calculated expression, without spaces, with a trailing character #");
    char input[1024];
    if(fgets(input, 1024, stdin) == NULL) {
        fprintf(stderr, "Expression not entered! (fgets failed)\n");
        return -1;
    }
    Analyzer *a = new Analyzer(input);
    if(argc > 1 && a->tree != NULL) {
        a->tree->MakeGraphFile(SupDotFile);
        sprintf(input, "dot %s -Tpng -o %s", SupDotFile, argv[1]);
        if(system(input) == -1) {
            if(argv[1] == NULL)
                fprintf(stderr, "system: null argument!\n");
            else
                fprintf(stderr, "Impossible to create picture %s, no access to shell, or impossible to create child process! (Command \"system\" failed) \n", argv[1]);
        }
        else
            printf("Tree successfully built, try to see \"%s\"!", argv[1]);
    }
    delete a;
    a = NULL;
    return 0;
}
