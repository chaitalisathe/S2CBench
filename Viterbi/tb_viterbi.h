//========================================================================================
// 
// File Name    : tb_viterbi.h
// Description  : Testbench
//=======================================================================================

#ifndef TB_VITERBI_H_
#define TB_VITERBI_H_

#include "systemc.h"
#include "define.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>




SC_MODULE(tb_viterbi) {

	// Inputs
	sc_in<bool>          clk;
	sc_in<bool>          rst;
	sc_out<bool>         in_valid;
	sc_in<bool>         out_valid;
	sc_out<bool>         out_ready;
	sc_in<bool>         in_ready;
	sc_in<sc_uint<8> > viterbi_output[N_OBS];
	

	// Outputs
	sc_out<sc_uint<8>> 	indata_obs[N_OBS]; // observation vector  // observation vector 
	sc_out<float>	indata_init[N_STATES];// initial state probabilities		  
	sc_out<float>   indata_transition[N_STATES * N_STATES];// transition matrix
	sc_out<float>   indata_emission[N_STATES * N_TOKENS];// emission matrix
	


//File pointers
	FILE* in_viterbi_file, * out_viterbi_golden_file, * out_viterbi_file_read;
	FILE* out_viterbi_file, * diff_file;


	/* C */
	void compare_results();

	/* R  */
	void recv();

	/* S */
	void send();


	SC_CTOR(tb_viterbi) {

		SC_CTHREAD(send, clk.pos());
		reset_signal_is(rst, false);

		SC_CTHREAD(recv, clk.pos());
		reset_signal_is(rst, false);
	}

	~tb_viterbi() {}

};


#endif  //
