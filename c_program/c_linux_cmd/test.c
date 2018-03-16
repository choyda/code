#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lib/str_match.h"


int main (int argc, char *argv[])
{
    struct finded_str   re[1024];
    int                 rc,i;
    unsigned int        last = 0;

    rc = find_str(argv[1], argv[2], re, 1024);
    if(rc < 0)
        pr_msg("findstr error[%d]\n", rc);
    if(rc == 0)
        pr_msg("can't finded\n");

    for(i = 0; i < rc; ++i){
        write(STDOUT_FILENO, argv[2]+last, re[i].begin-last);
        write(STDOUT_FILENO, argv[3], strlen(argv[3]));
        last = re[i].end;
    }
    printf("\n");
   
    return 0;
}

