#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "arithmeticCoder.h"
#include "arithmeticCoderTools.h"
#include "defines.h"
#include "arithmeticConCoder.h"
#include "arithmeticConCoderTools.h"
#include "definesCon.h"

using namespace std;

int main(){
//Arithmetic
	ArithmeticCoder *arCoder = new ArithmeticCoder();
	int alphabetSize = No_of_chars;
	FILE *f;
	f = fopen("stream.txt", "rb");
	fseek(f, 0L, SEEK_END);
	int size = ftell(f);
	fseek(f, 0L, SEEK_END);
	rewind(f);

	char *inputData = new char[size];
	int *freq = new int[alphabetSize + 2];

	for (int i = 0; i < alphabetSize + 2; i++)
	{
		freq[i] = 0;
	}
	freq[EOF_symbol] = 1;
	fread(inputData, size, 1, f);

	for (int i = 0; i < size; i++)
	{
		int ggg = inputData[i] - '0';
		freq[ggg]++;
	}
	fstream pr;
	pr.open("Pr.txt");
	for (int i = 0; i < 5; i++)
	{
		pr << (double)freq[i]/size << " ";
		std::cout << (double)freq[i]/size << std::endl;
	}
	pr.close();
	arCoder->initEncoder(inputData, size, freq);
	for (int i = 0; i < size; i++)
	{
		cout << i << endl;
		arCoder->encodeSymbol(inputData[i]);
	}
	arCoder->encodeSymbol(EOF_symbol);

	arCoder->endEncode();
	fclose(f);

	arCoder->initDecoder();
	int symbol;
	for (int i = 0; i < size; i++)
	{
		symbol = arCoder->decodeSymbol();
		if (symbol == EOF_symbol) break;
	}
	arCoder->endDecode();

//Context Arithmetic
	ArithmeticConCoder *arConCoder = new ArithmeticConCoder();

	int **freq = new int*[alphabetSize + 2];
	for (int i = 0; i < alphabetSize + 2; i++){
		freq[i] = new int[alphabetSize + 2];
		for (int i2 = 0; i2 < alphabetSize + 2; i2++)
		{
			freq[i][i2] = 0;
		}

	}

	for (int i = 0; i < alphabetSize + 2; i++)
	{
		freq[i][EOF_symbol] = 1;
	}

	for (int i = 0; i < size; i++)
	{
		int y = (int)inputData[i];
		int x = (int)inputData[i + 1];
		freq[y][x]++;
	}

	fstream pr;
	pr.open("PrCon.txt");
	for (int i = 0; i < alphabetSize; i++)
	{
		for (int i2 = 0; i2 < alphabetSize; i2++)
		{
			pr << freq[i][i2] << " ";
		}
		pr << endl;
	}
	pr.close();

	FILE *f2;
	f2 = fopen("stream_encode_2", "wb");
	fwrite(&inputData[0], 1, 1, f2);
	fclose(f2);

	arConCoder->initEncoder(inputData, size, freq[inputData[0]]);
	for (int i = 1; i < size; i++)
	{
		cout << i << endl;
		arCoder->encodeSymbol(inputData[i]);
		arCoder->initCum_freq(freq[inputData[i]]);
	}
	arConCoder->encodeSymbol(EOF_symbol);

	arConCoder->endEncode();
	fclose(f);

	char frstSymb = 0;
	arConCoder->initDecoder(&frstSymb);
	arConCoder->initCum_freq(freq[frstSymb]);

	int symbol;
	for (int i = 1; i < size; i++)
	{
		symbol = arCoder->decodeSymbol();
		arConCoder->initCum_freq(freq[symbol]);
		if (symbol == EOF_symbol) break;
	}
	arConCoder->endDecode();
	return 0;
}

