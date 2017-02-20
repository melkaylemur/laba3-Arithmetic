#define No_of_chars 256
#define EOF_symbol (No_of_chars+1)

#define No_of_symbols (No_of_chars+1)

#define Max_frequency 76001

#define Code_value_bits 16
typedef long code_value;

#define Top_value (((long)1<<Code_value_bits)-1)

#define EncodeFileName "stream_encode_1.txt"
#define DecodeFileName "stream_decode_1.txt"
#define ConEncodeFileName "stream_encode_2.txt"
#define ConDecodeFileName "stream_decode_2.txt"

#define First_qtr (Top_value/4+1)
#define Half	  (2*First_qtr)
#define Third_qtr (3*First_qtr)	
