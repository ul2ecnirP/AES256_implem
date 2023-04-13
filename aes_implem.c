#include <stdio.h>
#include <stdlib.h>
#define TAILLE_TOURS 32768
#define DECRYPT 0
#define ENCRYPT 1
#if (ENCRYPT == 1)
    #define CRYPT_START() br_aes_big_cbcenc_run(&ctx, iv, str, taille);
    #define CRYPT_INIT() br_aes_big_cbcenc_init(&ctx, key, 32);
    #define CRYPT_CTX br_aes_big_cbcenc_keys
#endif
#if (DECRYPT == 1)
    #define CRYPT_START() br_aes_big_cbcdec_run(&ctx, iv, str, taille);
    #define CRYPT_INIT() br_aes_big_cbcdec_init(&ctx, key, 32);
    #define CRYPT_CTX br_aes_big_cbcdec_keys
#endif


#include "BearSSL/inc/bearssl.h"

uint8_t key[32] = {
(uint8_t) 0x46, (uint8_t) 0x46,(uint8_t) 0x46,(uint8_t) 0x46,(uint8_t) 0x46, (uint8_t) 0x46,(uint8_t) 0x46,(uint8_t) 0x46,
(uint8_t) 0x46, (uint8_t) 0x46,(uint8_t) 0x46,(uint8_t) 0x46,(uint8_t) 0x46, (uint8_t) 0x46,(uint8_t) 0x46,(uint8_t) 0x46,
(uint8_t) 0x46, (uint8_t) 0x46,(uint8_t) 0x46,(uint8_t) 0x46,(uint8_t) 0x46, (uint8_t) 0x46,(uint8_t) 0x46,(uint8_t) 0x46,
(uint8_t) 0x46, (uint8_t) 0x46,(uint8_t) 0x46,(uint8_t) 0x46,(uint8_t) 0x46, (uint8_t) 0x46,(uint8_t) 0x46,(uint8_t) 0x46,

};
uint8_t iv[16]  = {
(uint8_t) 0x58,(uint8_t) 0x58,(uint8_t) 0x58,(uint8_t) 0x58,
(uint8_t) 0x58,(uint8_t) 0x58,(uint8_t) 0x58,(uint8_t) 0x58,
(uint8_t) 0x58,(uint8_t) 0x58,(uint8_t) 0x58,(uint8_t) 0x58,
(uint8_t) 0x58,(uint8_t) 0x58,(uint8_t) 0x58,(uint8_t) 0x58,
};


void AES256Implem(FILE *targetfile, unsigned long long sz, uint8_t iv[16]){
    CRYPT_CTX ctx;
    CRYPT_INIT();
    unsigned int taille = TAILLE_TOURS;
    uint8_t *str = malloc(TAILLE_TOURS);
    unsigned long long addi = 0;
    while (addi != sz)
    {
        if(sz > addi){
            addi = addi+taille;
            if(sz > addi){
                fread(str, sizeof(uint8_t), taille, targetfile);
                fseek(targetfile, -taille, SEEK_CUR);
                CRYPT_START();
                fwrite(str, sizeof(uint8_t), taille, targetfile);
                fseek(targetfile, 0, SEEK_CUR); 
            }
        }
        else{
            addi=addi-taille;
            taille = taille/2;
            if(taille < 16){
                printf("fin\n");
                break;
            }
        }
        if(addi == sz){
            fread(str, sizeof(uint8_t), taille, targetfile);
            fseek(targetfile, -taille, SEEK_CUR);
            CRYPT_START();
            fwrite(str, sizeof(uint8_t), taille, targetfile);
            fseek(targetfile, 0, SEEK_CUR); 
        }
    }
    printf("RESTE: %llu", sz-addi);
    fclose(targetfile);
}

int main(int argc, char *argv[]){
    if(argc != 2){
        printf("aes.exe <file>");
        exit(EXIT_FAILURE);
    }
    FILE *targetfile = fopen(argv[1], "rb+");
    if(targetfile == NULL){
        printf("erreur de fichier");
        exit(EXIT_FAILURE);
    }
    unsigned long long sz = 0;
    fseek(targetfile, 0uL, SEEK_END);
    sz = _ftelli64(targetfile);
    if(sz <= 0){
        printf("Erreur: uniquement 1<taille<4294967295");
        exit(EXIT_FAILURE);
    }    
    printf("SIZE:%llu\n", sz);
    rewind(targetfile);
    AES256Implem(targetfile,sz,iv);
    return 1;
}


//fsutil file createnew <filename> <length>
