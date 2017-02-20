#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "arithmeticCoder.h"
#include "arithmeticCoderTools.h"
#include "defines.h"
#include "arithmeticConCoder.h"
#include "arithmeticConCoderTools.h"

using namespace std;

void writeStrFile(vector<unsigned char> &str, int size_str){
    FILE *file = fopen("stream3.txt", "wb");
    for (int i = 0; i < size_str; i++){
	int k = str[i];
	fprintf(file, "%d", k);
    }
    fclose(file);
}

void generateStream(int N){
      //float p[5]={0};
      float p[5] = {0.3215, 0.0023, 0.21546, 0.13219, 0.32855};
      float sum = 1.0;
      /*for (int i = 0; i < 5; i++){
	    p[i] = (float)rand()/RAND_MAX;
	    while (p[i] > sum){
		p[i] = (float)rand()/RAND_MAX;
		if (p[i] < 0.0001){
		    p[i] = 0;
		}
	    }
	    sum -= p[i];
	    p[4] = sum;
      }*/
      int state = 0;
      std::vector <unsigned char> str;
      for (int i = 0; i < N; i++){
	  float vr = (float)rand()/RAND_MAX;
	  if (vr < p[0]){
	      state = 0;
	  }
	  else if (vr >= p[0] && vr < p[0]+p[1]){
	      state = 1;
	  }
	  else if (vr >= p[0]+p[1] && vr < p[0]+p[1] + p[2]){
	      state = 2;
	  }
	  else if (vr >= p[0]+p[1] + p[2] && vr < p[0]+p[1] + p[2] + p[3]){
	      state = 3;
	  }
	  else if (vr >= p[0]+p[1] + p[2] + p[3] && vr < 1){
	      state = 4;
	  }
	  str.push_back(state);
      }
      writeStrFile(str, str.size());
}

int main(){
	srand(time(NULL));
	int N = 10000;
	generateStream(N);
	
//Arithmetic
	ArithmeticCoder *arCoder = new ArithmeticCoder();
	int alphabetSize = No_of_chars;
	FILE *f;
	f = fopen("stream3.txt", "rb");
	fseek(f, 0L, SEEK_END);
	int size = ftell(f);
	fseek(f, 0L, SEEK_END);
	rewind(f);
	char *inputData = new char[size];
	fread(inputData, size, 1, f);
	int *freq = new int[alphabetSize + 2];

	for (int i = 0; i < alphabetSize + 2; i++){
		freq[i] = 0;
	}
	freq[EOF_symbol] = 1;
	for (int i = 0; i < size; i++){
		freq[(int)inputData[i]]++;
	}

	arCoder->initEncoder(inputData, size, freq);
	for (int i = 0; i < size; i++){
		arCoder->encodeSymbol(inputData[i]);
	}
	arCoder->encodeSymbol(EOF_symbol);
	arCoder->endEncode();
	fclose(f);
	arCoder->initDecoder();
	int symbol;
	for (int i = 0; i < size; i++){
		symbol = arCoder->decodeSymbol();
		if (symbol == EOF_symbol) break;
	}
	arCoder->endDecode();

//Context Arithmetic
	
	ArithmeticConCoder *arConCoder = new ArithmeticConCoder();
	int **freqCon = new int*[alphabetSize + 2];
	for (int i = 0; i < alphabetSize + 2; i++){
		freqCon[i] = new int[alphabetSize + 2];
		for (int i2 = 0; i2 < alphabetSize + 2; i2++){
			freqCon[i][i2] = 0;
		}

	}
	for (int i = 0; i < alphabetSize + 2; i++){
		freqCon[i][EOF_symbol] = 1;
	}
	for (int i = 0; i < size; i++){
		int y = (int)inputData[i];
		int x = (int)inputData[i + 1];
		freqCon[y][x]++;
	}
	
	FILE *f2;
	f2 = fopen(ConEncodeFileName, "wb");
	fwrite(&inputData[0], 1, 1, f2);
	fclose(f2);
	
	arConCoder->initEncoder(inputData, size, freqCon[inputData[0]]);
	for (int i = 1; i < size; i++){
		arConCoder->encodeSymbol(inputData[i]);
		arConCoder->initCum_freq(freqCon[inputData[i]]);
	}
	arConCoder->encodeSymbol(EOF_symbol);
	arConCoder->endEncode();

	char frstSymb = 0;
	arConCoder->initDecoder(&frstSymb);
	arConCoder->initCum_freq(freqCon[frstSymb]);

	symbol = 0;
	for (int i = 1; i < size; i++){
		symbol = arConCoder->decodeSymbol();
		arConCoder->initCum_freq(freqCon[symbol]);
		if (symbol == EOF_symbol) break;
	}
	arConCoder->endDecode();
	
	std::cout<<"Hello!!!"<<std::endl;
	return 0;
}

