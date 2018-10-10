#include <iostream>
#include <cstring>

#include "main.h"





int main() {

    std::string message;
    char key[32];

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

    unsigned char test[] = "sand";
    unsigned char* result = d_rotate_word(test,0);
    unsigned char* result2 = d_sub_words(test,0);

}

unsigned char get_sbox_value(unsigned char val){
    int loc = int(val);

    return s_box[loc];
}

// Will need a total of 11 for 128 bit encryption
unsigned char* generate_roundkey(char* key, int round){

}

unsigned char* d_rotate_word(unsigned char* key,int word_number){
    char temp = key[4*word_number];
    key[4*word_number] = key[4*word_number + 1];
    key[4*word_number+1] = key[4*word_number + 2];
    key[4*word_number+2] = key[4*word_number + 3];
    key[4*word_number+3] = temp;

    return key;
}

unsigned char* d_sub_words(unsigned char* key, int word_number){
    for(int i = 0 ; i < 4; i++){
        key[4*word_number + i] = get_sbox_value(key[4*word_number + i]);
    }
}
