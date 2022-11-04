#include "hmm.h"
#include <math.h>

enum Observes{
    A = 0,
    B,
    C,
    D,
    E,
    F
};

int main(int argc, char* argv[])
{
	if (argc != 4){
        printf("Error!!! Wrong Number of argument.\n");
        return 0;
    }
    //printf("*** Testing Start ***\n");

    /*  Read argv   */
    char * modellist = argv[1];
    char * observe_seq = argv[2];
	char * output_file = argv[3];

	/*	Load Models	*/
	HMM hmms[5];
	load_models(modellist, hmms, 5);
	int numOfModels=5;

    /*  load training data  */
	FILE *fp = open_or_die( observe_seq, "r");
    char token[MAX_LINE] = "";
	int o_frame[MAX_LINE];

    /*  write file  */
    FILE *fo = open_or_die( output_file, "w");

	while( fscanf( fp, "%s", token ) > 0 ){

		/*  Read Line */
        int T = strlen(token);
		for(int i=0; ;i++)
		{
			if( token[i] == '\0' || token[i] == '\n' ) break; // Skip to next line
			switch(token[i])
			{
				case 'A': o_frame[i] = A; break;
				case 'B': o_frame[i] = B; break;
				case 'C': o_frame[i] = C; break;
				case 'D': o_frame[i] = D; break;
				case 'E': o_frame[i] = E; break;
				case 'F': o_frame[i] = F; break;
				default: break;
			}
		}

        /*  Accuracy Calculation  */
        double P_star = 0;
        int which = 0;
        for(int num=0; num<numOfModels; num++)
        {
            double delta[50][6] = {{0}};
            int N = hmms[num].state_num;	
            // Initialization
            for(int i=0; i<N; i++){
                delta[0][i] = hmms[num].initial[i] * hmms[num].observation[o_frame[0]][i];
            }

            // Recursion
            for(int t=1; t<T; t++){
                for(int j=0; j<N; j++){
                    // Find Max
                    double max_p = 0;
                    for (int i=0; i<N; i++){
                        if ( max_p < delta[t-1][i] * hmms[num].transition[i][j]){
                            max_p = delta[t-1][i] * hmms[num].transition[i][j];
                        }
                    }
                    
                    // delta
                    delta[t][j] = max_p * hmms[num].observation[o_frame[t]][j];
                }
            }

            // Termination
            for(int i=0; i<N; i++){
                if(P_star <= delta[50 - 1][i]){
                    P_star = delta[50 - 1][i];
                    which = num;
                }
            }

        }

        fprintf( fo, "%s ", hmms[which].model_name);
		fprintf( fo, "%.6e\n", P_star);
  
	}

	fclose(fp);
    fclose(fo);

    //printf("*** Testing Done ***\n");

	return 0;
}
