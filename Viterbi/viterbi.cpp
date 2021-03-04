#include "viterbi.h"

//  Main thread  
void viterbi::viterbi_main() {


    // Variables declaration
    float* llike = new float[N_OBS * N_STATES]; // float
    float	min_p, p; //float
    int	t;
    int	prev, curr;
    int	min_s, s;
    unsigned int* obs = new unsigned int[N_OBS]; // observation vector 
    float* init = new float[N_STATES];//	init[N_STATES];// initial state probabilities		  
    float* transition = new float[N_STATES * N_STATES];// transition matrix
    float* emission = new float[N_STATES * N_TOKENS];// emission matrix
    unsigned int* path = new unsigned int[N_OBS];
    int i, j;

    // All probabilities are in -log space. (i.e.: P(x) => -log(P(x)) )
    // Reset state - should be executable in 1 clock cycle
    // reset code

    wait();

    // Main thread	
    while (true) {

        // =================read all inputs

        for (i = 0; i < N_OBS; i++) {
            obs[i] = indata_obs[i].read();
        }

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




        // Initialize with first observation and initial probabilities
    L_init: for (s = 0; s < N_STATES; s++) {
        llike[s] = init[s] + emission[s * N_TOKENS + obs[0]];
    }

    // Iteratively compute the probabilities over time
L_timestep: for (t = 1; t < N_OBS; t++) {
L_curr_state: for (curr = 0; curr < N_STATES; curr++) {
    // Compute likelihood HMM is in current state and where it came from.
    prev = 0;

    min_p = llike[(t - 1) * N_STATES + prev] +
        transition[prev * N_STATES + curr] +
        emission[curr * N_TOKENS + obs[t]];


L_prev_state: for (prev = 1; prev < N_STATES; prev++) {
    p = llike[(t - 1) * N_STATES + prev] +
        transition[prev * N_STATES + curr] +
        emission[curr * N_TOKENS + obs[t]];


    if (p < min_p) {
        min_p = p;
    }
}
llike[t * N_STATES + curr] = min_p;
}
}

// Identify end state
min_s = 0;
min_p = llike[(N_OBS - 1) * N_STATES + min_s];
L_end: for (s = 1; s < N_STATES; s++) {
p = llike[(N_OBS - 1) * N_STATES + s];
if (p < min_p) {
    min_p = p;
    min_s = s;
}
}
path[N_OBS - 1] = min_s;

// Backtrack to recover full path
L_backtrack: for (t = N_OBS - 2; t >= 0; t--) {
min_s = 0;
min_p = llike[t * N_STATES + min_s] + transition[min_s * N_STATES + path[t + 1]];
L_state: for (s = 1; s < N_STATES; s++) {
p = llike[t * N_STATES + s] + transition[s * N_STATES + path[t + 1]];
if (p < min_p) {
    min_p = p;
    min_s = s;
}
}
path[t] = min_s;
}




//======= write output to the port viterbi_output.write(path) ; 
i = 0;
while (i < N_OBS) {
    viterbi_output[i].write(path[i]);
    wait();
    i++;
}

std::ofstream out_log(OUT_LOG);

for (i = 0; i < N_OBS; i++) {
    out_log << path[i] << std::endl;
}

out_log.close();

delete[] init;
delete[] transition;
delete[] emission;
delete[] llike;
delete[] obs;
delete[] path;

wait();


    }
}