//Filip Lesko
#include <iostream>
#include <iomanip>
#include <fstream>  // fstream
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <openssl/evp.h>
#include <openssl/pem.h>
using namespace std;


int encrypt(const string & keyFile, string & mode, string & fileIn, const string& fileOut) {
    ifstream ifs (fileIn, ios::in | ios::binary);
    if (ifs.fail()) {
        return false;
    }
    ofstream ofs (fileOut, ios::out | ios::binary);
    if (ofs.fail()) {
        return false;
    }

    FILE * pubkeyfile = fopen(keyFile.c_str(),"r");
    if (!pubkeyfile) {
        ofs.close();
        ifs.close();
        return false;
    }
    EVP_PKEY * pubkey = PEM_read_PUBKEY(pubkeyfile, NULL, NULL, NULL);
    if (!pubkey) {
        ofs.close();
        ifs.close();
        return false;
    }
    unsigned char * my_ek = (unsigned char *) malloc(EVP_PKEY_size(pubkey));
    int my_ekl;
    unsigned char iv[EVP_MAX_IV_LENGTH] = "iv"; 
    
    unsigned char ot[1024]; 
    unsigned char st[2048];      

    const EVP_CIPHER * cipher;

    const char* cipherName = "aes-128-cbc";

    OpenSSL_add_all_ciphers();

    cipher = EVP_get_cipherbyname(cipherName);
    if (!cipher) {
        printf("Sifra %s neexistuje.\n", cipherName);
        ofs.close();    
        ifs.close();
    
        EVP_PKEY_free(pubkey);
        fclose(pubkeyfile);
        free(my_ek);
        return 1;
    }

    int tmpLength = 0;

    EVP_CIPHER_CTX * ctx;
    ctx = EVP_CIPHER_CTX_new();
    if (ctx == NULL) {
        ofs.close();    
        ifs.close();
    
        EVP_PKEY_free(pubkey);
        fclose(pubkeyfile);
        free(my_ek);
        return 2;
    }

    if (!EVP_SealInit(ctx, cipher, &my_ek, &my_ekl, iv, &pubkey, 1)) {
        ofs.close();    
        ifs.close();
    
        EVP_PKEY_free(pubkey);
        fclose(pubkeyfile);
        free(my_ek);
        return 3;
    }

    ofs.write((char*)&my_ekl, sizeof(int));
    //size_t p = ofs.tellp();
    for (int i = 0; i < my_ekl; i++) {
        ofs.write((char*)&my_ek[i], sizeof(unsigned char));
    }

    ofs.write((char*)&iv, sizeof(iv));


    while(ifs.read((char*) &ot, sizeof(ot)).gcount() > 0) {

        if (!EVP_SealUpdate(ctx,  st, &tmpLength, ot, ifs.gcount())) {
            ofs.close();    
            ifs.close();
    
            EVP_PKEY_free(pubkey);
            fclose(pubkeyfile);
            free(my_ek);
            return 4;
        }

        ofs.write((char*)&st, tmpLength);
    }

    if (!EVP_SealFinal(ctx, st, &tmpLength)) {
        ofs.close();    
        ifs.close();
    
        EVP_PKEY_free(pubkey);
        fclose(pubkeyfile);
        free(my_ek);
        return 5;
    }

    ofs.write((char*)&st, tmpLength);
    EVP_CIPHER_CTX_free(ctx);
    EVP_PKEY_free(pubkey);
    fclose(pubkeyfile);
    free(my_ek);
    ofs.close();    
    ifs.close();
    return 0;

}

int decrypt(const string & keyFile, string & mode, string & fileIn, const string& fileOut) {
     ifstream ifs (fileIn, ios::in | ios::binary);
    if (ifs.fail()) {
        return false;
    }
    ofstream ofs (fileOut, ios::out | ios::binary);
    if (ofs.fail()) {
        return false;
    }

    int keySize;
    if (ifs.read((char*)&keySize, sizeof(keySize)).gcount() != sizeof(keySize) )
        return false;

    unsigned char * my_ek = (unsigned char *) malloc(sizeof(unsigned char) * keySize);
    for ( int i = 0; i < keySize; i++ )  {
        //size_t p = ifs.tellg();
        if ( ! ifs.read((char*)&my_ek[i], sizeof(unsigned char)) ) {     
            cout << "inv" << endl; 
            ofs.close();    
            ifs.close();

            free(my_ek);
            return false;
        }
    }

    //size_t p = ifs.tellg();
    unsigned char iv[EVP_MAX_IV_LENGTH];
    for ( uint8_t i = 0; i < EVP_MAX_IV_LENGTH; i++ )  {
        if ( ! ifs.read((char*)&iv[i], sizeof(unsigned char)) ) {
            cout << "inv" << endl;
            ofs.close();    
            ifs.close();
            
            free(my_ek);
            return false;
        }        
    }
    //p = ifs.tellg();

    FILE * privatekeyfile = fopen(keyFile.c_str(),"r");
    EVP_PKEY * privkey = PEM_read_PrivateKey(privatekeyfile, NULL, NULL, NULL);
    if (!privkey) {
        ofs.close();    
        ifs.close();
    
        EVP_PKEY_free(privkey);
        fclose(privatekeyfile);
        free(my_ek);
        return false;
    }
    unsigned char ot[2048];  // open text
    unsigned char st[1024];  // sifrovany text

    const char* cipherName = "aes-128-cbc";

    const EVP_CIPHER * cipher;

    OpenSSL_add_all_ciphers();

    cipher = EVP_get_cipherbyname(cipherName);
    if (!cipher) {
        printf("Sifra %s neexistuje.\n", cipherName);
        ofs.close();    
        ifs.close();
    
        EVP_PKEY_free(privkey);
        fclose(privatekeyfile);
        free(my_ek);
        return 1;
    }

    int tmpLength = 0;

    EVP_CIPHER_CTX * ctx; // context structure
    ctx = EVP_CIPHER_CTX_new();
    if (ctx == NULL) {
        ofs.close();    
        ifs.close();
    
        EVP_CIPHER_CTX_free(ctx);
        EVP_PKEY_free(privkey);
        fclose(privatekeyfile);
        free(my_ek);
        return 2;
    }
        
    if (!EVP_OpenInit(ctx, cipher, my_ek, keySize, iv, privkey)) {
        ofs.close();    
        ifs.close();
    
        EVP_CIPHER_CTX_free(ctx);
        EVP_PKEY_free(privkey);
        fclose(privatekeyfile);
        free(my_ek);
        return 6;
    }
        

    int cnt;
    while((cnt = ifs.read((char*) &st, sizeof(st)).gcount()) > 0) {
        if (!EVP_DecryptUpdate(ctx, ot, &tmpLength, st, cnt)) { // desifrovani st
            ofs.close();    
            ifs.close();

            EVP_CIPHER_CTX_free(ctx);
            EVP_PKEY_free(privkey);
            fclose(privatekeyfile);
            free(my_ek);
            return 7;
        }

        ofs.write((char*)&ot, tmpLength);
    }

    if (!EVP_DecryptFinal(ctx, ot, &tmpLength)) { // dokonceni (ziskani zbytku z kontextu)
        ofs.close();    
        ifs.close();

        EVP_CIPHER_CTX_free(ctx);
        EVP_PKEY_free(privkey);
        fclose(privatekeyfile);
        free(my_ek);
        return 8;
    }

    ofs.write((char*)&ot, tmpLength);

    ofs.close();    
    ifs.close();
    EVP_CIPHER_CTX_free(ctx);
    EVP_PKEY_free(privkey);
    fclose(privatekeyfile);
    free(my_ek);
    return 0;
}

int main (int argc, char * argv[]) {
    if (argc != 5) {
        cout << argv[1] << endl;
        printf("Arguments not");
        return 1;
    }

    string operation (argv[1]);
    string key (argv[2]);
    string fileIn (argv[3]);
    string fileOut (argv[4]);

    if (operation == "-e") {
        int val = encrypt(key, operation, fileIn, fileOut);
        return val;
    } else if (operation == "-d") {
        int val = decrypt(key, operation, fileIn, fileOut);
        return val;
    } else {
        printf("Invalid operation");
        return 1;
    }  
}



