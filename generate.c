#include <string.h>
#include "desk.h"
#include "class.h"

void generate(char *file_name, Compiler *compiler){
    FILE *fp;
    int i;
    unsigned char buf[4] = {0xca, 0xfe,  0xba, 0xbe};
    
    for(i = strlen(file_name) - 1; 0 <= i; i--){
        if(file_name[i] == '.'){
            file_name[i] = '\0';
            break;
        }
    }
    file_name = strcat(file_name, ".class");

    if(!(fp = fopen(file_name, "wb")))
        compile_error(0, "could not create class file.");
    fwrite(buf, sizeof(unsigned char), 4, fp);
    fclose(fp);

    printf("generated %s\n", file_name);
}
