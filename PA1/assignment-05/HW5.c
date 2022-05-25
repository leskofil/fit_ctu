#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct RZ {
    char* RZ_ID;
    int cam_ID;
    time_t time;
    char month[4];
    int day;
    int hour;
    int min;
} RZ;

int cmp( const void *left, const void *right ) {
    const RZ *a = ( const RZ *)left;
    const RZ *b = ( const RZ *)right;
    int cmp = strcmp(a->RZ_ID, b->RZ_ID);

    if ( cmp == 0 )
    {
        return a->time - b->time;
    }
    else
    {
        return  cmp;
    }
} 

int intComparator (const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );  
}

void frees(RZ* RZ_list, int RZcnt, char* buffer) {
    for (int i = 0; i < RZcnt; i++) {
        free(RZ_list[i].RZ_ID);
            }
    free(RZ_list);
    free(buffer);  
}

void printList(RZ* RZ_list, int RZcnt) {
    for (int i = 0; i < RZcnt; i++) {
        printf("%s %d %lld %s %d %d %d \n", RZ_list[i].RZ_ID, RZ_list[i].cam_ID, (long long)RZ_list[i].time,
                                RZ_list[i].month,RZ_list[i].day ,RZ_list[i].hour, RZ_list[i].min);
    }
}

char has30Days (int m) {
  return (m == 3 || m == 5 || m == 8 || m == 10);
}

char has31Days (int m) {
  return (m == 0 || m == 2 || m == 4 || m == 6 || m == 7 || m == 9 || m == 11);
}

int monthStringToInt(char* months) {
    char arr[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    for (int i = 0; i < 12; i++) {
        if (strcmp(months, arr[i]) == 0)
            return i;
    }
    return -1;
}

char validateDate(struct tm* time) {
    if ( time->tm_min < 0 || time->tm_min > 59 || time->tm_hour < 0 || time->tm_hour > 23 || time->tm_mon < 0 || time->tm_mon > 11 || time->tm_mday < 1)
        return 0;
    if ( (has30Days(time->tm_mon)) && (time->tm_mday) > 30 ) {
        return 0; 
    }
    if ( has31Days(time->tm_mon) && (time->tm_mday) > 31 ) 
        return 0; 
    if ((time->tm_mon) == 1 && (time->tm_mday) > 28 ) 
        return 0; 
    return 1;
}

int readLine(char** buffer) {
    int sizeOfLine = 8, nChars = 0;
    char c;
    *buffer = (char*) malloc (sizeOfLine * sizeof(char));
 
    while( 1 ){
        c = getchar();

        if(c == '\n') {
            if (nChars != 0)
                return -1;
        }
            
        (*buffer)[nChars] = c;
        nChars++;
 
        if (c == '}' || c == ',')
            break;
    
        if(nChars >= sizeOfLine)
            *buffer = (char*) realloc( *buffer , ((sizeOfLine *= 2) + 1) * sizeof(char));
    }
 
    (*buffer)[nChars] = '\0';
    return nChars;
}


int readInput(RZ** RZ_list) {
    int cap = 10, RZcnt = 0, scanfRes; 
    char del0, del1, del2, del3;
    char * buffer = NULL;
    char buff[1001];
    char month[4];

    *RZ_list = (RZ*)malloc (sizeof(RZ) * cap); //TODO realloc
    

    if (scanf(" %c", &del0 ) != 1 || del0 != '{') { //chech first char
        frees(*RZ_list, RZcnt, buffer);     
        return -1; //call frees
    }  

    do {
        int charCnt = readLine(&buffer);
        if (charCnt == -1) {
            frees(*RZ_list, RZcnt, buffer);
            return -1;
        }
        struct tm time = {0};
        time.tm_year = 2019;

/*
        for(int i = 0; i < charCnt; i++) {
            printf("%c\n", buffer[i]);
        }
*/
        scanfRes = sscanf(buffer," %d %c %1000s %3s %d %d %c %d %c ",
                            &(*RZ_list)[RZcnt].cam_ID, &del1, buff, month, &time.tm_mday,
                            &time.tm_hour, &del2, &time.tm_min, &del3);

        if (scanfRes != 9 || del1 != ':' || del2 != ':' || (del3 != ',' && del3 != '}')) {
            frees(*RZ_list, RZcnt, buffer);
            return -1;
        }

        int monthInt = monthStringToInt(month);
        time.tm_mon = (monthInt);
        if (monthInt == -1) {
            frees(*RZ_list, RZcnt, buffer);  
            return -1;
        }
        
        if (validateDate(&time) == 0) {
            frees(*RZ_list, RZcnt, buffer);   
            return -1;
        }

        (*RZ_list)[RZcnt].time = mktime(&time);


        (*RZ_list)[RZcnt].RZ_ID = (char*) malloc((strlen(buffer) + 1) * sizeof(char));
        strcpy((*RZ_list)[RZcnt].RZ_ID, buff);

        strcpy((*RZ_list)[RZcnt].month, month);        
        (*RZ_list)[RZcnt].day = time.tm_mday;
        (*RZ_list)[RZcnt].hour = time.tm_hour;
        (*RZ_list)[RZcnt].min = time.tm_min;

        RZcnt++;

        if (RZcnt == cap) {
            cap *= 2;
            *RZ_list = (RZ*)realloc(*RZ_list, sizeof(RZ) * cap);
        }
        free(buffer);
                
    } while (del3 == ',');

    return RZcnt;     
}

int searchRzIdDw (RZ* RZlist, char rz[], int start, int end) {
    if(start > end) {
        return start;
    }
        int mid = start + (end - start)/2;

        if (strcmp(RZlist[mid].RZ_ID, rz) >= 0) {
            return searchRzIdDw(RZlist, rz, start, mid - 1); 
        }
        return searchRzIdDw(RZlist, rz, mid + 1, end); 
}

int searchRzIdUp (RZ* RZlist, char rz[], int start, int end) {
    if(start > end) {
        return start;
    }
        int mid = start + (end - start)/2;

        if (strcmp(RZlist[mid].RZ_ID, rz) <= 0) {
            return searchRzIdUp(RZlist, rz, mid + 1, end); 
        }
        return searchRzIdUp(RZlist, rz, start, mid - 1); 
}

int binarySearch(RZ* RZlist, time_t time, int start, int end) 
{ 
    if (end >= start) { 
        int mid = start + (end - start) / 2; 
  
        if (RZlist[mid].time ==  time) 
            return mid; 
  
        if (RZlist[mid].time >= time) 
            return binarySearch(RZlist, time, start, mid - 1); 
  

        return binarySearch(RZlist, time, mid + 1, end); 
    } 
    return -1; 
} 

int smaller(RZ* RZlist, time_t target, int start , int end)  //smaller
{  
    int ans = -1;  
    while (start <= end)  
    {  
        int mid = (start + end) / 2;  
    
        if (RZlist[mid].time >= target)  
        {  
            end = mid - 1;  
        }    
        else 
        {  
            ans = mid;  
            start = mid + 1;  
        }  
    }  
    return ans;  
} 

int greater(RZ* RZlist, time_t target, int start, int end) //greater
{ 
    int ans = -1; 
    while (start <= end) 
    { 
        int mid = (start + end) / 2; 
  
        if (RZlist[mid].time <= target) 
            start = mid + 1; 
   
        else
        { 
            ans = mid; 
            end = mid - 1; 
        } 
    } 
  
    return ans; 
} 

int searchDateDw (RZ* RZlist, time_t time, int start, int end) {
    if(start > end) {
        return start;
    }
        int mid = start + (end - start)/2;

        if (RZlist[mid].time >= time) {
            return searchDateDw(RZlist, time, start, mid - 1); 
        }
        return searchDateDw(RZlist, time, mid + 1, end); 
}

int searchDateUp (RZ* RZlist, time_t time, int start, int end) {
    if(start > end) {
        return start;
    }
        int mid = start + (end - start)/2;

        if (RZlist[mid].time <= time) {
            return searchDateUp(RZlist, time, mid + 1, end); 
        }
        return searchDateUp(RZlist, time, start, mid - 1); 
}

int search(RZ* RZlist, int RZcnt) {
    char del2;
    int scanRes;
    char buff[1001];
    char month[4];
    struct tm time = {0};
    time.tm_year = 2019;

    //printList(RZlist, RZcnt);
    while ((scanRes = scanf(" %1000s %3s %d %d %c %d", buff, month, &time.tm_mday,
                            &time.tm_hour, &del2, &time.tm_min)) != EOF) {

        if (del2 != ':' || scanRes != 6)
            return -1;

        int monthInt = monthStringToInt(month);
        time.tm_mon = monthInt;

        if (!validateDate(&time)) {
            return -1;
        }

        time_t tm = mktime(&time);

        int index1 = searchRzIdDw(RZlist, buff, 0 ,RZcnt - 1);

        if (index1 >= (RZcnt)) {
            printf("> Automobil nenalezen.\n");
        } else if(strcmp(RZlist[index1].RZ_ID, buff) != 0) { //je tam presne
            printf("> Automobil nenalezen.\n");
        } else {
            int index2 = searchRzIdUp(RZlist, buff, index1 ,RZcnt - 1);

            int index3 = binarySearch(RZlist, tm, index1, index2 - 1);
            
            int diff = index2 - index1;
            int* idArr = (int *) malloc(sizeof(int) * diff);
            int* idArr2 = (int *) malloc(sizeof(int) * diff);
            int cnt = 0; 
            int cnt2 = 0;

            if (index3 == -1) {
                int sm = smaller(RZlist, tm, index1, index2 - 1);
                if(sm == -1) {
                    printf("> Predchazejici: N/A\n");
                } else {
                    idArr2[cnt2] = RZlist[sm].cam_ID;
                    cnt2++;
                    for (int i = sm; i > index1; i--) {
                        if (RZlist[i].time == RZlist[i-1].time) {
                            idArr2[cnt2] = RZlist[i-1].cam_ID;
                            cnt2++;
                        } else {
                            break;
                        }
                    }
                    qsort(idArr2, cnt2, sizeof(int), intComparator);
                    printf("> Predchazejici: %s %d %.2d:%.2d, %dx [", RZlist[sm].month, RZlist[sm].day,
                                                           RZlist[sm].hour, RZlist[sm].min, cnt2);

                               
                    for(int i = 0; i < cnt2; i++) {
                        printf("%d", idArr2[i]);
                        if (i != (cnt2 - 1)) {
                            printf(", ");
                        }
                    }
                    printf("]\n");         
                }           


                int gr = greater(RZlist, tm, index1, index2 - 1);
                if(gr == -1) {
                    printf("> Pozdejsi: N/A\n");
                } else {
                    idArr[cnt] = RZlist[gr].cam_ID;
                    cnt++;
                    for (int i = gr; i < index2 - 1; i++) {
                        if (RZlist[i].time == RZlist[i+1].time) {
                            idArr[cnt] = RZlist[i+1].cam_ID;
                            cnt++;
                        } else {
                            break;
                        }
                    }

                    qsort(idArr, cnt, sizeof(int), intComparator);
                    printf("> Pozdejsi: %s %d %.2d:%.2d, %dx [", RZlist[gr].month, RZlist[gr].day,
                                                           RZlist[gr].hour, RZlist[gr].min, cnt);
                                      
                    for(int i = 0; i < cnt; i++) {
                        printf("%d", idArr[i]);
                        if (i != (cnt - 1)) {
                            printf(", ");
                        }
                    }
                    printf("]\n"); 
                }


            } else {
                for (int i = (index3 + 1); i < index2; i++) {
                    if (tm == RZlist[i].time) {
                        idArr[cnt] = RZlist[i].cam_ID;
                        cnt++;
                    } else {
                        break;
                    }
                }

                for (int p = index3; p >= index1; p--) {
                    if (tm == RZlist[p].time) {
                        idArr[cnt] = RZlist[p].cam_ID;
                        cnt++;
                    } else {
                        break;
                    }
                    
                } 
                
                qsort(idArr, cnt, sizeof(int), intComparator);
                printf("> Presne: %s %d %.2d:%.2d, %dx [", RZlist[index3].month, RZlist[index3].day,
                                                        RZlist[index3].hour, RZlist[index3].min, cnt); 
                                                                             
                for(int i = 0; i < cnt; i++) {
                    printf("%d", idArr[i]);
                    if (i != (cnt - 1)) {
                        printf(", ");
                    }
                }
                printf("]\n");
            }
            free(idArr);
            free(idArr2);            
        }
        
    }
    return 1;

}


int main () {
    RZ* RZ_list = NULL;

    printf("Data z kamer:\n");
    int RZcnt = readInput(&RZ_list);
    if (RZcnt == -1) {
        printf("Nespravny vstup.\n");
        return 1;
    }

    qsort(RZ_list, RZcnt, sizeof(RZ), cmp); 
    

    printf("Hledani:\n");
    if (search(RZ_list, RZcnt) == -1) {
        printf("Nespravny vstup.\n");
    }

    for(int i = 0; i < RZcnt; i++) {
        free(RZ_list[i].RZ_ID);
    }
    free(RZ_list);

    return 0;

}
