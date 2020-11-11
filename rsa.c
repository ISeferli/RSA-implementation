#include "rsa.h"
#include "utils.h"
#include <stdbool.h>
#include<time.h> 

/*
 * Sieve of Eratosthenes Algorithm
 * https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
 *
 * arg0: A limit
 * arg1: The size of the generated primes list. Empty argument used as ret val
 *
 * ret:  The prime numbers that are less or equal to the limit
 */
size_t * sieve_of_eratosthenes(int limit, int *primes_sz){
	size_t *primes;

	/* TODO */		
	bool* boolPrime; 
	boolPrime = (bool*)malloc(sizeof(bool)*(limit+1));
    memset(boolPrime, true, limit+1);  //Making a boolean array where every index is true

	int counter = 0;
	int finalCount = 0;
	for(int p=2; p*p<=limit; p++){
		if(boolPrime[p]==true){ //Means that we have a prime number and we need to mark its multipliers
			for(int i=p*p; i<=limit; i+=p){
				boolPrime[i] = false;
			}
		}
	}

	for(int i=2; i<limit; i++){
		if(boolPrime[i]==true){
			counter++; //For every prime the counter increases
		}
	}

	*primes_sz = counter;	
	primes = (size_t*)malloc(sizeof(size_t)*counter);

	for(int i=2; i<limit; i++){
		if(boolPrime[i]==true){
			primes[finalCount] = i;
			finalCount++;
		}
	}

	return primes;
}


/*
 * Greatest Common Denominator
 *
 * arg0: first number
 * arg1: second number
 *
 * ret: the GCD
 */
int gcd(int a, int b){
	/* TODO */
	int rem;

	//Using Euclidean Algorithm
	if(a==0){
		return b;
	}	
	rem = b%a;
	return gcd(rem, a);
}


/*
 * Chooses 'e' where 
 *     1 < e < fi(n) AND gcd(e, fi(n)) == 1
 *
 * arg0: fi(n)
 *
 * ret: 'e'
 */
size_t choose_e(size_t fi_n){
	size_t e;

	/* TODO */
	srand(time(0)); //sets the starting point for producing a series of pseudo-random integers
	e = (rand() % (fi_n)) + 1; //Taking a random number from 1 till fi_n
	while((gcd(e, fi_n) != 1) || (e%fi_n==0)){
		//Repeating the process until a right number of e is found
		e = (rand() % (fi_n)) + 1;
	}

	return e;
}


/*
 * Calculates the modular inverse
 *
 * arg0: first number
 * arg1: second number
 *
 * ret: modular inverse
 */
size_t mod_inverse(size_t a, size_t b){
	/* TODO */
    a = a%b; 
    for (int x=1; x<b; x++){ 
       if ((a*x) % b == 1){ 
          return x;
	   } 
	}
}


/*
 * Generates an RSA key pair and saves
 * each key in a different file
 */
void rsa_keygen(void){
	size_t p;
	size_t q;
	size_t n;
	size_t fi_n;
	size_t e;
	size_t d;

	int primesSize, randomIndex;
	/* TODO */
	size_t* primeList;
	primeList = sieve_of_eratosthenes(RSA_SIEVE_LIMIT, &primesSize);
	
	srand(time(0));
	randomIndex = rand() % primesSize;
	p = primeList[randomIndex];

	randomIndex = rand() % primesSize;
	q = primeList[randomIndex];

	n = p*q;
	fi_n = (p-1)*(q-1);
	e = choose_e(fi_n);
	d = mod_inverse(e, fi_n);

	size_t* pubKeyPointer;
	size_t* privKeyPrivate;
	size_t* helpPub; //Helping pointer to keep the head of the pointer pubKeyPointer
	size_t* helpPriv; //Helping pointer to keep the head of the pointer privKeyPrivate
	pubKeyPointer = (size_t*)malloc(2*sizeof(size_t)); //These pointers will have two positions for two numbers of size size_t
	privKeyPrivate = (size_t*)malloc(2*sizeof(size_t));
	helpPub = pubKeyPointer;
	*helpPub = n; //The first position of the pointer shows the address of n
	helpPub++; //Incrementing the pointer to show the next position
	*helpPub = d; //The second position of the pointer shows the address of d
	FILE* pubKey;
	pubKey = fopen("public.key", "w");
	if(pubKey == NULL){
		printf("Error.");   
		exit(1);             
	}
	
	fwrite(pubKeyPointer, sizeof(size_t), 2, pubKey); //Writing the content of pubKeyPointer in a file pubKey that has 2 size_t contents
	fclose(pubKey);

	helpPriv = privKeyPrivate;
	*helpPriv = n;
	helpPriv++;
	*helpPriv = e;
	FILE* privKey;
	privKey = fopen("private.key", "w");
	if(privKey == NULL){
		printf("Error.");   
		exit(1);             
	}

	fwrite(privKeyPrivate, sizeof(size_t), 2, privKey);
	fclose(privKey);
}


/*
 * Encrypts an input file and dumps the ciphertext into an output file
 *
 * arg0: path to input file
 * arg1: path to output file
 * arg2: path to key file
 */
void rsa_encrypt(char *input_file, char *output_file, char *key_file){
	/* TODO */
	FILE* in;
	FILE* out;
	FILE* keyF;
	size_t cipher;
	int input_len;
	size_t* keyPointer;
	size_t n;
	size_t d;

	in = fopen(input_file, "r");
	if(in == NULL){
		printf("Error in opening file for encryption.");   
		exit(1);             
	}
	fseek(in, 0, SEEK_END);
	input_len = ftell(in);
	fseek(in, 0, SEEK_SET);
	unsigned char plaintext[input_len];
	if(!(fread(plaintext, 1, input_len, in))){
		printf("Error in fread");
	}
	fclose(in);

	keyPointer = (size_t*)malloc(2*sizeof(size_t));
	keyF = fopen(key_file, "r");
	if(keyF == NULL){
		printf("Error in opening file for encryption.");   
		exit(1);             
	}
	fread(keyPointer, sizeof(size_t), 2, keyF);
	fclose(keyF);
	n = *keyPointer;
	keyPointer++;
	d = *keyPointer;

	size_t* ciphertext;
	size_t* headOfCipher;
	out = fopen(output_file, "w");
	ciphertext = (size_t*)malloc(input_len*sizeof(size_t));
	headOfCipher = ciphertext;
	for(int i=0; i<input_len; i++){
		cipher = 1;
		for(int j=0; j<d; j++){
			cipher = (cipher * plaintext[i])%n;
		}
		*ciphertext = cipher;
		ciphertext++;
	}
	fwrite(headOfCipher, sizeof(size_t), input_len, out);
	fclose(out);
}


/*
 * Decrypts an input file and dumps the plaintext into an output file
 *
 * arg0: path to input file
 * arg1: path to output file
 * arg2: path to key file
 */
void rsa_decrypt(char *input_file, char *output_file, char *key_file)
{
	/* TODO */
	FILE* in;
	FILE* out;
	FILE* keyF;
	int input_len;
	size_t* keyPointer;
	size_t n;
	size_t e;

	in = fopen(input_file, "r");
	if(in == NULL){
		printf("Error in opening file for encryption.");   
		exit(1);             
	}
	fseek (in, 0, SEEK_END);
	input_len = ftell(in);
	fseek (in, 0, SEEK_SET);
	size_t ciphertext[input_len/8];
	if(!(fread(&ciphertext, sizeof(size_t), input_len/8, in))){
		printf("Error in fread");
	}
	fclose(in);

	keyPointer = (size_t*)malloc(2*sizeof(size_t));
	keyF = fopen(key_file, "r");
	if(keyF == NULL){
		printf("Error in opening file for encryption.");   
		exit(1);             
	}
	fread(keyPointer, sizeof(size_t), 2, keyF);
	fclose(keyF);
	n = *keyPointer;
	keyPointer++;
	e = *keyPointer;

	out = fopen(output_file, "w");
	size_t plain;
	unsigned char* plaintext;
	unsigned char* headOfPlain;
	plaintext = (unsigned char*)malloc((input_len/8)*sizeof(unsigned char*));
	headOfPlain = plaintext;
	for(int i=0; i<input_len/8; i++){
		plain = 1;
		for(int j=0; j<e; j++){
			plain = (plain * ciphertext[i])%n;
		}
		*plaintext = plain;
		plaintext++;
	}
	fwrite(headOfPlain, 1, (input_len/8), out);
	fclose(out);
}
