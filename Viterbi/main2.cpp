#define SC_INCLUDE_FX
#include "systemc.h"
#include "define.h"

int sc_main(int argc, char** argv)
{
	std::cout << "Hello World \n";

	sc_clock                clk("clk", 25, SC_NS, 0.5, 12.5, SC_NS, true);
	sc_signal<bool>         rst;
	//// Inputs to viterbi algorithm
	sc_signal<sc_uint<8>> 	indata_obs[N_OBS]; // observation vector 
	sc_signal<float>			indata_init[N_STATES];
	//sc_signal<sc_fixed<18, 15>>			indata_transition[N_STATES * N_STATES];// transition matrix
	//sc_signal<sc_fixed<18, 15>>		indata_emission[N_STATES * N_TOKENS];// emission matrix

	return 0;
}