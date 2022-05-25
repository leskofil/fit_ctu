#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

int main (void) {
    double x1, y1, r1, x2, y2, r2, d, area; 
    double a, b, c;  // del
    printf("Zadejte parametry kruznice #1:\n");

    if ( scanf(" %lf %lf %lf", &x1, &y1, &r1) != 3 || r1 <= 0 ) {
        printf("Nespravny vstup.\n");
        return 1;
    }

    printf("Zadejte parametry kruznice #2:\n");
     
    if ( scanf(" %lf %lf %lf", &x2, &y2, &r2) != 3 || r2 <= 0 ) {
        printf("Nespravny vstup.\n");
        return 1;
    }

    d = sqrt( (a = pow( ( y2 - y1), 2)) + (b = pow( ( x2 - x1), 2)) );

    //printf("%lf\n%lf\n%lf\n", d, a, b);

    if ( fabs(( r1 + r2 ) - d ) < 10000 * DBL_EPSILON * d ) {
        printf("Vnejsi dotyk, zadny prekryv.\n");
        return 0;
    }

    if ( fabs(( r1 - r2 ) - d ) < 10000 * DBL_EPSILON * d ) {
        area = M_PI * r2 * r2;
        printf("Vnitrni dotyk, kruznice #2 lezi uvnitr kruznice #1, prekryv: %lf\n", area);
        return 0;
    }

    if ( fabs(( r2 - r1 ) - d ) < 10000 * DBL_EPSILON * d ) {
        area = M_PI * r1 * r1;
        printf("Vnitrni dotyk, kruznice #1 lezi uvnitr kruznice #2, prekryv: %lf\n", area);
        return 0;
    }


    if ( (r1 + r2) < d ) {
        printf ("Kruznice lezi vne sebe, zadny prekryv.\n");
        return 0;
    }

    if ( (r1 - r2) > d ) {
        area = M_PI * r2 * r2;
        printf("Kruznice #2 lezi uvnitr kruznice #1, prekryv: %lf\n", area);
        return 0;
    }

    if ( (r2 - r1) > d ) {
        area = M_PI * r1 * r1;
        printf("Kruznice #1 lezi uvnitr kruznice #2, prekryv: %lf\n", area);
        return 0;
    }


    if ( ( r1 - r2 ) < d && ( r1 + r2 ) > d ) {

        a = r1 * r1;
        b = r2 * r2;
        c = d * d;       

        area = a * acos( ( (c) + a - b) / (2 * d * r1) ) // resource: https://mathworld.wolfram.com/Circle-CircleIntersection.html?fbclid=IwAR1_peE-HJJn-Yo8XlAiTHp3Hb4h4UR3iNoo8w1pIFV79bc1FyUJA5ZSw-A
             + b * acos((c  + b - a) / (2 * d * r2)) 
             - ( sqrt ( ( -d + r1 + r2 ) * ( d + r1 - r2 ) * ( d - r1 +r2 ) * ( d + r1 + r2 ) ) / 2 );

        printf("Kruznice se protinaji, prekryv: %lf\n", area);
        return 0;

    }



    if ( fabs( ( r1 - r2 + x1 - x2 + y1 - y2) ) < 100 * DBL_EPSILON ) {
        area = M_PI * r1 * r1;
        printf("Kruznice splyvaji, prekryv: %lf\n", area);
        return 0;
    }



    return 0;




}