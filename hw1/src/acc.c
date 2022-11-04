#include "hmm.h"
#include <math.h>

int main() {
    char buffer1[100] = "";
    char buffer2[100] = "";
    char discard[100] = "";

    FILE *fc = open_or_die( "./result.txt", "r");
    FILE *fa = open_or_die( "./data/test_lbl.txt", "r");

    int acc = 0, total = 0;
    while(!feof(fa)){
        int u;
        u=fscanf( fa, "%s\n", buffer2 );
        u=fscanf( fc, "%s %s\n", buffer1 , discard);
        total++;
        if(buffer1[7]==buffer2[7])  acc++;
    }
    fclose(fc);
    fclose(fa);
    double ans = 0;
    ans = acc;
    ans/=total;
    printf("accuracy : %d / %d = %lf\n", acc, total, ans);
    return 0;
}

