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

    while (true) {

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
        wait();
        inFile.close();

        
        cout << endl << "Starting comparing results " << endl;

        compare_results();



        sc_stop();
        wait();


    }

   

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
    unsigned int* viterbi_out_write= new unsigned int[N_OBS];
    int i;

    //std::ifstream out_viterbi_file(OUT_FILE_NAME,"wt");
    std::ofstream out_viterbi_file(OUT_FILE_NAME);

    if (!out_viterbi_file) {
        cout << "Could not open " << OUT_FILE_NAME << "\n";
        sc_stop();
        exit(-1);
    }
    i = 0;
    wait();
    while (true)
    {
        
        while (i < N_OBS)
        {
            wait();
            viterbi_out_write[i] = viterbi_output[i].read();
           // std::cout << viterbi_out_write[i] << "\t";

            out_viterbi_file << viterbi_out_write[i] << std::endl;
            i++;
        }

        wait();

     

        out_viterbi_file.close();

        delete[] viterbi_out_write;
      
    }
    
}


//---------------------------------
// Compare results function
//--------------------------------
void tb_viterbi::compare_results() {

    //int outviterbi [N_OBS], outviterbi_golden[N_OBS];
    unsigned int* outviterbi = new unsigned int[N_OBS];
    unsigned int* outviterbi_golden = new unsigned int[N_OBS];
    int line = 1, errors = 0;



    // Open results file
    outfile.close();

    outfile.open(OUT_FILE_NAME);

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

    std::ofstream diff_file(DIFF_FILE_NAME);
    if (!diff_file) {
        cout << "Could not open " << DIFF_FILE_NAME << "\n";
        sc_stop();
        exit(-1);
    }

    int w = 0;
    while (w < N_OBS)
    {
        w++;
        wait();
    }

    //diff_file.close();

    std::string data1;

    int i = 0;

    while (!outfile.eof()) {
        outfile >> data1;
        outviterbi[i++] = std::stoi(data1);

    }

    outfile.close();



    std::string data2;

    int type = 0;
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

    cout << "output" << "---" << "Golden output" << std::endl;
    for (i = 0; i < N_OBS; i++)
    {
        cout << outviterbi[i] << "---" << outviterbi_golden[i] << std::endl;
        if (outviterbi_golden[i] != outviterbi[i]) {
            errors++;
            diff_file << outviterbi[i] << "---" << outviterbi_golden[i] << std::endl;
        }

    }
    

    if (errors == 0) {
    cout << endl << "Finished simulation SUCCESSFULLY" << endl;
    
    }
    else
    {
        cout << endl << "Finished simulation " << errors << " MISSMATCHES between Golden and Simulation" << endl;
        
       
    }
    diff_file.close();
}

