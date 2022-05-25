#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare (const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );  
}

void printArr(int* arr, int arrElem) {
    for (int i = 0; i < arrElem; i++) {
        printf("%d", arr[i]);
    }
    printf("\n");
}

int loadIn(int* arr) {
    printf("Delky nosniku:\n");
    int temp;
    int arrElem = 0;
    int scanRes;

    while ((scanRes = scanf(" %d", &temp)) != EOF) {
        if (scanRes != 1 || temp < 1 || arrElem >= 9999)
            return 0;
        
        arr[arrElem] = temp;
        arrElem++;
    }

    if(arrElem < 3) 
        return 0;

   // printArr(arr, arrElem);

    return arrElem;
}

int checkTriangle (int* arr) {
    return (arr[0] + arr[1] > arr[2]);
}

int countTriangles (int* arr, int arrElem) {
    qsort (arr, arrElem, sizeof(int), compare);
    //printArr(arr, arrElem);
    int triangle[3];
    long long int cnt = 0;

    for (int i = 0; i < arrElem - 2; i++) {
        triangle[0] = arr[i];

        for (int j = i + 1; j < arrElem - 1; j++) {
            triangle[1] = arr[j];

        
            for(int p = j + 1; p < arrElem; p++) {
                triangle[2] = arr[p];

                /*for (int k=0; k<3; k++) 
                    printf("%d ", triangle[k]); 
                printf("\n"); */

                if (checkTriangle(triangle))
                    cnt++;

                while ((p + 1) < arrElem && (arr[p] == arr[p+1])) { //function
                        p++;
                }
            }
            
            while ((j + 1) < arrElem && (arr[j] == arr[j+1])) { //function
                    j++;
            }
        }
        while ((i + 1) < arrElem && (arr[i] == arr[i+1])) { //function
            i++;
        }

    }

    return cnt;
}

int main () {
    int arr[10000];

    int arrElem = loadIn(arr);

    if (arrElem == 0) {
        printf("Nespravny vstup.\n");
        return 1;
    }

    long long int cnt = countTriangles(arr, arrElem);

    printf("Trojuhelniku: %lld\n", cnt);

    return 1;

}
