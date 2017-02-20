#ifndef ARITHMETIC_CODER_TOOLS
#define ARITHMETIC_CODER_TOOLS

#include <stdio.h>
#include "defines.h"
#include <stdlib.h>

class ArithmeticCoderTools{
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

	ArithmeticCoderTools();
	~ArithmeticCoderTools();

	void start_outputing_bits();
	void output_bit(int bit);
	void done_outputing_bits();

	void start_inputing_bits();
	int input_bit();
	
	void closeEncodeFile();
	void closeDecodeFile();

};

#endif
