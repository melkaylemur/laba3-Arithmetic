#include "arithmeticConCoderTools.h"

ArithmeticConCoderTools::ArithmeticConCoderTools(){}
ArithmeticConCoderTools::~ArithmeticConCoderTools(){}

void ArithmeticConCoderTools::start_outputing_bits()
{
	
	fout = fopen(EncodeFileName, "ab");
	fseek(fout, 1, SEEK_SET);
	buffer = 0;
	bits_to_go = 8;
}

void ArithmeticConCoderTools::output_bit(int bit)
{  
	buffer >>= 1; if (bit) buffer |= 0x80;
	bits_to_go -= 1;
	if (bits_to_go == 0) {
		putc(buffer, fout);
		bits_to_go = 8;
	}
}

void ArithmeticConCoderTools::done_outputing_bits()
{
	putc(buffer >> bits_to_go, fout);
}

char ArithmeticConCoderTools::start_inputing_bits()
{
	char ret = 0;
	fin = fopen(EncodeFileName, "rb");
	fread(&ret, 1, 1, fin);
	bits_to_goInput = 0;
	garbage_bits = 0;
	return ret;
}

int ArithmeticConCoderTools::input_bit()
{
	int t;
	counter2++;
	if (bits_to_goInput == 0) {					
		bufferInput = getc(fin);					
		if (bufferInput == EOF) {
			garbage_bits += 1;					
			if (garbage_bits>Code_value_bits - 2) {	
				fprintf(stderr, "Bad input file\n");
				system("pause");
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

void ArithmeticConCoderTools::closeEncodeFile()
{
	fclose(fout);
}

void ArithmeticConCoderTools::closeDecodeFile()
{
	fclose(fin);
}
