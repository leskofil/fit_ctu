//Filip Lesko

#include <iostream>
#include <iomanip>
#include <fstream>  // fstream
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <openssl/evp.h>
using namespace std;

#pragma pack(push, r1, 1)
struct THeader {
    uint8_t IDLength;
    uint8_t colorMapType;
    uint8_t pictureType;
    uint16_t colorMapBeg;
    uint16_t colorMapLen;
    uint8_t bitDepth;
    uint8_t PicSpecific[10]; 
};
#pragma pack(pop, r1)


void renameFile (const string & operation, const string & mode, string & file) {
    
    if (operation == "-e") {
        file.resize(file.size() - 4);
        if (mode == "ecb") 
            file.append("_ecb.tga");
        else
            file.append("_cbc.tga");
    } else if (operation == "-d") {
        file.resize(file.size() - 8);
        if (mode == "ecb") 
            file.append("_ecb_dec.tga");
        else
            file.append("_cbc_dec.tga");
    }
}

bool chooseMode (const string& mode, string& cipherName) {
    if (mode == "cbc") {
       cipherName = "aes-128-cbc"; 
        return true;
    } else if (mode == "ecb") {
        cipherName = "aes-128-ecb";
        return true;
    }
    else 
        return false;
}

bool readHead (ifstream & ifs, ofstream & ofs, THeader & h ) {
    if(ifs.fail()) 
        return false;

    if(ifs.read ((char*)&h, sizeof(h)  ).gcount() != (sizeof(h)))
        return false;
        
    ofs.write((char*)&h, sizeof(h) ); //write head
 
    uint8_t tmp;
     for ( uint8_t i = 0; i < h.IDLength; i++ )  {
        if ( ! ifs.read((char*)&tmp, sizeof(tmp)) ) {
            cout << "inv" << endl; 
            return false;
        }
        if ( ! ofs.write((char*)&tmp, sizeof(tmp)) ) {
            cout << "onv" << endl; 
            return false; 
        }
    }
    
    for(size_t i = 0; i < h.colorMapLen * h.bitDepth / 8; ++i ) {
        if (!ifs.read((char*)&tmp, sizeof(tmp)) ) {
            cout << "inv" << endl;
            return false;
        }
        if (!ofs.write((char*)&tmp, sizeof(tmp))) {
            cout << "onv" << endl;
            return false;
        }
    }

    return true;
}

int encrypt(const string & operation, string & mode, string & file) {
    string file1 = file;
    ifstream ifs (file, ios::in | ios::binary);
    if (ifs.fail()) {
        return false;
    }
    renameFile(operation, mode, file);
    ofstream ofs (file, ios::out | ios::binary);
    if (ofs.fail()) {
        return false;
    }

    THeader h;
  
    if (readHead(ifs, ofs, h) == false ) {
        cout << "Invalid input" << endl;
        ofs.close();
        ifs.close();
        return -1;
    }

    unsigned char ot[1024];  // open text
    unsigned char st[2048];  // sifrovany text
    unsigned char key[EVP_MAX_KEY_LENGTH] = "Muj klic";  // klic pro sifrovani
    unsigned char iv[EVP_MAX_IV_LENGTH] = "inicial. vektor";  // inicializacni vektor
    
    string cipheName;

    if (chooseMode(mode, cipheName) == false) 
        return -1;

    const char* cipherName = cipheName.c_str();

    const EVP_CIPHER * cipher;

    OpenSSL_add_all_ciphers();
    /* sifry i hashe by se nahraly pomoci OpenSSL_add_all_algorithms() */
    cipher = EVP_get_cipherbyname(cipherName);
    if (!cipher) {
        printf("Sifra %s neexistuje.\n", cipherName);
        return 1;
    }

    int tmpLength = 0;

    EVP_CIPHER_CTX * ctx; // context structure
    ctx = EVP_CIPHER_CTX_new();
    if (ctx == NULL)
        return 2;

    /* Sifrovani */
    if (!EVP_EncryptInit_ex(ctx, cipher, NULL, key, iv)) // context init - set cipher, key, init vector
        return 3;

    while(ifs.read((char*) &ot, sizeof(ot)).gcount() > 0) {

        if (!EVP_EncryptUpdate(ctx,  st, &tmpLength, ot, ifs.gcount())) // encryption of pt
            return 4;

        ofs.write((char*)&st, tmpLength);
    }

    if (!EVP_EncryptFinal_ex(ctx, st, &tmpLength)) // get the remaining ct
        return 5;


    ofs.write((char*)&st, tmpLength);
    EVP_CIPHER_CTX_free(ctx);
    ofs.close();    
    ifs.close();
    for (auto & c: mode) c = toupper(c);
    cout << file1 << " -> " << "(zašifrujeme v " << mode << " módu) -> " << file << endl;
    return 0;
}

int decrypt(const string & operation, string & mode, string & file) {
    string file1 (file);
    ifstream ifs (file, ios::in | ios::binary);
    renameFile(operation, mode, file);
    ofstream ofs (file, ios::binary);
    THeader h;
  
    if (readHead(ifs, ofs, h) == false ) {
        cout << "Invalid input" << endl;
        ofs.close();
        ifs.close();
        return -1;
    }

    unsigned char ot[2048];  // open text
    unsigned char st[1024];  // sifrovany text
    unsigned char key[EVP_MAX_KEY_LENGTH] = "Muj klic";  // klic pro sifrovani
    unsigned char iv[EVP_MAX_IV_LENGTH] = "inicial. vektor";  // inicializacni vektor
    
    string cipheName;

    if (chooseMode(mode, cipheName) == false) 
        return -1;

    const char* cipherName = cipheName.c_str();

    const EVP_CIPHER * cipher;

    OpenSSL_add_all_ciphers();
    /* sifry i hashe by se nahraly pomoci OpenSSL_add_all_algorithms() */
    cipher = EVP_get_cipherbyname(cipherName);
    if (!cipher) {
        printf("Sifra %s neexistuje.\n", cipherName);
        return 1;
    }

    int tmpLength = 0;

    EVP_CIPHER_CTX * ctx; // context structure
    ctx = EVP_CIPHER_CTX_new();
    if (ctx == NULL)
        return 2;

 /* Desifrovani */
    if (!EVP_DecryptInit_ex(ctx, cipher, NULL, key, iv)) // nastaveni kontextu pro desifrovani
        return 6;

    int cnt;
    while((cnt = ifs.read((char*) &st, sizeof(st)).gcount()) > 0) {
        if (!EVP_DecryptUpdate(ctx, ot, &tmpLength, st, cnt)) { // desifrovani st
            ofs.close();
            ifs.close();
            return 7;
        }

        ofs.write((char*)&ot, tmpLength);

    }

    if (!EVP_DecryptFinal_ex(ctx, ot, &tmpLength)) { // dokonceni (ziskani zbytku z kontextu)
        ofs.close();
        return 8;
    }

    ofs.write((char*)&ot, tmpLength);
    ofs.close();    
    ifs.close();

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    cout << file1 << " -> (dešifrujeme) -> " << file << endl;

    return 0;
}

int main (int argc, char * argv[]) {
    if (argc != 4) {
        cout << argv[1] << endl;
        printf("Arguments not");
        return 1;
    }

    string operation (argv[1]);
    string mode (argv[2]);
    string file (argv[3]);


    if (operation == "-e") {
        int val = encrypt(operation, mode, file);
        return val;
    } else if (operation == "-d") {
        int val = decrypt(operation, mode, file);
        return val;
    } else {
        printf("Invalid operation");
        return 1;
    }  
}



