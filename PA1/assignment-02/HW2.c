#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>

int isSquareFree(int n) 
{ 
    if (n % 2 == 0) 
       n = n/2; 
   
    if (n % 2 == 0) 
       return 0; 
  
    for (int i = 3; i <= sqrt(n); i = i+2) { 
        if (n % i == 0) { 
           n = n/i; 
 
           if (n % i == 0) 
               return 0; 
        } 
    } 
  
    return 1; 
} 
/*
void sqNumArr(int* sqFreeNums, int order, int maxOrder) {
    if (maxOrder < order) {
        sqFreeNums = (int *)realloc(sqFreeNums, order * sizeof(int));           
        for(int i = maxOrder; i < order; i++) {
            for(int j = sqFreeNums[i - 1] + 1; j = j; j++ ) {
                if(isSquareFree(j)) {
                    sqFreeNums[i] = j;
                    for (int k = 0; k < 4; k++)
                    {
                        printf("%d\n", sqFreeNums[k]);
                    }
                    
                    break;
                }
            }
        }
    }
}
*/

void sqNumArr(int** sqFreeNums, int order, int maxOrder) {
    int lastVal = 0;
    if (maxOrder < order) {
        *sqFreeNums = (int *)realloc(*sqFreeNums, (order + 1) * sizeof(int));           
        for (int i = maxOrder; i < order; i++) {
            if (i == 0)
                lastVal = 1;
            else 
                lastVal = (*sqFreeNums)[i - 1] + 1;
            
            while(!isSquareFree(lastVal)) {
                lastVal++;
            }
            (*sqFreeNums)[i] = lastVal;
        }
    
    }
}


int main ( ) {
    int order, scanRes, maxOrder = 0;
    int* sqFreeNums = (int*)calloc(1, sizeof(int));

    printf("Poradi:\n");

    while (1){
        scanRes = scanf("%d", &order);

        if (scanRes == EOF) {
            free(sqFreeNums);
            return 0;
        }

        if (scanRes != 1 || order < 1) {
            printf("Nespravny vstup.\n");
            free(sqFreeNums);
            return 1;
        }
        sqNumArr(&sqFreeNums, order, maxOrder);

        if(order>maxOrder){
            maxOrder = order;
        }

        printf("= %d\n",sqFreeNums[order - 1]);


    }
}