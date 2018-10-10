#include <iostream>
#include <cstring>

#include "constansts.h"
#include "main.h"

int main() {

    std::string message;
    char key[32*11];

    //Read message
    std::cout<< "Enter the message you would like to encrypt:" << std::endl;
//    std::cin >> message;

    //Message Length
    int message_length = message.length();

    //Make new char array for the size of string submitted
    char message_bytes[message_length+1];

    //Convert string to char array
    std::strcpy(message_bytes,message.c_str());

    //Read key
    std::cout << "Enter a key with which you would like to encrypt the message" << std::endl;
//    std::cin>>key;

    unsigned char test[] = "abcdefghijklmnopqrstuvwxyz123456";
//    unsigned char* result = d_rotate_word(test,0);
//    unsigned char* result2 = d_sub_words(test,0);
    for(int i = 0 ; i< 7; i++){
        unsigned char* result3 = extract_word(test, i);
        std::cout<<std::endl;
    }

//    std::cout<<result3<<std::endl;

}

unsigned char get_sbox_value(unsigned char val){
    int loc = int(val);

    return s_box[loc];
}

// Will need a total of 11 for 128 bit encryption
unsigned char* generate_roundkey(char* key, int round){
    for(int round = 0; round < 11; round++){

    }
}

unsigned char* extract_word(unsigned char* key, int column_number){
    unsigned char result[4];
    int offset = (column_number / 4)*16 + column_number%4;
    std::cout<<offset<<std::endl;
    for(int i = 0; i< 4; i++){
        result[i] = key[(4*i) + offset];
        std::cout<<i<< " is "<<result[i]<<std::endl;
    }
}

unsigned char* d_rotate_word(unsigned char* key){
    unsigned char temp = key[0];
    key[0] = key[1];
    key[1] = key[2];
    key[2] = key[3];
    key[3] = temp;

    return key;
}

unsigned char* d_sub_words(unsigned char* key, int word_number){
    for(int i = 0 ; i < 4; i++){
        key[i] = get_sbox_value(key[i]);
    }
}
