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
    int i, m = 0, j = 0;
    unsigned int* obs_read = new unsigned int[N_OBS];
    //unsigned int* obs_read = new unsigned int;
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

    
        // Reading all matrix and inputs
        while (!inFile.eof()) {
            inFile >> data;

            if (data.c_str()[0] == '%') {
                type++;
                i = 0;
            }

            else {
                if (type == 1 && i < N_OBS) {
                    obs_read[i] = std::stoi(data);
                }

                if (type == 2 && i < N_STATES) {
                    //init_read[i] = std::stof(data);
                    indata_init[i].write(std::stof(data));
                }

                if (type == 3 && i < N_STATES * N_STATES) {
                    //transmission_read[i] = std::stof(data);
                    indata_transition[i].write(std::stof(data));
                }

                if (type == 4 && i < N_STATES * N_TOKENS) {
                   // emission_read[i] = std::stof(data);
                    indata_emission[i].write(std::stof(data));
                }

                i++;
            }
        }

        wait();

        while (true) {
            for (m = 0; m < N_OBS; m++) {
                indata_obs.write(obs_read[m]);
                wait();
            }
            inFile.close();

            wait();

            cout << endl << "Starting comparing results " << endl;

            compare_results();
            
            sc_stop();

            wait();
        }

       

        // writing inputs sequentially
    //wait();
    






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
    //unsigned int* viterbi_out_write= new unsigned int[N_OBS];
    unsigned int viterbi_out_write;
    int i = 0;

    //std::ifstream out_viterbi_file(OUT_FILE_NAME,"wt");
    //std::ofstream out_viterbi_file(OUT_FILE_NAME);
    out_viterbi_file = fopen(OUT_FILE_NAME, "wt");

    if (!out_viterbi_file) {
        cout << "Could not open " << OUT_FILE_NAME << "\n";
        sc_stop();
        exit(-1);
    }

    wait();

    while (true)
    {
        wait();
        viterbi_out_write = viterbi_output.read();
        fprintf(out_viterbi_file, "%i\n", viterbi_out_write);
        wait();
    }
}
    



//---------------------------------
// Compare results function
//--------------------------------
void tb_viterbi::compare_results() {

    int outviterbi, outviterbi_golden, line = 1, errors = 0;

    int i = 0;
    while (i < N_OBS) {
        i++;
        wait();
    }

    // Close file where outputs are stored
    fclose(out_viterbi_file);

    // Open results file
    out_viterbi_file = fopen(OUT_FILE_NAME, "rt");

    if (!out_viterbi_file) {
        cout << "Could not open " << OUT_FILE_NAME << endl;
        sc_stop();
        exit(-1);
    }

    //
    //Load the golden output from file
    //
    //out_filter_golden_file.open(OUTFILENAME_GOLDEN);
    out_viterbi_golden_file = fopen(OUT_FILE_NAME_GOLDEN, "rt");
    if (!out_viterbi_golden_file) {
        cout << "Could not open " << OUT_FILE_NAME_GOLDEN << endl;
        sc_stop();
        exit(-1);
    }

    //
    // comparison result with golden output
    //

    diff_file = fopen(DIFF_FILE_NAME, "w");
    if (!diff_file) {
        cout << "Could not open " << DIFF_FILE_NAME << "\n";
        sc_stop();
        exit(-1);
    }

    //      while(out_filter_golden_file.eof()){
    while (fscanf(out_viterbi_golden_file, "%d", &outviterbi_golden) != EOF) {
        fscanf(out_viterbi_file, "%d", &outviterbi);
        cout << endl << "Cycle[" << line << "]: " << outviterbi_golden << "-- " << outviterbi;

        // out_filter_file_read >> outfilter;
        // out_filter_golden_file >> outfilter_golden;
        if (outviterbi != outviterbi_golden) {
            cout << "\nOutput missmatch [line:" << line << "] Golden:" << outviterbi_golden << " -- Output:" << outviterbi;

            fprintf(diff_file, "\nOutput missmatch[line:%d] Golden: %d -- Output: %d", line, outviterbi_golden, outviterbi);
            errors++;
        }
        line++;

    }

    if (errors == 0)
        cout << endl << "Finished simulation SUCCESSFULLY" << endl;
    else
        cout << endl << "Finished simulation " << errors << " MISSMATCHES between Golden and Simulation" << endl;

    fclose(out_viterbi_file);
    fclose(diff_file);
    fclose(out_viterbi_golden_file);
    /*int w = 0;
    while (w < N_OBS)
    {
        w++;
        wait();
    }

    diff_file.close();

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

    for (i = 0; i < N_OBS; i++) {
        if (outviterbi_golden[i] != outviterbi[i]) {
        errors++;
    }
    }*/
    //
    //    if (errors == 0)
    //        cout << endl << "Finished simulation SUCCESSFULLY" << endl;
    //    else
    //        cout << endl << "Finished simulation " << errors << " MISSMATCHES between Golden and Simulation" << endl;
}

