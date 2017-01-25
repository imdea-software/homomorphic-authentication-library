 #include "prf.h"

int prf(uint8_t * out, int out_size, uint8_t * input, int in_size, uint8_t * key, int key_size)
{
	switch (ENC_TYPE)
	{
		case 1:		// hmac
			hmac(out, out_size, input,  in_size, key, key_size);
			break;
		case 2:		// aes
			aes(out, out_size, input,  in_size, key, key_size);
			break;
		default:
			error_hdl(-1,"No such ENC_TYPE");
			return 1;
	};
	return 0;
}

int hmac(uint8_t * out, int out_size, uint8_t * input, int in_size, uint8_t * key, int key_size)
{
	// Get the key from the key file key_file 
	
	if (out_size < HASH_LEN)
	{
		error_hdl(1,"Output string not big enough to get the hash value");
		return 1;
	}
	int BLOCK_SIZE = 0;

	// If the hash function is either sha-256 or sha-224 or sha-1
	if (HASH_LEN == 28 || HASH_LEN == 32 || HASH_LEN == 20)
	{
		BLOCK_SIZE = 64;
	}
	if (HASH_LEN == 48 || HASH_LEN == 64)
	{
		BLOCK_SIZE = 128;
	}
	if (BLOCK_SIZE == 0)
	{
		error_hdl(1,"HASH_LEN not accepted");
		return 1;
	}

	// Variable in which we will copy the appropriate key
	
	int max_size = (BLOCK_SIZE > key_size)*BLOCK_SIZE + key_size*(key_size >= BLOCK_SIZE);
	uint8_t * K = malloc(sizeof(uint8_t)* max_size);
	int temp_size = key_size;		// The key can be hashed and therefore its temporary size changed

	// Fill the key with the input one
	for (int i = 0; i < temp_size; ++i)
	{
		K[i] = key[i];
	}

	if (temp_size > BLOCK_SIZE)			// In this case hash the key down to the block size
	{
		md_map_sh512(K,K,temp_size);		// Hash K into K
		temp_size = 64;					// The size of the key is now 64 bytes
	}

	if (temp_size < BLOCK_SIZE)
	{	
		// Pad the rest of the key with zeros
		for (int i = temp_size; i < BLOCK_SIZE; ++i)
		{
			K[i] = (uint8_t) 0x00;
		}
		temp_size = BLOCK_SIZE;
	}

	// At this point, we have temp_size = BLOCK_SIZE whatever happens

	// Xor the key to create k_o and k_i, two variations on the key.

	uint8_t * k_o = malloc(sizeof(uint8_t)*temp_size);
	uint8_t * k_i = malloc(sizeof(uint8_t)*temp_size);

	uint8_t opad = (uint8_t) 0x5c;
	uint8_t ipad = (uint8_t) 0x36;

	for (int i = 0; i < temp_size; ++i)
	{
		k_o[i] = K[i] ^ opad;
		k_i[i] = K[i] ^ ipad; 
	}

	int error_ind = 0;		// Error indicator, has to stay equal to 0

	// Proccessing the hmac: h(k_o || h(k_i || m)) where m is the message
	// We call 	inner_input = k_i || m
	//			outer_input = k_o || h(k_i || m)


	unsigned char * inner_input = malloc(sizeof(unsigned char)*(in_size + temp_size));				// Size is size of input + key size
	unsigned char * outer_input = malloc(sizeof(unsigned char)*(HASH_LEN + temp_size));			

	error_ind += concatenate(inner_input, in_size + temp_size, k_i, temp_size, input, in_size);

	if(hash_func(out,inner_input, in_size + temp_size))
	{
		error_hdl(1,"Problem in the inner hash");
		return 1;
	}

	error_ind += concatenate(outer_input, HASH_LEN + temp_size, k_o, temp_size, out, HASH_LEN);

	if(hash_func(out, outer_input, HASH_LEN + temp_size))
	{
		error_hdl(1,"Problem with the outer hash");
		return 1;
	}

	if (error_ind > 0)
	{
		error_hdl(1,"Error while processing the concatenations");
		return 1;
	}

	free(K);
	free(outer_input);
	free(inner_input);
	free(k_o);
	free(k_i);

	return 0;
}

int hash_func(uint8_t * hash, uint8_t * msg, int length)
{
	switch (HASH_LEN)
	{
		case 20:
			md_map_shone(hash, msg, length);
			break;
		case 28:
			md_map_sh224(hash, msg, length);
			break;
		case 32:
			md_map_sh256(hash, msg, length);
			break;
		case 48:
			md_map_sh384(hash, msg, length);
			break;
		case 64:
			md_map_sh512(hash, msg, length);
			break;
		default:
			error_hdl(1,"No hash function for this output size");
			return 1;
	};
	return 0;
}

int expand(uint8_t * out, uint8_t * in, int in_size, int bit_number)
{
	// The input is concatenated with a fixed byte then hashed down to
	// 32 bytes with sha256.
	// This operation is repeated n times with a different byte to concatenate
	// until we have n*256 >= bit_number. 
	// If needed, the last hash will therefore not be copied entirely into
	// the output but truncated.

	// Bits we need to truncate from the last hash
	int over_bits = bit_number%256;
	// Bytes we need to truncate from the last hash
	// The last byte will not be copied entirely sometimes
	int over_bytes = over_bits/8 + 1*(over_bits%8 != 0);
	
	// Number of hashes to copy entirely into output
	int hash_nb = bit_number/256;

	// Ouput size in bytes
	int out_size = bit_number/8 + 1*(bit_number%8 != 0);
	
	// We take the input and concatenate it with i (i form 1 to the number of hashes we need)
	// We store the concatenation in conc_in
	// We store the hash of that in hashed_in
	// For instance with 2 hashes we get:
	// 		(h(input+1))+(h(input+2))
	uint8_t * hashed_in = malloc(sizeof(uint8_t)*32);
	uint8_t * conc_in = malloc(sizeof(uint8_t)* (in_size +1));


	// Fill the array with the input
	for (int i = 0; i < in_size; ++i)	
	{
		conc_in[i] = in[i];
	}

	// Concatenate and hash
	for (int i = 0; i < hash_nb; ++i)
	{
		// At each turn we change the last byte
		conc_in[in_size] = (uint8_t) i;
		// Hash it
		md_map_sh256(hashed_in, conc_in, in_size+1);
		
		// Copy the bytes from the hashed concatenated input into the output
		for (int j = 0; j < 32; ++j)
		{
			out[i*32 + j] = hashed_in[j];
		}
	}

	// Do the same for the last hash to compute, but truncate the result
	// to the appropriate number of bits
	if ((over_bits != 0))
	{
		// Fill in the last
		conc_in[in_size] = (uint8_t) hash_nb;

		md_map_sh256(hashed_in, conc_in, in_size+1);

		for (int j = 0; j < over_bytes; ++j)
		{
			// Copy the bytes form the last hashed input into the output
			// If over_bits-8*j >= 8, it means all the bits need to be copied
			// Otherwise, it means only that number of bits will be copied
			// and that will be the last round
			uint8_t_copy(out + hash_nb*32 + j, hashed_in[j], over_bits - 8*j);
		}

	}

	free(hashed_in);
	free(conc_in);
	return 0;
}

/*
int aes(uint8_t * out, int out_size, uint8_t * input, int in_size, uint8_t * key, int key_size)
{
	if (HASH_LEN != 32 || key_size != 32)
	{
		error_hdl(-1,"Only 256 bits allowed for aes atm");
		return 1;
	}

	// Get the key from the structure and the 
	// initialization vector
	unsigned char * iv = malloc(sizeof(char)*32);
	unsigned char * K = malloc(sizeof(char)*64);		// 64 hex characters -> 32 bytes

	for (int i = 0; i < 256; ++i)
	{
		if (uint8_t_to_hex(K, key, 32))
		{
			error_hdl(-1, "Error converting key 1 to char");
			return 1;
		}
		if (uint8_t_to_hex(iv, key, 16))
		{
			error_hdl(-1, "Error converting key 1 to char for iv");
			return 1;
		}		
	}

	// Convert everything to char
	char * char_input = malloc(sizeof(char)* in_size);
	char * char_out = malloc(sizeof(char)* 64);

	if (uint8_t_to_char(char_input, input, in_size))
	{
		error_hdl(-1,"Error converting the input to char");
		return 1;
	}

	// Bits written evrytime by the algorithm
	int * bytes = malloc(sizeof(int));



	EVP_CIPHER_CTX * ctx;
	if (!(ctx = EVP_CIPHER_CTX_new()))
	{
		error_hdl(-1,"Pb creating the evp context");
		return 1;
	}

	if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, K, iv))
	{
		error_hdl(-1,"Pb with encryption initialization");
		return 1;
	}

	if(1 != EVP_EncryptUpdate(ctx, char_out, bytes, char_input, in_size))
	{
		error_hdl(-1,"Pb with inputing message");
		return 1;
	}
	//printf("Written bytes:%d\n", *bytes);

	if(1 != EVP_EncryptFinal_ex(ctx, char_out + *bytes, bytes))
	{
		error_hdl(-1,"Pb with encryption");
		return 1;
	}
	//printf("Written bytes:%d\n", *bytes);
	EVP_CIPHER_CTX_free(ctx);

	if (hex_to_uint8_t(out, char_out, 32))
	{
		error_hdl(-1,"Error converting the output to uint8_t");
		return 1;
	}



	// Clean up

	free(char_input);
	free(char_out);
	free(bytes);
	free(iv);
	free(K);

	return 0;
} 
*/