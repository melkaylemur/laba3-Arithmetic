#include "arithmeticCoder.h"

ArithmeticCoder::ArithmeticCoder(){}
ArithmeticCoder::~ArithmeticCoder(){}

void ArithmeticCoder::initEncoder(char* data, int size, int *freq){
	int noOfSymbols = No_of_symbols;
	int noOfChars = No_of_chars;

	arTools = new ArithmeticCoderTools();
	int i;
	for (i = 0; i<noOfChars; i++) {
	char_to_index[i] = i + 1;
	index_to_char[i + 1] = i;
	}
	cum_freq[noOfSymbols] = 0;
	for (i = noOfSymbols; i > 0; i--){
		cum_freq[i - 1] = cum_freq[i] + freq[i];
	}
	if (cum_freq[0] > Max_frequency){
		printf("cumFreq>Max_freq");
		abort();
	}
	arTools->start_outputing_bits();
	start_encoding();
	
}

void ArithmeticCoder::start_encoding(){   
	low = 0;
	high = Top_value;
	bits_to_follow = 0;
}

void ArithmeticCoder::encodeSymbol(int symbol){   
	long range;
	range = (long)(high - low) + 1;
	high = low + (range*cum_freq[symbol - 1]) / cum_freq[0] - 1;
	low = low + (range*cum_freq[symbol]) / cum_freq[0];
	for (;;) {
		if (high<Half) {
			bit_plus_follow(0);
		}
		else if (low >= Half) {
			bit_plus_follow(1);
			low -= Half;
			high -= Half;
		}
		else if (low >= First_qtr && high<Third_qtr) {
			bits_to_follow += 1;
			low -= First_qtr;
			high -= First_qtr;
		}
		else break;
		low = 2 * low;
		high = 2 * high + 1;
	}
}


void ArithmeticCoder::bit_plus_follow(int bit){   
	arTools->output_bit(bit);
	while (bits_to_follow>0) {
		arTools->output_bit(!bit);
		bits_to_follow -= 1;
	}
}
void ArithmeticCoder::done_encoding(){
	bits_to_follow += 1;
	if (low<First_qtr) bit_plus_follow(0);
	else bit_plus_follow(1);
}

void ArithmeticCoder::update_model(int symbol){
}

void ArithmeticCoder::initDecoder(){
	arTools->start_inputing_bits();
	start_decoding();
}

void ArithmeticCoder::start_decoding(){
	int i;
	foutDecode = fopen(DecodeFileName, "wb");
	value = 0;
	for (i = 1; i <= Code_value_bits; i++) {
		value = 2 * value + arTools->input_bit();
		arTools->counter++;
	}
	lowDec = 0;
	highDec = Top_value;
}

int ArithmeticCoder::decodeSymbol() {   
	long range;
	int cum;
	int symbol;
	range = (long)(highDec - lowDec) + 1;
	cum = (((long)(value - lowDec) + 1)*cum_freq[0] - 1) / range;
	for (symbol = 1; cum_freq[symbol]>cum; symbol++);
	highDec = lowDec + (range*cum_freq[symbol - 1]) / cum_freq[0] - 1;
	lowDec = lowDec + (range*cum_freq[symbol]) / cum_freq[0];
	for (;;) {
		if (highDec<Half) {
		}
		else if (lowDec >= Half) {
			value -= Half;
			lowDec -= Half;
			highDec -= Half;
		}
		else if (lowDec >= First_qtr && highDec<Third_qtr) {
			value -= First_qtr;
			lowDec -= First_qtr;
			highDec -= First_qtr;
		}
		else break;
		highDec = 2 * highDec + 1;
		value = 2 * value + arTools->input_bit();
	}
	char charBuffer = (char)symbol;
	fwrite(&charBuffer, 1, 1, foutDecode);
	return symbol;
}

void ArithmeticCoder::endEncode()
{
	arTools->closeEncodeFile();
}

void ArithmeticCoder::endDecode()
{
	fclose(foutDecode);
	arTools->closeDecodeFile();
}
