//===========================================================================
//
// File Name    : main.cpp
// Description  : viterbi top system description
//===========================================================================
#include <systemc.h>
#include "viterbi.h"
#include "tb_viterbi.h"
#include "define.h"


int sc_main(int argc, char* argv[])
{

    std::cout << "Hello World" << std::endl;
  sc_clock                clk("clk", 25, SC_NS, 0.5, 12.5, SC_NS, true);
  sc_signal<bool>         rst;
  sc_signal<bool>         in_valid;
  sc_signal<bool>         out_valid;
  sc_signal<bool>         out_ready;
  sc_signal<bool>         in_ready;
  // Inputs to viterbi algorithm
  sc_signal<sc_uint<8>>* indata_obs = new sc_signal<sc_uint<8>>[N_OBS]; // observation vector 
  sc_signal<float>* indata_init = new sc_signal<float>[N_STATES];
  sc_signal<float>* indata_transition = new sc_signal<float>[N_STATES * N_STATES];// transition matrix
  sc_signal<float>*		indata_emission = new sc_signal<float>[N_STATES*N_TOKENS];// emission matrix
	
  // output  
  sc_signal<sc_uint<8>>*  viterbi_output = new sc_signal<sc_uint<8>>[N_OBS]; // most likely state chain
  int i, j;

  viterbi u_viterbi("viterbi");
  tb_viterbi test("tb_viterbi");

  //connect to  module
  u_viterbi.clk(clk);
  u_viterbi.rst(rst);
  u_viterbi.in_valid(in_valid);
      u_viterbi.out_valid(out_valid);
  u_viterbi.out_ready(out_ready);
  u_viterbi.in_ready(in_ready);

  for (i = 0; i < N_OBS; i++) {
      u_viterbi.indata_obs[i](indata_obs[i]);
  }

  for (i = 0; i < N_STATES; i++) {
      u_viterbi.indata_init[i](indata_init[i]);
  }


  for (i = 0; i < N_STATES; i++) {
      for (j = 0; j < N_STATES; j++) {
          u_viterbi.indata_transition[i* N_STATES + j](indata_transition[i * N_STATES + j]);
      }
  }

  for (i = 0; i < N_STATES; i++) {
      for (j = 0; j < N_TOKENS; j++) {
          u_viterbi.indata_emission[i * N_TOKENS+ j](indata_emission[i * N_TOKENS+ j]);
      }
  }

  for (i = 0; i < N_OBS; i++) {
      u_viterbi.viterbi_output[i](viterbi_output[i]);
  }

  // connect to test bench
  test.clk( clk );
  test.rst( rst );

  test.in_valid(in_valid);
  test.out_valid(out_valid);
  test.out_ready(out_ready);
  test.in_ready(in_ready);
    
      for (i = 0; i < N_OBS; i++) {
          test.indata_obs[i](indata_obs[i]);
      }

  for (i = 0; i < N_STATES; i++) {
      test.indata_init[i](indata_init[i]);
  }


  for (i = 0; i < N_STATES; i++) {
      for (j = 0; j < N_STATES; j++) {
          test.indata_transition[i * N_STATES + j](indata_transition[i * N_STATES + j]);
      }
  }

  for (i = 0; i < N_STATES; i++) {
      for (j = 0; j < N_TOKENS; j++) {
          test.indata_emission[i * N_TOKENS+ j](indata_emission[i * N_TOKENS+ j]);
      }
  }

      for (i = 0; i < N_OBS; i++) {
          test.viterbi_output[i](viterbi_output[i]);
      }




#ifdef WAVE_DUMP
  // Trace files
  sc_trace_file* trace_file = sc_create_vcd_trace_file("trace_behav");

  // Top level signals
  sc_trace(trace_file, clk              , "clk");
  sc_trace(trace_file, rst              , "rst"); 
  //sc_trace(trace_file, in_data          , "in_data");
  sc_trace(trace_file, filter_output    , "viterbi_output");

#endif  // End WAVE_DUMP

  sc_start( 25, SC_NS );
  rst.write(0);

  sc_start( 25, SC_NS );
  rst.write(1);

  sc_start();

#ifdef WAVE_DUMP
   sc_close_vcd_trace_file(trace_file);
#endif

   delete[] indata_emission;
   delete[] indata_transition;
   delete[] indata_init;
   delete[] indata_obs;
   delete[] viterbi_output;

    return 0;

};
