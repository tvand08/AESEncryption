#include <iostream>
#include <cstring>

#include "constansts.h"
#include "main.h"

int main() {

    std::string message;

    // 128 bit key = 16 bytes per key | 11 generated keys
    unsigned char key[16 * 11];

    //Read message
//    std::cout<< "Enter the key for the cipher" << std::endl;
//    std::cin >> message;

    //Message Length
//    int message_length = message.length();

    //Make new char array for the size of string submitted
//    char message_bytes[message_length+1];

    //Convert string to char array
//    std::strcpy(message_bytes,message.c_str());

    //Read key
    char input[17];
    std::cout << "Enter a key with which you would like to encrypt the message (16 characters)" << std::endl;
    std::cin.getline(input,17);
    for(int i = 0; i < 16; i++){
        key[i] = (unsigned char)input[i];
    }

    for(int i = 0; i< 10; i++){
        generate_roundkey(key, i);
    }

    for(int i=0; i < 11*16; i++){
        if((int)key[i]<16)
            std::cout<<"0";

        std::cout << std::hex << (int)key[i];
    }

}

unsigned char get_sbox_value(unsigned char val){
    int loc = int(val);

    return s_box[loc];
}

unsigned char* generate_roundkey(unsigned char* key, int round){
            unsigned char word[4];
            unsigned char xor_res[4];

            extract_word(key, round*4 + 3, word);

            enc_rotate_word(word);
            enc_sub_words(word);
            extract_word(key, round*4, xor_res);
            exclusive_or(word,xor_res);

            word[0] = word[0] ^ r_con[round];

            insert_word(word, round*4 + 4, key);

        for(int i = 1; i<4; i++){
            unsigned char word_loop[4];
            unsigned char xor_res_loop[4];

            extract_word(key, (round)*4 + i + 3, word_loop);
            extract_word(key, (round)*4 + i, xor_res_loop);

            exclusive_or(word_loop,xor_res_loop);

            insert_word(word_loop, round*4 + i + 4, key);
        }
    return key;
}


void exclusive_or(unsigned char* target, unsigned char* against){
    for(int i = 0; i < 4; i++){
        target[i] = target[i] ^ against[i];
    }
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
