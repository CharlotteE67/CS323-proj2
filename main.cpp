#include "syntax.tab.c"

#define DEBUG false

int main(int argc,char **argv){
    //from lab2 sample
    if(argc <= 1){
        fprintf(LEX_ERR_OP, "Usage: %s <file_path>\n", argv[0]);
        return EXIT_FAIL;
    } else {
        FILE *f = fopen(argv[1], "r");
        if(!f){
            fprintf(LEX_ERR_OP, "Can't open the file %s\n", argv[1]);
            return EXIT_FAIL;
        }
        yyrestart(f);
        yyparse();
    }
    if (argc >= 3 && argv[2][0] == 't') {
        root->show(0);
    }
    return EXIT_OK;
}