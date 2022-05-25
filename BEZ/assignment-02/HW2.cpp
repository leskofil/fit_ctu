#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>

char isValidHash (int numberOfZeros, unsigned char* hash) {
    //char hash[] = {0b0000000, 0b00010000};
    for (int i = 0; i < numberOfZeros; ++i) {
            int j = i/8;
            int pos = i%8;
            unsigned int flag = 128;

            flag = flag >> pos;

            if (hash[j] & flag) {
                return 0;
            }
        }
    return 1;    
}

int main (int argc, char * argv[]) {
    //char text[] = "";
    char hashFunction[] = "sha384";  // zvolena hashovaci funkce ("sha1", "md5", ...)

    int numberOfZeros;
    int scanRes = sscanf(argv[1], "%d", &numberOfZeros);
    if (argc != 2 || scanRes != 1 || numberOfZeros < 0 || numberOfZeros > 384) {
        //printf("Invalid Input\n");
        return 1;
    }
    
    EVP_MD_CTX * ctx;  // struktura kontextu
    const EVP_MD * type; // typ pouzite hashovaci funkce
    unsigned char hash[EVP_MAX_MD_SIZE]; // char pole pro hash - 64 bytu (max pro sha 512)
    unsigned int length;  // vysledna delka hashe

    /* Inicializace OpenSSL hash funkci */
    OpenSSL_add_all_digests();
    /* Zjisteni, jaka hashovaci funkce ma byt pouzita */
    type = EVP_get_digestbyname(hashFunction);

    /* Pokud predchozi prirazeni vratilo -1, tak nebyla zadana spravne hashovaci funkce */
    if (!type) {
        printf("Hash %s neexistuje.\n", hashFunction);
        return 1;
    }

    ctx = EVP_MD_CTX_new(); // create context for hashing
    if (ctx == NULL)
        return 2;
    
    char *text = (char*) malloc(numberOfZeros + 2);
    int k = 0;
    srand(time(0));
    while(k == 0) {
        
        for(int i = 0; i < numberOfZeros; i++) {
            text[i] = rand()%100;
        }
        
        if (numberOfZeros != 0)
            text[numberOfZeros] = '\0';
        else {
            text[numberOfZeros] = 'a';
            text[numberOfZeros + 1] = '\0';
        }
        //printf("%s\n", text);
        /* Hash the text */
        if (!EVP_DigestInit_ex(ctx, type, NULL)) // context setup for our hash type
            return 3;

        if (!EVP_DigestUpdate(ctx, text, strlen(text))) // feed the message in
            return 4;

        if (!EVP_DigestFinal_ex(ctx, hash, &length)) // get the hash
            return 5;
        
        k = isValidHash(numberOfZeros, hash);
    }

    EVP_MD_CTX_free(ctx); // destroy the context

    /* Vypsani vysledneho hashe */

    for (unsigned int i = 0; i < strlen(text); i++)
        printf("%02x", text[i]);
    printf(" ");    
    for (unsigned int i = 0; i < length; i++)
        printf("%02x", hash[i]);
    printf("\n");
    free(text);
    return 0;
}