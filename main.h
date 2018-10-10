//
// Created by tvand on 2018-10-03.
//

#ifndef AESENCRYPTION_MAIN_H
#define AESENCRYPTION_MAIN_H

#endif //AESENCRYPTION_MAIN_H

unsigned char get_sbox_value(unsigned char val);

unsigned char* generate_roundkey(char* key, int round);

unsigned char* d_rotate_word(unsigned char* key,int word_number);

unsigned char* d_sub_words(unsigned char* key, int word_number);

unsigned char* extract_word(unsigned char* key, int column_number);
