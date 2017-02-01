#include "types.h"

int separate(uint8_t * out1, uint8_t * out2, uint8_t * input, int in_size)
{
	if (in_size%2 != 0)
	{
		error_hdl(-1,"Input of separate not of even size");
		return 1;
	}

	for (int i = 0; i < (in_size/2); ++i)
	{
		out1[i] = input[2*i];
		out2[i] = input[2*i +1];
	}
	return 0;
}

unsigned char hex_value(int nb)
{
	unsigned char result;
	if (nb < 10)
		result = (char) nb + 48;
	else
		result = (char) nb + 87;
	return result;
}

unsigned int value(char c)
{
    if (c >= '0' && c <= '9') { return c - '0';      }
    if (c >= 'a' && c <= 'f') { return c - 'a' + 10; }
    if (c >= 'A' && c <= 'F') { return c - 'A' + 10; }
    return 1;
}

int xor_byte (uint8_t * out, uint8_t * in1, uint8_t * in2, int size)
{
	for (int i = 0; i < size; ++i)
	{
		out[i] = (uint8_t) (in1[i] ^ in2[i]);					// Xor the two strings char by char
	}
	return 0;
}

int xor_hex (unsigned char * out, unsigned char * in1, unsigned char * in2)
{
	static char const alphabet[] = "0123456789abcdef";

	int size = sizeof(in1);

	if (size != sizeof(in2) || size != sizeof(out))
	{
		error_hdl(-1,"Strings of different sizes in xor");
		return 1;
	}

	int alpha_index;

	for (int i = 0; i < size; ++i)
	{
		alpha_index = (value(in1[i]) ^ value(in2[i]));		// Xor the two strings char by char
		out[i] = alphabet[alpha_index];						// Put the result in out
	}
	return 0;
}

int concatenate (uint8_t * out, int size_out, uint8_t * in1, int size1, uint8_t * in2, int size2)
{
	for (int i = 0; i < size_out; ++i)
	{
		if (i < size1)
		{
			out[i] = in1[i];
		}
		else
		{
			out[i] = in2[i-size1];
		}
		
	}
	return 0;
}


uint8_t * char_to_uint8_t (int * size, char * char_array)
{
	int j = 0;
	while (char_array[j] != '\0' && j != MAX_STR_SIZE)
	{
		j++;
	}
	if (*size == MAX_STR_SIZE)
	{
		error_hdl(-1, "String too long");
		return NULL;
	}

	uint8_t * byte_array = malloc(sizeof(uint8_t)*(j));
	for (int i = 0; i < j; ++i)
	{
		byte_array[i] = (uint8_t) char_array[i];
	}
	*size = j;
	return byte_array;
}

int uint8_t_to_char (char * char_array,uint8_t * byte_array, int size)
{
	for (int i = 0; i < size; ++i)
	{
		char_array[i] = (char) byte_array[i];
	}
	return 0;
}

int hex_to_uint8_t (uint8_t * byte_array , char * hex_array, int byte_size)
{
	for (int i = 0; i < byte_size; ++i)
	{
		byte_array[i] = (uint8_t) ( value(hex_array[2*i] * 16) + value(hex_array[2*i +1]) );
	}
	return 0;
}

int uint8_t_to_hex (char * hex_array, uint8_t * byte_array , int byte_size)
{
	int nb1;
	int nb2;
	// A hex number will be seen as nb1*16 + nb2
	for (int i = 0; i < byte_size; ++i)
	{
		nb1 =  ((int) byte_array[i]) /16;
		nb2 = ((int) byte_array[i])%16;
		hex_array[2*i] = hex_value(nb1);
		hex_array[2*i+1] = hex_value(nb2);
	}
	return 0;
}


int uint8_t_to_fmpz_t(fmpz_t out_v, uint8_t * byte_array , int byte_size)
{
	//convert from uint8_t to char in HEX
	char * v_c = malloc(sizeof(char)*(byte_size*2+1));
	uint8_t_to_hex(v_c, byte_array, byte_size);
	v_c[byte_size*2] = '\0';
	
	//convert into an integer
	if(fmpz_set_str(out_v, v_c, 16))
		return 1;
	
	return 0;
}

int uint8_t_copy(uint8_t * out, uint8_t in, int nb_bits)
{
	if (nb_bits >= 8)
	{
		*out = in;
	}
	else
	{
		uint8_t temp = (uint8_t) 255;
		temp = temp >> (8-nb_bits);

		*out = temp & in;
	}
	return 0;

}

int max(int in1, int in2)
{
	if (in1 >= in2)
		return in1;
	else
		return in2;

}

int label(char * out, int j)
{
	char nb[] = "0123456789";
	char * ptr = out;


	int shift = j; 

	do
	{
        ptr++;
        shift = shift/10;
    }
    while(shift);
    *ptr = '\0';
    do
    {
        *--ptr = nb[j%10];
        j = j/10;
    }
    while(j);
	
	return 0;
}
