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

	//sc_in<sc_uint<8>> sanity_check;
	
	//sc_in<sc_uint<8> > * viterbi_output;
	sc_in<sc_uint<8>> viterbi_output;

	// Outputs
	//sc_out<sc_uint<8>>* 	indata_obs; // observation vector  
	sc_out<sc_uint<8>> indata_obs;// observation vector 
	sc_out<float>*	indata_init;// initial state probabilities		  
	sc_out<float> *  indata_transition;// transition matrix
	sc_out<float> *  indata_emission;// emission matrix
	


  //File pointers
	FILE* in_iterbi_file, * out_viterbi_golden_file, * out_viterbi_file_read;
	FILE* out_viterbi_file, * diff_file;

	/* C */
	void compare_results();

	/* R  */
	void recv();

	/* S */
	void send();


	SC_CTOR(tb_viterbi) {

		//viterbi_output = new sc_in<sc_uint<8> >[N_OBS];

		//indata_obs = new sc_out<sc_uint<8>>[N_OBS]; // observation vector  // observation vector 
		indata_init = new sc_out<float>[N_STATES];// initial state probabilities		  
		indata_transition = new sc_out<float>[N_STATES * N_STATES];// transition matrix
		indata_emission = new sc_out<float>[N_STATES * N_TOKENS];// emission matrix

		SC_CTHREAD(send, clk.pos());
		reset_signal_is(rst, false);

		SC_CTHREAD(recv, clk.pos());
		reset_signal_is(rst, false);
	}

	~tb_viterbi() {

		//delete[] indata_obs;
		delete[] indata_init;
		delete[] indata_transition;
		delete[] indata_emission;
		//delete[] viterbi_output;
	}
	
};


#endif  //
