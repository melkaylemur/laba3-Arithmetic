#ifndef ARITHMETIC_CON_CODER
#define ARITHMETIC_CON_CODER

#include <stdio.h>
#include <stdlib.h> 
#include "arithmeticConCoderTools.h"
#include "defines.h"

class ArithmeticConCoder{

private:
int char_to_index[No_of_chars];	
unsigned char index_to_char[No_of_symbols + 1];
int cum_freq[No_of_symbols + 1];
int high = Top_value;
int low = 0;
int bits_to_follow = 0;
ArithmeticConCoderTools *arTools;

code_value value;
code_value lowDec, highDec;

FILE *foutDecode;
void bit_plus_follow(int bit);
void start_encoding();
void start_decoding();
char firstSymbol = 0;
public:
	ArithmeticConCoder();
	~ArithmeticConCoder();
	void initEncoder(char* data, int size, int *freq);
	void initDecoder(char *frstSymb);
	void encodeSymbol(int symbol);
	void done_encoding();
	int decodeSymbol();
	void initCum_freq(int *freq);
	void update_model(int symbol);
	void endEncode();
	void endDecode();

};

#endif
