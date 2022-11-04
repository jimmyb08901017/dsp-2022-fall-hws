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
    if (argc != 5){
        printf("Error!!! Wrong Number of argument.\n");
        return 0;
    }
    /*  Read argv   */
    int TRAIN_ITER = atoi(argv[1]);
    char * model_init = argv[2];
    char * filename = argv[3];
    char * dump_filename = argv[4];


    /*  initial */
	HMM new_hmm;
	loadHMM( &new_hmm, model_init );
    //printf("New HMM made.\n");
	//dumpHMM( stderr, &new_hmm );

    //printf("*** training start ***\n");
    for(int iter_num=0; iter_num < TRAIN_ITER; iter_num++)
    {
        int o_frame[MAX_LINE];
        memset(o_frame, 0, sizeof(o_frame));
        char token[MAX_LINE] = "";
        int N = 6;

        double pi_numerator[6] = {0};
        double epsilon[6][6] = {{0}};   //double aij_numerator[6][6] = {{0}};
        double aij_denominator[6] = {0};
        double bjk_numerator[6][6] = {{0}};
        double bjk_denominator[6] = {0};
        double gamma[50][6] = {{0}};
        double beta[MAX_SEQ][MAX_STATE] = {{0}};
        double alpha[MAX_SEQ][MAX_STATE] = {{0}};

        /*  load training data  */
        FILE *fp = open_or_die( filename, "r");
        int count = 0;
	    while( fscanf( fp, "%s", token ) > 0)
        {
            
            count ++;
            // Read One Line
            int T =0;
            for(int i=0; ;i++)
            {
                if( token[i] == '\0' || token[i] == '\n' ) break; // Skip to next line

                T++;
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


            /*******************************/
            /*  Forward Algorithm - alpha  */
            /*******************************/
            //double alpha[MAX_OBSERV][MAX_STATE] = {{0}}; // alpha[time][state]
            memset(alpha,0.0,sizeof(alpha));
            memset(beta,0,sizeof(beta));

            // Initialization
            for(int i=0; i<N; i++)
            {
                double temp = 0;
                temp = new_hmm.initial[i] * new_hmm.observation[token[0] - 65][i];
                alpha[0][i] = temp;
                
            }

            // Induction
            for(int t=0; t<T-1; t++)
            {
                for(int j=0; j<N; j++)
                {
                    // Sum up alpha(t-1)
                    double temp_sum = 0;
                    for (int i=0; i<N; i++)
                    {
                        temp_sum += alpha[t][i] * new_hmm.transition[i][j];
                    }
                    
                    // alpha
                    alpha[t+1][j] = temp_sum * new_hmm.observation[o_frame[t+1]][j];
                }
            }

            

            /*******************************/
            /*  Backward Algorithm - beta  */
            /*******************************/
             // beta[time][state]
            
            // Initialization
            for(int i=0; i<N; i++){
                beta[T-1][i] = 1;
            }

            // for(int i=0; i<N; i++){
            //     printf("alpha[0][%d] = %lf\n",i, alpha[0][i]);
            // }

            // Induction
            for(int t=T-2; t>=0; t--){
                for(int i=0; i<N; i++){
                    //beta[t][i] +=  new_hmm.transition[i][j] * new_hmm.observation[o_frame[t+1]][j] * beta[t+1][j];

                    double t_sum = 0.0;
                    for (int j=0; j<N; j++){
                        t_sum +=  new_hmm.transition[i][j] * new_hmm.observation[o_frame[t+1]][j] * beta[t+1][j];
                    } 
                    beta[t][i] = t_sum;
                }
                // printf("alpha[0][1] = %lf\n", alpha[0][1]);
                // printf("alpha[0][2] = %lf\n", alpha[0][2]);
                // printf("alpha[0][3] = %lf\n", alpha[0][3]);
            }
            // printf("------------------------\n");
            // printf("alpha[0][1] = %lf\n", alpha[0][1]);
            // printf("------------------------\n");
            // unsigned int i = 0;
            // while(i<6){
            //     printf("alpha[0][%d] = %lf\n", i, alpha[0][i]);
            //     i++;
            // }
            // printf("------------------------\n");
            /*******************************/
            /*     gamma = alpha * beta    */
            /*******************************/
            memset(gamma,0,sizeof(gamma));
            // Induction
            for(int t=0; t<T; t++){
                // Sum up 
                double temp_sum = 0;
                for (int j=0; j<N; j++){
                    temp_sum +=  alpha[t][j] * beta[t][j];
                }

                // gamma
                for(int i=0; i<N; i++){	
                    gamma[t][i] += alpha[t][i] * beta[t][i] / temp_sum;
                }
            }



            /*************/
            /*  epsilon  */
            /*************/

            for(int t=0; t<T-1; t++)
            {
                // normalization
                double mother=0;
                for(int i=0;i<N;i++){
                    for(int j=0;j<N;j++)
                    {
                        mother += alpha[t][i] * new_hmm.transition[i][j] * new_hmm.observation[o_frame[t+1]][j] * beta[t+1][j];
                    }
                }


                for(int i=0; i<N; i++){
                    for (int j=0; j<N; j++)
                    {
                        double temp;
                        temp = alpha[t][i] * new_hmm.transition[i][j] * new_hmm.observation[o_frame[t+1]][j] * beta[t+1][j];
                        temp /= mother;
                        epsilon[i][j] += temp;
                    }
                }
            }


              /******************/
             /*  accumulation  */
            /******************/


            for(int i=0; i<N; i++){
                pi_numerator[i] += gamma[0][i]; 
            }

            /*
            for(int i=0; i<N; i++){
                for(int j=0; j<N; j++){
                    for(int t=0; t<T-1; t++){
                        aij_numerator[i][j] += epsilon[t][i][j]; 
                    }
                }
            }
            */
            
            for(int i=0; i<N; i++){
                for(int t=0; t<T-1; t++){
                    aij_denominator[i] += gamma[t][i];
                }
            }


            for(int j=0; j<N; j++){
                for(int t=0; t<T; t++) {
                    bjk_denominator[j] += gamma[t][j]; 
                }   
            }

            for(int j=0; j<N; j++){
                for (int t=0; t<T; t++){
                    bjk_numerator[o_frame[t]][j] += gamma[t][j];
                }
            }
            
        }
        fclose(fp);
        /*******************************/
        /*       Update Parameters     */
        /*******************************/
        // for(int i=0; i<N; i++){	
        //         printf("Alpha : %e\n", alpha[0][i]);
        // }

        // for(int i=0; i<N; i++){	
        //         printf("Gamma : %e\n", gamma[0][i]);
        // }
        
        for(int i=0; i<N; i++){
            new_hmm.initial[i] = pi_numerator[i] / 10000;
        }

        for(int i=0 ;i<N; i++){
            for(int j=0; j<N; j++){
                new_hmm.transition[i][j] = epsilon[i][j] / aij_denominator[i];
            }
        }

        for(int j=0 ;j<N; j++){
            for(int k=0; k<N; k++){
                new_hmm.observation[k][j] = bjk_numerator[k][j] / bjk_denominator[j];
            }
        }

    }

    

    //printf("*** Training done ***\n");

    FILE *fw = open_or_die( dump_filename, "w");
    dumpHMM( fw, &new_hmm );
    fclose(fw);
    //dumpHMM( stderr, &new_hmm );
    
    
	return 0;
}

