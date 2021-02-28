//========================================================================================
// 
// File Name    : tb_viterbi.cpp
// Description  : Testbench
//=======================================================================================
#include "tb_viterbi.h"
#pragma warning(disable : 4996)

//--------------------------
// Send data thread
//--------------------------
void tb_viterbi::send() {



    // Variables declaration
    int i, j = 0;
    unsigned int* obs_read = new unsigned int[N_OBS];
    float* init_read = new float[N_STATES];
    float* transmission_read = new float[N_STATES * N_STATES];
    float* emission_read = new float[N_STATES * N_TOKENS];

    //Reset routine
    std::ifstream   inFile(IN_FILE_NAME);

    if (!inFile) {
        cout << "Could not open " << IN_FILE_NAME << "\n";
        sc_stop();
        exit(-1);
    }
        std::string data;

        int type = 0;


        while (!inFile.eof()) {
            inFile >> data;

            if (data.c_str()[0] == '%') {
                type++;
                i = 0;
            }

            else {
                if (type == 1 && i < N_OBS) {
                    obs_read[i] = std::stoi(data);
            
                    indata_obs[i].write(obs_read[i]);
                }

                if (type == 2 && i < N_STATES) {
                    init_read[i] = std::stof(data);
                    indata_init[i].write(init_read[i]);
                }

                if (type == 3 && i < N_STATES * N_STATES) {
                    transmission_read[i] = std::stof(data);
                    indata_transition[i].write(transmission_read[i]);
                }

                if (type == 4 && i < N_STATES * N_TOKENS) {
                    emission_read[i] = std::stof(data);
                    indata_emission[i].write(emission_read[i]);
                }
                
                i++;
                
            }
            
        }

        inFile.close();

        wait();

        cout << endl << "Starting comparing results " << endl;

      compare_results();
        sc_stop();

        wait();

        delete[] obs_read;
        delete[] init_read;
        delete[] transmission_read;
        delete[] emission_read;
}






//-----------------------------
// Receive data thread
//----------------------------
void tb_viterbi::recv() {

    // Variables declaration
    sc_uint<8>* viterbi_out_write= new sc_uint<8>[N_OBS];
    int i;

    //std::ifstream out_viterbi_file(OUT_FILE_NAME,"wt");
    std::ofstream out_viterbi_file(OUT_FILE_NAME);

    if (!out_viterbi_file) {
        cout << "Could not open " << OUT_FILE_NAME << "\n";
        sc_stop();
        exit(-1);
    }

    wait();

    while (true)
    {
        for (i = 0; i < N_OBS; i++) {
            viterbi_out_write[i] = viterbi_output[i].read();
        }

        for (i = 0; i < N_OBS; i++) {
            out_viterbi_file << viterbi_out_write[i] << std::endl;
        }

        wait();
    }

    out_viterbi_file.close();

}


//---------------------------------
// Compare results function
//--------------------------------
void tb_viterbi::compare_results() {

    unsigned int outviterbi [N_OBS], outviterbi_golden[N_OBS];
        int line = 1, errors = 0;

    // Open results file
        std::ifstream   outfile(OUT_FILE_NAME);

    if (!outfile) {
        cout << "Could not open " << OUT_FILE_NAME << endl;
        sc_stop();
        exit(-1);
    }

    //
    //Load the golden output from file
    //
    std::ifstream  goldfile(OUT_FILE_NAME_GOLDEN);
    if (!goldfile) {
        cout << "Could not open " << OUT_FILE_NAME_GOLDEN << endl;
        sc_stop();
        exit(-1);
    }

    //
    // comparison result with golden output
    //

    std::ofstream diff_file (DIFF_FILE_NAME);
    if(!diff_file) {
        cout << "Could not open " << DIFF_FILE_NAME << "\n";
        sc_stop();
        exit(-1);
    }

    //while (fscanf(out_viterbi_golden_file, "%i", &outviterbi_golden) != EOF) {
    //    fscanf(out_viterbi_file, "%i", &outviterbi);


    //   // cout << endl << "Cycle[" << line << "]: " << outviterbi_golden << "-- " << outviterbi;


    //    if (outviterbi != outviterbi_golden) {
    //        cout << "\nOutput missmatch [line:" << line << "] Golden:" << outviterbi_golden << " -- Output:" << outviterbi;

    //        fprintf(diff_file, "\nOutput missmatch[line:%i] Golden: %i -- Output: %i", line, outviterbi_golden, outviterbi);

    //        errors++;
    //    }

    //    line++;

    //}

    diff_file.close();

    std::string data1;

    int type = 0, i = 0;

    while (!outfile.eof()) {
        outfile >> data1;

        if (data1.c_str()[0] == '%') {
            type++;
            i = 0;
        }

        else {
            if (type == 1 && i < N_OBS) {
                outviterbi[i++] = std::stoi(data1);
            }
        }
    }
    outfile.close();


    
    std::string data2;

    type = 0;
    i = 0;

    while (!goldfile.eof()) {
        goldfile >> data2;

        if (data2.c_str()[0] == '%') {
            type++;
            i = 0;
        }

        else {
            if (type == 1 && i < N_OBS) {
                outviterbi_golden[i++] = std::stoi(data2);
            }
        }
    }
    goldfile.close();

    for (i = 0; i < N_OBS; i++) {
        if (outviterbi_golden != outviterbi) {
        errors++;
    }
    }

    if (errors == 0)
        cout << endl << "Finished simulation SUCCESSFULLY" << endl;
    else
        cout << endl << "Finished simulation " << errors << " MISSMATCHES between Golden and Simulation" << endl;
}

