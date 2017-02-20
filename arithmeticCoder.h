#ifndef ARITHMETIC_CODER
#define ARITHMETIC_CODER

#include <stdio.h>
#include <stdlib.h> 
#include "arithmeticCoderTools.h"
#include "defines.h"

class ArithmeticCoder{

private:

	int char_to_index[No_of_chars];
	unsigned char index_to_char[No_of_symbols + 1];
	int cum_freq[No_of_symbols + 1];

	int high = Top_value;
	int low = 0;
	int bits_to_follow = 0;
	ArithmeticCoderTools *arTools;

	code_value value;
	code_value lowDec, highDec;

	FILE *foutDecode;
	void bit_plus_follow(int bit);
	void start_encoding();
	void start_decoding();

public:
	ArithmeticCoder();
	~ArithmeticCoder();
	void initEncoder(char* data, int size, int *freq);
	void initDecoder();
	void encodeSymbol(int symbol);
	void done_encoding();
	int decodeSymbol();
	void update_model(int symbol);
	void endEncode();
	void endDecode();

};

#endif
