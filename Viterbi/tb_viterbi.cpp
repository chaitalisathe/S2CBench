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

    //sc_signal<sc_uint<8>>* indata_obs = new sc_signal<sc_uint<8>>[N_OBS];
    //Reset routine
    std::ifstream   inFile1(IN_FILE_NAME);

    if (!inFile1) {
        cout << "Could not open " << IN_FILE_NAME << "\n";
        sc_stop();
        exit(-1);
    }
    wait();
  //  while (true) {


        std::ifstream inFile(IN_FILE_NAME);

        std::string data;

        int type = 0;

        in_valid.write(1);
        wait();
        

        while (!in_ready.read())
        {
            wait();
        }



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
                    init_read[i] = std::stod(data);
                    indata_init[i].write(init_read[i]);
                }

                if (type == 3 && i < N_STATES * N_STATES) {
                    transmission_read[i] = std::stod(data);
                    indata_transition[i].write(transmission_read[i]);
                }

                if (type == 4 && i < N_STATES * N_TOKENS) {
                    emission_read[i] = std::stod(data);
                    indata_emission[i].write(emission_read[i]);
                }
                
                i++;
                wait();
                
            }
            
        }
        inFile.close();



        /*       for (i = 0; i < N_OBS; i++) {
                   indata_obs[i].write(obs_read[i]);
               }

               for (i = 0; i < N_STATES; i++) {
                   indata_init[i].write(init_read[i]);
               }

               for (i = 0; i < N_STATES; i++) {
                   for (j = 0; j < N_STATES; j++) {
                       indata_transition[i * N_STATES + j].write(transmission_read[i * N_STATES + j]);
                   }
               }

               for (i = 0; i < N_STATES; i++) {
                   for (j = 0; j < N_TOKENS; j++) {
                       indata_emission[i * N_TOKENS + j].write(emission_read[i * N_TOKENS + j]);
                   }
               }
               wait();*/
               //}
        cout << endl << "Starting comparing results " << endl;

      compare_results();
        sc_stop();

        wait();
 //   }
}






//-----------------------------
// Receive data thread
//----------------------------
void tb_viterbi::recv() {

    // Variables declaration
    sc_uint<8>* viterbi_out_write= new sc_uint<8>[N_OBS];
    int i;

    //std::ifstream out_viterbi_file(OUT_FILE_NAME,"wt");
    out_viterbi_file = fopen(OUT_FILE_NAME, "wt");

    if (!out_viterbi_file) {
        cout << "Could not open " << OUT_FILE_NAME << "\n";
        sc_stop();
        exit(-1);
    }



while (!out_valid.read())
{ 
    wait(); 
}


out_ready.write(1);

    while (true)
    {
        for (i = 0; i < N_OBS; i++) {
            viterbi_out_write[i] = viterbi_output[i].read();

       
            fprintf(out_viterbi_file, "%p",viterbi_out_write);
        }
        
    }

}


//---------------------------------
// Compare results function
//--------------------------------
void tb_viterbi::compare_results() {

    unsigned int outviterbi [N_OBS], outviterbi_golden[N_OBS];
        int line = 1, errors = 0;

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


    std::ifstream   outfile(OUT_FILE_NAME);
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


    std::ifstream  goldfile(OUT_FILE_NAME_GOLDEN);
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


    fclose(out_viterbi_file);
    fclose(diff_file);
    fclose(out_viterbi_golden_file);



}

