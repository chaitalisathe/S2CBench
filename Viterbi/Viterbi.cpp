#include "viterbi.h"

//  Main thread  
void viterbi::viterbi_main() {


    // Variables declaration
    float* llike = new float[N_OBS * N_STATES]; // float
    float	min_p, p; //float
    int	t = 0;
    int	prev, curr;
    int	min_s, s, t2 = N_OBS - 1;
    //unsigned int* obs = new unsigned int[N_OBS]; // observation vector 
    sc_uint<8> in_data;
    float* init = new float[N_STATES];//	init[N_STATES];// initial state probabilities		  
    float* transition = new float[N_STATES * N_STATES];// transition matrix
    float* emission = new float[N_STATES * N_TOKENS];// emission matrix
    //unsigned int* path = new unsigned int[N_OBS];
    unsigned int path;
    int i, j;

    // All probabilities are in -log space. (i.e.: P(x) => -log(P(x)) )
    // Reset state - should be executable in 1 clock cycle
    // reset code

    wait();

    // Main thread	
    while (true) {

        // =================read all inputs

        wait();
        in_data = indata_obs.read();


        /*for (i = 0; i < N_OBS; i++) {
            obs[i] = indata_obs[i].read();
        }*/


        for (i = 0; i < N_STATES; i++) {
            init[i] = indata_init[i].read();
        }

        for (i = 0; i < N_STATES; i++) {
            for (j = 0; j < N_STATES; j++) {
                transition[i * N_STATES + j] = indata_transition[i * N_STATES + j].read();
            }
        }

        for (i = 0; i < N_STATES; i++) {
            for (j = 0; j < N_TOKENS; j++) {
                emission[i * N_TOKENS + j] = indata_emission[i * N_TOKENS + j].read();
            }
        }

        //// shift
        //for (j = N_OBS - 1; j > 0; j--) {
        //    obs[j] = obs[j - 1];
        //}
        //obs[0] = in_data;


        // Initialize with first observation and initial probabilities
        if (t == 0) {

            for (s = 0; s < N_STATES; s++) {
                llike[s] = init[s] + emission[s * N_TOKENS + in_data];
            }
        }

        // Iteratively compute the probabilities over time
        else if (t > 0 && t < N_OBS) {
            for (curr = 0; curr < N_STATES; curr++) {
                // Compute likelihood HMM is in current state and where it came from.
                prev = 0;

                min_p = llike[(t - 1) * N_STATES + prev] +
                    transition[prev * N_STATES + curr] +
                    emission[curr * N_TOKENS + in_data];


                for (prev = 1; prev < N_STATES; prev++) {
                    p = llike[(t - 1) * N_STATES + prev] +
                        transition[prev * N_STATES + curr] +
                        emission[curr * N_TOKENS + in_data];


                    if (p < min_p) {
                        min_p = p;
                    }
                }
                llike[t * N_STATES + curr] = min_p;
            }
        }

        t++;

        if (t >= N_OBS) {

            if (t2 == N_OBS - 1) {

                // Identify end state
                min_s = 0;
                min_p = llike[(N_OBS - 1) * N_STATES + min_s];


                for (s = 1; s < N_STATES; s++) {
                    p = llike[(N_OBS - 1) * N_STATES + s];
                    if (p < min_p) {
                        min_p = p;
                        min_s = s;
                    }

                }
            }

            else
            {

                // Backtrack to recover full path

                min_s = 0;
                min_p = llike[t2 * N_STATES + min_s] + transition[min_s * N_STATES + path];
                for (s = 1; s < N_STATES; s++) {
                    p = llike[t2 * N_STATES + s] + transition[s * N_STATES + path];
                    if (p < min_p) {
                        min_p = p;
                        min_s = s;
                    }
                }



            }
            path = min_s;
            viterbi_output.write(path);
            wait();
            t2--;


            //======= write output to the port viterbi_output.write(path) ; 
     /*       i = 0;
            while (i < N_OBS) {
                viterbi_output.write(path[i]);
                wait();
                i++;
            }

            std::ofstream out_log(OUT_LOG);

            for (i = 0; i < N_OBS; i++) {
                out_log << path[i] << std::endl;
            }*/


            // out_log.close();

           /*  delete[] init;
             delete[] transition;
             delete[] emission;
             delete[] llike;*/
             //delete[] obs;
           //  delete[] path;

             //wait();


        }
    }
}