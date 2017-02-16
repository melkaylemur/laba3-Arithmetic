#ifndef ARITHMETIC_CON_CODER_TOOLS
#define ARITHMETIC_CON_CODER_TOOLS

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "definesCon.h"
#include <stdlib.h>

class ArithmeticConCoderTools{
private:
	int buffer;
	int bits_to_go;

	int bufferInput;
	int bits_to_goInput;
	int garbage_bits;
	FILE *fout;
	FILE *fin;
public:
	int counter = 0;
	int counter2 = 0;

	ArithmeticConCoderTools();
	~ArithmeticConCoderTools();

	void start_outputing_bits();
	void output_bit(int bit);
	void done_outputing_bits();

	char start_inputing_bits();
	int input_bit();
	
	void closeEncodeFile();
	void closeDecodeFile();

};

#endif
