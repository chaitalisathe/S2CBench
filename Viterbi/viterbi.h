#pragma once

#include <systemc.h>
#include "define.h"

SC_MODULE(viterbi)
{
public:

    // Inputs
    sc_in_clk clk;
    sc_in<bool> rst;

    sc_in<sc_uint<8>> indata_obs;
    //sc_in<sc_uint<8>>* indata_obs; // observation vector 
    sc_in<float>* indata_init;// initial state probabilities
    sc_in<float>* indata_transition;// transition matrix
    sc_in<float>* indata_emission;// emission matrix

   // sc_out<sc_uint<8>> sanity_check;


   // output  
   // sc_out<sc_uint<8>>* viterbi_output; // most likely state chain
    sc_out<sc_uint<8>> viterbi_output;


   /* F */
    void viterbi_main();


    // Constructor
    SC_CTOR(viterbi) {

        //indata_obs = new sc_in<sc_uint<8>>[N_OBS]; // observation vector 
        indata_init = new sc_in<float>[N_STATES];// initial state probabilities
        indata_transition = new sc_in<float>[N_STATES * N_STATES];// transition matrix
        indata_emission = new sc_in<float>[N_STATES * N_TOKENS];// emission matrix
        //viterbi_output = new sc_out<sc_uint<8>>[N_OBS]; // most likely state chain

        SC_CTHREAD(viterbi_main, clk.pos());
        reset_signal_is(rst, false);
        sensitive << clk.pos();

    }

    // Destructor
    ~viterbi()
    {
       // delete[] indata_obs;
        delete[] indata_init;
        delete[] indata_transition;
        delete[] indata_emission;
       // delete[] viterbi_output;
    }
};

