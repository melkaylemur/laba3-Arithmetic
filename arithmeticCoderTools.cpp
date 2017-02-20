#include "arithmeticCoderTools.h"

ArithmeticCoderTools::ArithmeticCoderTools(){}
ArithmeticCoderTools::~ArithmeticCoderTools(){}

void ArithmeticCoderTools::start_outputing_bits(){
	
	fout = fopen(EncodeFileName, "wb");
	buffer = 0;
	bits_to_go = 8;
}

void ArithmeticCoderTools::output_bit(int bit){  
	buffer >>= 1; if (bit) buffer |= 0x80;
	bits_to_go -= 1;
	if (bits_to_go == 0) {
		putc(buffer, fout);
		bits_to_go = 8;
	}
}

void ArithmeticCoderTools::done_outputing_bits(){
	putc(buffer >> bits_to_go, fout);

}

void ArithmeticCoderTools::start_inputing_bits(){
	fin = fopen(EncodeFileName, "rb");
	bits_to_goInput = 0;
	garbage_bits = 0;
}

int ArithmeticCoderTools::input_bit(){
	int t;
	counter2++;
	if (bits_to_goInput == 0) {
		bufferInput = getc(fin);
		if (bufferInput == EOF) {
			garbage_bits += 1;
			if (garbage_bits>Code_value_bits - 2) {
				fprintf(stderr, "Bad input file\n");
				exit(-1);
			}
		}
		bits_to_goInput = 8;
	}
	t = bufferInput & 1;
	bufferInput >>= 1;
	bits_to_goInput -= 1;
	return t;
}

void ArithmeticCoderTools::closeEncodeFile(){
	fclose(fout);
}

void ArithmeticCoderTools::closeDecodeFile(){
	fclose(fin);
}