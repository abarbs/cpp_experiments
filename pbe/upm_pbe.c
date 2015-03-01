/*
  Code to decrypt the encrypted portion of the Universal Password Manager
  upm.db file (upm.sourceforge.net) using C/openssl. UPM's PBE algorithm
  (as of 3/1/2015) uses the SHA256 digest function, AES256 cipher.

  Parsing the rest of the UPM.db file (reading the file version/salt)
  is not yet implemented. Right now, it is assumed the salt is of length
  PKCS12_SALT_LEN, and immediately precedes the encrypted block.

  Based on example program from http://marc.info/?l=openssl-users&m=109506216926970&w=2
*/
#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/pkcs12.h>
#include <openssl/err.h>

#define PASSGEN_NUMITER 20 // Default num iterations from UPM, probably pretty low ...

static int pbe_nid = 0;

int do_crypt(FILE *in, FILE *out, int crypt_mode, char *pswd);

int main(int argc, char **argv) {
  FILE *in, *out;
  ERR_load_crypto_strings();

  OpenSSL_add_all_algorithms();
  OpenSSL_add_all_ciphers();
  OpenSSL_add_all_digests();

  pbe_nid = OBJ_create("1.0.0.0", "m-PBE-SHA256-AES-256",
                       "my-pbeWithSHA256And256BitAES-CBC");
  PKCS5_PBE_add();
  EVP_PBE_alg_add(pbe_nid, EVP_aes_256_cbc(), EVP_sha256(), PKCS12_PBE_keyivgen);

  if (argc < 5) {
    printf("Usage: %s pswd infile outfile cryptmode\n", argv[0]);
    return 2;
  }
  if ((in = fopen(argv[2], "rb")) == NULL) {
    printf("Error reading input\n");
    return 3;
  }
  if ((out = fopen(argv[3], "wb")) == NULL) {
    printf("Error writing output\n");
    return 4;
  }
  if (do_crypt(in, out, atoi(argv[4]), argv[1]) == 0) {
    printf("Error running do_crypt()\n");
    return 5;
  }

  fclose(in);
  fclose(out);
  ERR_free_strings();
  return 0;
}

int do_crypt(FILE *in, FILE *out, int crypt_mode, char *pswd) {
  unsigned char inbuf[BUFSIZ], outbuf[BUFSIZ + EVP_MAX_BLOCK_LENGTH];
  int inlen, outlen;
  unsigned char salt[PKCS12_SALT_LEN];
  int ret = 0; // Assume the worst
  X509_ALGOR *pbe;
  EVP_CIPHER_CTX ctx;

  EVP_CIPHER_CTX_init(&ctx);

  // If encrypting, generate salt and write to out stream, else read
  // the salt from input stream
  if (crypt_mode) {
    if (RAND_bytes(salt, PKCS12_SALT_LEN) != 1
        || fwrite(salt, 1, PKCS12_SALT_LEN, out) < PKCS12_SALT_LEN)
      goto clean;
  } else {
    if (fread(salt, 1, PKCS12_SALT_LEN, in) < PKCS12_SALT_LEN)
      goto clean;
  }

  pbe = PKCS5_pbe_set(pbe_nid, PASSGEN_NUMITER, salt, PKCS12_SALT_LEN);

  /*

  As of today, the default in OpenSSL's PKCS12_key_gen function (called by
  PKCS12_PBE_keyivgen) is to assume the password is ASCII (hence we'll assume
  the same and use strlen(pswd)).

  Can #define PBE_UNICODE when building OpenSSL for implementation to use
  PKCS12_key_gen_uni (the unicode version). If you're allowing unicode passwords,
  you'll need to make appropriate change to calculate the password length.

  */
  if (pbe == NULL || !EVP_PBE_CipherInit(pbe->algorithm, pswd, strlen(pswd),
                                          pbe->parameter, &ctx, crypt_mode)) {
    goto clean;
  }

  for (;;) {
    if ((inlen = fread(inbuf, 1, BUFSIZ, in)) ==  0) {
      if (ferror(in)) {
        goto clean;
      } else {
        break;
      }
    }

    if (!EVP_CipherUpdate(&ctx, outbuf, &outlen, inbuf, inlen))
      goto clean;

    if (fwrite(outbuf, 1, outlen, out) < outlen && ferror(out))
      goto clean;
  }

  if (!EVP_CipherFinal_ex(&ctx, outbuf, &outlen))
    goto clean;
  if (fwrite(outbuf, 1, outlen, out) < outlen && ferror(out))
    goto clean;

  ret = 1;

clean:
  ERR_print_errors_fp(stderr); // Does nothing if there were no errors
  EVP_CIPHER_CTX_cleanup(&ctx);
  EVP_cleanup();
  return ret;
}
