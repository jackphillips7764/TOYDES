#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//all constants needed for encyption and decryption
unsigned char p_init[8] = {1,5,2,0,3,7,4,6};
unsigned char p_final[8] = {3,0,2,4,6,1,7,5};
unsigned char sbox1[4][4] = {{1, 0, 3, 2},
							 {3, 2, 1, 0},
							 {0, 2, 1, 3},
							 {3, 1, 3, 2}};

unsigned char sbox2[4][4] = {{0, 1, 2, 3},
							 {2, 0, 1, 3},
							 {3, 0, 1, 0},
							 {2, 1, 0, 3}};


//here we do permutations
//we do the bit math setting each bit of number with
//bit that is supposed to be there
void permutation(unsigned int * final, unsigned int first,
		unsigned char * perm, unsigned int size){
	if (!final){
		return;
	}
	*final = 0;
	for (int i = 0; i < size; i++){
		//if that bit is set set it in the final result
		if(first & (1 << perm[i])){
			*final |= (1 << i);
		}

	}

}
//left circular shift
//take in a number and an ammount to cricular shift it by
//then does bit operation shifting it and oring it with the
//number with numbers at top bits that got shifted off
unsigned int leftcirularshift5(unsigned int num, int amount){
	unsigned int mask = (1<<5) - 1;
	return ((num << amount) & mask) | (num >> (5 - amount));
}

unsigned char f_func(unsigned char bit_4, unsigned char k){
	//here we set up all permutations we use
	unsigned char expantion[8] = {3,0,1,2,1,2,3,0};
	unsigned char p4[8] = {1,3,2,0};
	unsigned char p2[8] = {1,2};
	unsigned char p2_[8] = {0,3};
	unsigned int expand = 0;
	//we do perutation of the 4 bit number given to func
	//expand it to a 8 bit number
	permutation(&expand, bit_4, expantion, 8);
	//xor it with the key
	unsigned char xo = k ^ expand;

	//split it into two 4 bit chunks
	unsigned char left = xo & ( (1 << 4) - 1);
	unsigned char right = xo >> 4;

	//sboxes
	unsigned int x = 0;
	unsigned int y = 0;

	//do another permutation to
	//pull out the values that make
	//up each index to get numbers it's same as permutation
	permutation(&x, left, p2, 2);
	permutation(&y, left, p2_, 2);

	//now we pull our value from the sbox
	unsigned char two_bit1 = sbox1[y][x];

	x = 0;
	y = 0;
	//we do it again to get second sbox value
	permutation(&x, right, p2, 2);
	permutation(&y, right, p2_, 2);

	unsigned char two_bit2 = sbox2[y][x];

	//we combind the results and do another permutation
	unsigned int thing = (two_bit2 << 2) + two_bit1;
	unsigned int result = 0;
	permutation(&result, thing, p4, 4);
	//return the final result
	return result;


}
//here encryption block with key parts
//with k1, k2 it will encrypt
//when you reverse them it decrypts a block
unsigned char block(unsigned char text,
		unsigned char k1, unsigned char k2){
	unsigned int perm1 = 0;
	//do init permutation on our plain text
	permutation(&perm1, text, p_init, 8);

	//serperate left and right sides
	//4 bit chuncks
	unsigned char left = perm1 & ( (1 << 4) - 1);
	unsigned char right = perm1 >> 4;

	//run them throught the func and xor them
	//to other half
	unsigned char result = f_func(right, k1);
	unsigned char part1 = result ^ left;

	//repeate with other side using above result
	unsigned char result2 = f_func(part1, k2);
	unsigned char part2 = right ^ result2;

	unsigned char pre_result = part2 + (part1 << 4);

	//do final permutation
	unsigned char result_final = 0;
	permutation(&result_final, pre_result, p_final, 8);
	return result_final;

}
//Must be pre allocated string
void encrypt(char * source, unsigned char * dest, unsigned int size, unsigned char k1, unsigned char k2){
	for (int i = 0; i < size; i ++){
		dest[i] = block(source[i], k1, k2);
	}

}

//Must be pre allocated string
void decrypt(unsigned char * source, unsigned char * dest, int size, unsigned char k1, unsigned char k2){
	for (int i = 0; i < size; i ++){
		dest[i] = block(source[i], k2, k1);
	}

}

int main(int argc, char ** argv){
	unsigned char p10[10] = {2,4,1,6,3,9,0,8,7,5};
	unsigned char p8[8] = {5,2,6,3,7,4,9,8};
	/* CREATE KEYS */
	//key operations
	unsigned int key = 742;
	key &= 0x3ff;

	//10 bit permutation
	unsigned int p10_final = 0;
	permutation(&p10_final, key, p10, 10);

	//split into 5 bit chunks
	unsigned int p5_left = p10_final & ((1 << 5) - 1);
	unsigned int p5_right = p10_final >> 5;

	//left rotation by 1
	p5_left = leftcirularshift5(p5_left, 1);
	p5_right = leftcirularshift5(p5_right, 1);


	//get each key by combining left and right
	//and doing 8 bit permutation
	unsigned int k1 = 0;
	permutation(&k1, (p5_right << 5) + p5_left, p8, 8);

	//left rotate again and do p8 again to get k2
	p5_left = leftcirularshift5(p5_left, 1);
	p5_right = leftcirularshift5(p5_right, 1);
	unsigned int k2 = 0;
	permutation(&k2, (p5_right << 5) + p5_left, p8, 8);

	//encrypt a test message

	char * message = "Lool at this super secret message 123129810398204!";
	unsigned int length = strlen(message) + 1;
	unsigned char * encrpt_message = malloc(length + 1);
	unsigned char * decrpt_message = malloc(length + 1);

	encrypt(message, encrpt_message, length, k1, k2);
	for (int i = 0; i < length; i ++){
		printf("%x", encrpt_message[i]);
	}
	printf("\n");

	//decreypt it and print out showing it's the same
	decrypt(encrpt_message,decrpt_message, length, k1, k2);
	for (int i = 0; i < length; i ++){
		printf("%c", decrpt_message[i]);
	}
	printf("\n");

	return 0;
}
