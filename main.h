//
// Created by tvand on 2018-10-03.
//

#ifndef AESENCRYPTION_MAIN_H
#define AESENCRYPTION_MAIN_H

#endif //AESENCRYPTION_MAIN_H

unsigned char get_sbox_value(unsigned char val);

unsigned char* generate_roundkey(unsigned char* key, int round);

void enc_rotate_word(unsigned char key[4]);

void enc_sub_words(unsigned char* key);

void extract_word(unsigned char* key, int word_number, unsigned char* result);

void insert_word(unsigned char* insert, int word_number, unsigned char* key);

void exclusive_or(unsigned char* target, unsigned char* against);


