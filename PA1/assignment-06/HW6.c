#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef struct Data {
    char* num;
    char* id;
    char* code;
} DATA;

void freeList(DATA* list, int userCnt) {
    for(int i = 0; i < userCnt; i++) {
        free(list[i].code);
        free(list[i].id);
        free(list[i].num);
    }
    free (list);
}

void printList(DATA* list, int userCnt) {
    for (int i = 0; i < userCnt; i++) {
        printf("%s  %s %s\n", list[i].num, list[i].id, list[i].code);
    }
}

void toUpperCase (char* id, int len) {
    while(*id != '\0') {
        *id = toupper((unsigned char)*id);
        id++;
    }
}

char* idToCode(char* id) { //https://www.geeksforgeeks.org/convert-sentence-equivalent-mobile-numeric-keypad-sequence/
    int idLen = strlen(id);
    char* code = (char*) malloc(idLen * (sizeof(char) + 1));
    char str[26][2] = {"2","2","2", 
                       "3","3","3", 
                       "4","4","4", 
                       "5","5","5", 
                       "6","6","6", 
                       "7","7","7","7", 
                       "8","8","8", 
                       "9","9","9","9"
                      };
    int pos = 0;

    toUpperCase(id, idLen);

    for (int i = 0; i < idLen; i++) {
        if (isalpha(id[i])) {
            int index = id[i] - 'A' + 1;
            code[pos] = str[index - 1][0];
            pos++;
        } else if (isspace(id[i]) || id[i] == '1') {
            code[pos] = '1';
            pos++;
        } else {
            code[pos] = '0';
            pos++;
        }
    }
    code[pos] = '\0';
    //printf("%s", code);
    return code;               
}

int readId (char** id, int listCnt) {
    int cntId = 1;
    int cntData = 0;
    char flag = 0;

    if ((*id)[0] !='"')
        return -1;

    while ((*id)[cntId]) {
        if (flag == 0) {    
            if ((*id)[cntId] == '\\') {
                cntId++;
                flag = 1;
            } else if ((*id)[cntId] == '"' ) {
                break;
            } else {
                (*id)[cntData] = (*id)[cntId];
                cntId++;
                cntData++;
            } 
        } else {
            (*id)[cntData] = (*id)[cntId];
            if ((*id)[cntId + 1] == '\0' && (*id)[cntId] == '"')
                return -1;
            else {
            flag = 0;
            cntId++;
            cntData++;                
            }
        }
    }

    if ((*id)[cntId + 1] != '\0' ) {
        return -1;
    }

    (*id)[cntData] = '\0';

    return 1;

}

int searchUserNum(DATA* list, char* num, int userCnt) {
    for (int i = 0; i < userCnt; i++) {
        if (strcmp(num, list[i].num) == 0) {
            return i;
        }
    }
    return -1;
}

int searchUserCode(DATA* list, char* code, int userCnt, int* index) {
    int cnt = 0;
    for (int i = 0; i < userCnt; i++) {
        if (strcmp(code, list[i].code) == 0 && strcmp(code, list[i].num) != 0) {
            *index = i;
            cnt++;
        }
    }    
    return cnt;
}

void addToList(DATA** list, char* num, char* id, int userCnt) {
    (*list)[userCnt].num = (char*) malloc(strlen(num) * (sizeof(char) + 1));
    (*list)[userCnt].id = (char*) malloc(strlen(id) * (sizeof(char) + 1));
    strcpy((*list)[userCnt].num, num);
    strcpy((*list)[userCnt].id, id);
    (*list)[userCnt].code = idToCode(id);
}

void updateList(DATA** list, char* num, char* id, int index) {
    int len =  strlen(id);
    (*list)[index].id = (char*) realloc((*list)[index].id, len * (sizeof(char) + 1));
    strcpy((*list)[index].id, id);
    free((*list)[index].code);
    (*list)[index].code = idToCode(id);
}

int addUser(DATA* list, char* buffer, int charCnt, int* userCnt) {
    char* id = (char*) malloc(sizeof(char)* charCnt);
    char* num = (char*) malloc(sizeof(char)* charCnt);
    char c, r, x;
    int scanRes;
    int index;

    scanRes = sscanf(buffer,"%c%c %[0-9]%c %[^\n]s", &c, &r, num, &x, id);

    if (buffer[charCnt - 2] != '"' || scanRes != 5 || r != ' ' || x != ' ') {
        free(id);
        free(num);
        return -1;
    }

    //readNum(num, list, userCnt);
    if (readId(&id, *userCnt) == -1) {
       free(id);
       free(num);
       return -1;
   }

    //searchUserId(list, userCnt); 
    if ((index = searchUserNum(list, num, *userCnt)) ==  -1) {
        addToList(&list, num, id, *userCnt);
        printf("NEW\n");
        (*userCnt)++;
    } else {
        updateList(&list, num, id, index);
        printf("UPDATED\n");
    }

   //printList(list, *userCnt);

    free(id);
    free(num);    
    return 1;
}


void removeFromList (DATA** list, int index, int userCnt) {
    free((*list)[index].code);
    free((*list)[index].id);
    free((*list)[index].num);

    for (int j = index; j < (userCnt - 1); j++) {
        (*list)[j] = (*list)[j + 1];
   } 
   
}

int deleteUser(DATA* list, char* buffer, int charCnt, int* userCnt) {
    char* num = (char*) malloc(sizeof(char)* charCnt);
    char c, r;
    int scanRes;
    int n;
    int index;

    scanRes = sscanf(buffer,"%c%c %[0-9]%n", &c, &r, num, &n);

    if (scanRes != 3 || (n+1) != charCnt || r != ' ') {
        free(num);
        return -1;
    }

    if ((index = searchUserNum(list, num, *userCnt)) == -1) {
        printf("NOT FOUND\n");
        //printList(list, *userCnt);
        free(num);
        return 1;
    } else {
        removeFromList(&list, index, *userCnt);
        printf("DELETED\n");
        (*userCnt)--;
        //printList(list, *userCnt);
        free(num);
        return 1;
    } 
}

int findUser(DATA* list, char* buffer, int charCnt, int* userCnt) {
    char* num = (char*) malloc(sizeof(char)* charCnt);
    char c, r;
    int scanRes;
    int n;

    scanRes = sscanf(buffer,"%c%c %[0-9]%n", &c, &r, num, &n);

    if (scanRes != 3 || (n+1) != charCnt || r != ' ') {
        free(num);
        return -1;
    }

    int indexNum = searchUserNum(list, num, *userCnt);
    int indexCode;
    int cntCode = searchUserCode(list, num, *userCnt, &indexCode);

    if (cntCode == 0 && indexNum == -1) {
        printf("NOT FOUND\n");
    } else if (cntCode == 0 && indexNum != -1) {
        printf("FOUND %s (%s)\n", list[indexNum].num, list[indexNum].id);
    } else if (cntCode != 0 && indexNum == -1) {
        if (cntCode == 1) {
            printf("FOUND %s (%s)\n", list[indexCode].num, list[indexCode].id);
        } else {
            printf("AMBIGUOUS (%d matches)\n", cntCode);
        }
    } else {
        printf("AMBIGUOUS (%d matches)\n", (cntCode + 1));
    }

    free(num);
    return 1;
}

int readIdent(void) {
    char* buffer = NULL;
    size_t size;
    int charCnt;
    char ident;
    int cap = 10;
    int userCnt = 0;
    DATA* list = (DATA*) malloc(sizeof(DATA) * cap);

    while ((charCnt = getline(&buffer, &size, stdin)) != EOF) {
        sscanf(buffer, "%c", &ident);

        switch (ident) {
        case '+':
            if (addUser(list, buffer, charCnt, &userCnt) == -1) {
                printf("INVALID COMMAND\n");
                break;
            }
            break;
        case '-':
            if (deleteUser(list, buffer, charCnt, &userCnt) == -1 ) {
                printf("INVALID COMMAND\n");
            }
            break;
        case '?':
            if (findUser(list, buffer, charCnt, &userCnt) == -1 ) {
                printf("INVALID COMMAND\n");
            }
            break;
        default:
                printf("INVALID COMMAND\n");
            break;
        }

        if (userCnt == cap) {
            cap *= 2;
            list = (DATA*)realloc(list, sizeof(DATA) * cap);
        
        }  
    }
    free(buffer);
    freeList(list, userCnt);
    return 0;
}

int main () {    
    printf("PBX configuration (+ = set, - = delete, ? = test, EOF = quit):\n");

    if (readIdent()) {
        return 1; 
    }
}