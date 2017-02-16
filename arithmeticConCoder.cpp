#include "arithmeticConCoder.h"

ArithmeticConCoder::ArithmeticConCoder(){}
ArithmeticConCoder::~ArithmeticConCoder(){}

void ArithmeticConCoder::initCum_freq(int *freq){
	int noOfSymbols = No_of_symbols;
	int noOfChars = No_of_chars;

	for (int i = 0; i<noOfChars; i++) {
		char_to_index[i] = i + 1;
		index_to_char[i + 1] = i;
	}
	cum_freq[noOfSymbols] = 0;
	for (int i = noOfSymbols; i > 0; i--){
		cum_freq[i - 1] = cum_freq[i] + freq[i];
	}
	if (cum_freq[0] > Max_frequency){
		printf("cumFreq>Max_freq");
		abort();
	}

}
void ArithmeticConCoder::initEncoder(char* data, int size, int *freq){
	int noOfSymbols = No_of_symbols;
	int noOfChars = No_of_chars;

	arTools = new ArithmeticCoderTools();
	for (int i = 0; i<noOfChars; i++){
	char_to_index[i] = i + 1; 
	index_to_char[i + 1] = i;
	}
	cum_freq[noOfSymbols] = 0;
	for (int i = noOfSymbols; i > 0; i--)
	{												/
		cum_freq[i - 1] = cum_freq[i] + freq[i];
	}
	arTools->start_outputing_bits();
	start_encoding();
	
}

void ArithmeticConCoder::start_encoding()
{   
	low = 0;
    	high = Top_value;
    	bits_to_follow = 0;
}

void ArithmeticConCoder::encodeSymbol(int symbol){   
	long range;
	range = (long)(high - low) + 1;
	high = low +(range*cum_freq[symbol - 1]) / cum_freq[0] - 1;
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


void ArithmeticConCoder::bit_plus_follow(int bit){   
	arTools->output_bit(bit);
	while (bits_to_follow>0) {
		arTools->output_bit(!bit);
		bits_to_follow -= 1;
	}
}
void ArithmeticConCoder::done_encoding(){
	bits_to_follow += 1;
	if (low<First_qtr) bit_plus_follow(0);
	else bit_plus_follow(1);
}

void ArithmeticConCoder::update_model(int symbol)
{
}

void ArithmeticConCoder::initDecoder(char *frstSymb)
{
	*frstSymb = arTools->start_inputing_bits();
	firstSymbol = *frstSymb;
	start_decoding();
}

void ArithmeticConCoder::start_decoding()
{
	int i;
	foutDecode = fopen(DecodeFileName, "wb");
	fwrite(&firstSymbol, 1, 1, foutDecode);
	value = 0;
	for (i = 1; i <= Code_value_bits; i++) {
		value = 2 * value + arTools->input_bit();
		arTools->counter++;
	}
	lowDec = 0;
	highDec = Top_value;
}

int ArithmeticConCoder::decodeSymbol(){   
	long range;									
	int cum;									
	int symbol;									
	range = (long)(highDec - lowDec) + 1;
	cum =(((long)(value - lowDec) + 1)*cum_freq[0] - 1) / range;
	for (symbol = 1; cum_freq[symbol]>cum; symbol++);
	highDec = lowDec +											/
	(range*cum_freq[symbol - 1]) / cum_freq[0] - 1;
	lowDec = lowDec +(range*cum_freq[symbol]) / cum_freq[0];
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
		lowDec = 2 * lowDec;
		highDec = 2 * highDec + 1;
		value = 2 * value + arTools->input_bit();

	}
	char charBuffer = (char)symbol;

	fwrite(&charBuffer, 1, 1, foutDecode);
	return symbol;
}

void ArithmeticConCoder::endEncode()
{
	arTools->closeEncodeFile();
}

void ArithmeticConCoder::endDecode()
{
	fclose(foutDecode);
	arTools->closeDecodeFile();
}
