#include <stdio.h>
#include <unistd.h>
#include <openssl/bn.h>

#define NBITS 256

void printBN(char *msg, BIGNUM *a){
	
	/* Use BN_bn2hex(a) for hex string
	* Use BN_bn2dec(a) for decimal string*/
	
	char *number_str = BN_bn2hex(a);
	printf("%s %s\n", msg, number_str);
	OPENSSL_free(number_str);
}

int main(){

	BN_CTX *ctx = BN_CTX_new();

	BIGNUM *a = BN_new();
	BIGNUM *b = BN_new();
	BIGNUM *n = BN_new();

	BIGNUM *d = BN_new();
	BIGNUM *e = BN_new();
	BIGNUM *m = BN_new();

	BIGNUM *res = BN_new();

	BN_dec2bn(&e, "65537");
	BN_hex2bn(&m, "4120746f702073656372657421");
	BN_hex2bn(&n, "DCBFFE3E51F62E09CE7032E2677A78946A849DC4CDDE3A4D0CB81629242FB1A5");
	BN_hex2bn(&d, "74D806F9F3A62BAE331FFE3F0A68AFE35B3D2E4794148AACBC26AA381CD7D30D");

	BN_mod_exp(res, m, e, n, ctx);

	printBN("res = ", res);

	return 0;
}
