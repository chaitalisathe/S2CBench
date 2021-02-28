//========================================================================================
// 
// File Name    : viterbi.h
// Description  : viterbi module declaration
//=============================================================

#ifndef VITERBI_H_
#define VITERBI_H_

#include "systemc.h"
#include "define.h"


SC_MODULE(viterbi) {


public:

    // Inputs
    sc_in_clk clk;
    sc_in<bool> rst;
    sc_in<bool>         in_valid;
    sc_out<bool>         out_valid;
    sc_in<bool>         out_ready;
    sc_out<bool>         in_ready;


    sc_in<sc_uint<8>> indata_obs[N_OBS]; // observation vector 
    sc_in<float> indata_init [N_STATES];// initial state probabilities
    sc_in<float>   indata_transition[N_STATES * N_STATES];// transition matrix
    sc_in<float> indata_emission[N_STATES * N_TOKENS];// emission matrix
    

   // output  
    sc_out<sc_uint<8>>  viterbi_output[N_OBS]; // most likely state chain
  


   /* F */
    void viterbi_main(void);


    // Constructor
    SC_CTOR(viterbi) {

        SC_CTHREAD(viterbi_main, clk.pos());
        reset_signal_is(rst, false);
        sensitive << clk.pos();

    }

    // Destructor
    ~viterbi() {}


};


#endif   //  VITERBI_H_

