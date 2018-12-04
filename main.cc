#include <iostream>
#include <cstring>
#include <ctime>
#include <chrono>
#include <mpi.h>
#include <fstream> 

#include "constansts.h"
#include "main.h"

int main(int argc, char* argv[]) {
    // 128 bit key = 16 bytes per key | 11 generated keys
    unsigned char key[16 * 11];
    unsigned char result[2560000] = {0};
    char key_input[17];
    char msg_input[2560001] ={'='};
    int        comm_sz;               /* Number of processes    */
    int        my_rank;               /* My process rank        */

    MPI_Init(NULL,NULL);

    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);


    if(my_rank == 0){
        // Read the message into the message input
        std::ifstream in("input.txt");
        std::string contents((std::istreambuf_iterator<char>(in)), 
            std::istreambuf_iterator<char>());
        strcpy(msg_input,contents.c_str());

        for(int i = 1; i< comm_sz; i++){
            MPI_Send(msg_input,256001,MPI_CHAR,i,0,MPI_COMM_WORLD);
        }
    }else{
        MPI_Recv(msg_input,256001,MPI_CHAR,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }
    // Read key into key input
    std::ifstream inkey("key.txt");
    std::string keystring((std::istreambuf_iterator<char>(inkey)), 
        std::istreambuf_iterator<char>());
    strcpy(key_input,keystring.c_str());

     // Move the key text into the first section of the key
    for(int i = 0; i < 16; i++){
        key[i] = (unsigned char)key_input[i];
    }

    //typedef std::chrono::high_resolution_clock Clock;
    //auto t1 = Clock::now();

    int message_length = 0;
    while(msg_input[message_length] != '\000'){ message_length++; }

    int num_blocks = (message_length / 16) + 1;
    // Generate the round keys from the cipher key
     //Begin encryption
    for(int i = 0; i< 10; i++){
        generate_roundkey(key,i);
    }
    
    int rounds_per_process = num_blocks / comm_sz;
    int remaining = num_blocks % comm_sz;
    unsigned char process_result[(rounds_per_process*16) + 1];
    unsigned char text[16] = {0};

    int start;
    if(my_rank < remaining){
        rounds_per_process++;
        start = my_rank * rounds_per_process;
    }else{
        start = (my_rank * rounds_per_process) + remaining;
    }
    for(int j = 0; j < rounds_per_process ; j++){
        //std::cout<<"Process #"<<my_rank<<" started round #"<<j<<" from "<<start<<" to "<<start+rounds_per_process<<std::endl;

        // Move the current message into the unsigned char array
        for(int i = (j*16); i < (j+1)*16; i++){
            text[i%16] = (unsigned char)msg_input[start + i];
        }
        // Add cipher key to cipher text
        exclusive_or(text , &key[0], 16);
        /* Perform the 9 rounds of encryption
         * For each round algorithm will
         * 1. Sub out the bytes using the Rijndael s-box
         * 2. Shift the rows of the cipher text
         * 3. Mix up the columns using a fixed matrix
         *    and galois field multiplication
         * 4. Add the round key for the given round to
         *    the cipher text
         */
        for(int i  = 1; i < 10; i++ ){
           enc_sub_bytes(text);
           enc_shift_rows(text);
           enc_mix_columns(text);
           exclusive_or(text, &key[i*16],16);
        }

        // Sub the bytes with the s-box
        enc_sub_bytes(text);

        // Shift the rows once more
        enc_shift_rows(text);

        // Add the final round key to the cipher text
        exclusive_or(text, &key[10*16], 16);
        process_result[rounds_per_process*16] = (unsigned char)my_rank;


        for(int i = (j*16); i < (j+1)*16; i++){
            process_result[i] = text[i%16];
        }
        
    }

    if(my_rank != 0){
        MPI_Send(process_result,(rounds_per_process*16) + 1,MPI_UNSIGNED_CHAR,0,1,MPI_COMM_WORLD);
        
    }else{
        for(int i = 0; i < rounds_per_process*16; i++){
            result[i] = process_result[i];
        }
        for(int q = 1; q < comm_sz; q++){
            unsigned char rcv[(rounds_per_process*16)*2] = {0};
            char       greeting[42];
            
            MPI_Recv(rcv, (rounds_per_process*16)*2,MPI_UNSIGNED_CHAR,q,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            int start;
            if(q < remaining){
                start = my_rank * rounds_per_process;
            }else{
                start = (my_rank * (rounds_per_process - 1)) + remaining;
            }
        }
    
        print_hex(result,num_blocks*16);
    }

    MPI_Finalize(); 

   // auto t2 = Clock::now();
    //std::cout<< std::chrono::duration_cast<std::chrono::nanoseconds>(t2-t1).count()<<" Nanoseconds"<<std::endl;

    // Print out the finalized cipher text
}

// Shared methods

// Takes in a array of characters and prints out the
// values of the characters in hex up to the number provided
void print_hex(unsigned char* output, int byte_count){
    for(int i = 0; i < byte_count; i++){
        if(output[i]<16){
            std::cout<<0;
        }
        std::cout<<std::hex<<int(output[i]);
    }
    std::cout<<std::endl;
}

// Returns the byte value of the rijndael s-box at the
// location provided
unsigned char get_sbox_value(unsigned char val){
    int loc = int(val);
    return s_box[loc];
}

// Executes an exclusive or between the two char arrays provided
// up to the number provided
void exclusive_or(unsigned char* target, unsigned char* against, int num_bytes){
    for(int i = 0; i < num_bytes; i++){
        target[i] = target[i] ^ against[i];
    }

}

// AES methods

// Sub the bytes of the characters provided with
// the corresponding values in the s-box
void enc_sub_bytes(unsigned char* msg){
    for(int i = 0 ; i< 16; i++){
        msg[i] = get_sbox_value(msg[i]);
    }
}

// Shift the rows of the text provided
// Shits each row left row_num-1 times
void enc_shift_rows(unsigned char* msg){
    for(int i = 1; i< 4; i++){
        for(int j =0; j < i; j++){
            int offset = i*4;
            unsigned char temp = msg[0+i];
            msg[0+i] = msg[4+i];
            msg[4+i] = msg[8+i];
            msg[8+i] = msg[12+i];
            msg[12+i] = temp;
        }
    }
}

// Mix the columns of the cipher text based on the
// galois field multiplication with a fixed matrix
void enc_mix_columns(unsigned char* msg){
    unsigned char result[16];

    for(int i = 0; i < 4; i++){
        for(int j= 0; j < 4; j++){
            int i_offset = i*4;
            int j_offset = j*4;

            unsigned char accumulate;

            accumulate = galois_multiply(column_matrix[i_offset], msg[j_offset]);

            for (int x = 1; x < 4; x++) {
                unsigned char temp;
                temp = galois_multiply(column_matrix[i_offset + x], msg[j_offset + x]);
                accumulate ^= temp;
            }
            result[i + j*4] = accumulate;
        }
    }

    for(int i = 0; i < 16; i++){
        msg[i] = result[i];
    }
}

unsigned char galois_multiply(unsigned char a, unsigned char b){
    unsigned char result = 0x00;

    for(int i = 0; i < 8; i++){
        if((b & 1) != 0 ){
            result ^= a;
        }

        bool hi_bit_set = (a & 0x80) != 0;
        a <<= 1;
        if (hi_bit_set ){
            a ^= 0x1B;
        }

        b >>= 1;
    }
    return result;
}

// Key generation methods
unsigned char* generate_roundkey(unsigned char* key, int round){
            unsigned char word[4];
            unsigned char xor_res[4];

            extract_word(key, round*4 + 3, word);

            enc_rotate_word(word);
            enc_sub_words(word);
            extract_word(key, round*4, xor_res);
            exclusive_or(word,xor_res,4);

            word[0] = word[0] ^ r_con[round];

            insert_word(word, round*4 + 4, key);

        for(int i = 1; i<4; i++){
            unsigned char word_loop[4];
            unsigned char xor_res_loop[4];

            extract_word(key, (round)*4 + i + 3, word_loop);
            extract_word(key, (round)*4 + i, xor_res_loop);

            exclusive_or(word_loop,xor_res_loop,4);

            insert_word(word_loop, round*4 + i + 4, key);
        }
    return key;
}

void insert_word(unsigned char* insert, int word_number, unsigned char* key){
    int offset = word_number*4;

    for(int i = 0; i< 4; i++){
        key[i + offset] = insert[i];
    }
}

void extract_word(unsigned char* key, int word_number, unsigned char* result){
    int offset = word_number*4;

    for(int i = 0; i< 4; i++){
        result[i] = key[i + offset];
    }
}

void enc_rotate_word(unsigned char* key){
    unsigned char temp = key[0];
    key[0] = key[1];
    key[1] = key[2];
    key[2] = key[3];
    key[3] = temp;
}

void enc_sub_words(unsigned char* key){
    for(int i = 0 ; i < 4; i++){
        key[i] = get_sbox_value(key[i]);
    }
}
