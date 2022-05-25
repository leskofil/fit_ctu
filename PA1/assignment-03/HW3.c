#ifndef __PROGTEST__
#include <stdio.h>
#include <assert.h>
#endif /* __PROGTEST__ */

#define CUCKSINDAY 180 

char has30Days (int m) {
  return (m == 4 || m == 6 || m == 9 || m == 11);
}

char has31Days (int m) {
  return (m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12);
}

char isLeapYear (int y) {
  return (((y) % 4 == 0 && ((y) % 100 != 0)) || (((y) % 400 == 0) && (y) % 4000 != 0));
}

long int intervalToDays ( int y, int m, int d, int h, int i ) {
  long int days = 0;
  int j;

  for ( j = 1600; j < y; j++ ) {
    if (isLeapYear(j))
      days = days + 366;
    else
      days = days + 365; 
  }

  for ( j = 1; j < m; j++ ) {
    if (has30Days(j))
      days = days + 30;
    if (has31Days(j))
      days = days + 31;
    if(j == 2 && isLeapYear(y))
      days = days + 29;
    if (j == 2 && !isLeapYear(y))
      days = days + 28;
  } 

  return days + d;
}

int restOfDayCucks (int h, int i) { // do konca dna
    int cucks = 0;
    
    for (int j = h + 1; j < 24; j++) {
      if (j > 12) {
        cucks = cucks + j - 12;
      }
      else if (j == 0)
        cucks = cucks + 12;
      else 
        cucks = cucks + j;
    }

    if (i == 0) {
      if (h > 12) {
        cucks = cucks + h - 12;
      }
      else if (h == 0)
        cucks = cucks + 12;
      else
        cucks = cucks + h;
    }
    cucks += 23 - h;

    if (i <= 30)
      cucks += 1;

  return cucks;
}

int toTheDayCucks (int h, int i) { // od zaciatku dna
    int cucks = 0;
    
    for (int j = 0; j <= h; j++) {
      if (j > 12) {
        cucks = cucks + j - 12;
      }
      else if (j == 0)
        cucks = cucks + 12;
      else 
        cucks = cucks + j;
    }

    cucks += h;

    if (i >= 30)
      cucks += 1;

  return cucks;
}

char validateDate ( int y, int m, int d, int h, int i ) {
  if ( i < 0 || i > 59 || h < 0 || h > 23 || y < 1600 || d < 1 || m < 1 || m > 12)
    return 0;
  if ( has30Days(m) && d > 30 ) {
    return 0; 
  }
  if ( has31Days(m) && d > 31 ) 
    return 0; 
  if (isLeapYear(y) && m == 2 && d > 29)
    return 0;
  if (!isLeapYear(y) && m == 2 && d > 28)
    return 0;

  return 1;  
}

int cuckooClock ( int y1, int m1, int d1, int h1, int i1,
                  int y2, int m2, int d2, int h2, int i2, long long int * cuckoo ) 
{
  if (!validateDate(y1, m1, d1, h1, i1) || !validateDate(y2, m2, d2, h2, i2)) 
    return 0;

  long int daysDiff = intervalToDays(y2, m2, d2, h2, i2) - intervalToDays(y1, m1, d1, h1, i1);;
/*
  if (daysDiff == 1) {
    if (h2 < h1) {
      int bc = restOfDayCucks(h1, i1) + toTheDayCucks(h2, i2); 
      *cuckoo = bc;
      return 1;
    }
  }
*/

  if (daysDiff < 0) 
    return 0; 

  if (daysDiff == 0) {
    if (h2 - h1 < 0) {
      return 0;
    }
    if (h2 == h1) {
      if (i2 < i1)
        return 0; 
    }
  }

  *cuckoo = daysDiff * CUCKSINDAY - 180 + restOfDayCucks (h1, i1) + toTheDayCucks (h2, i2);
  return 1;
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
  long long int cuckoo;

  assert ( cuckooClock ( 2020, 10,  1, 13, 15,
                         2020, 10,  1, 18, 45, &cuckoo ) == 1 && cuckoo == 26 );
  assert ( cuckooClock ( 2020, 10,  1, 13, 15,
                         2020, 10,  2, 11, 20, &cuckoo ) == 1 && cuckoo == 165 );
  assert ( cuckooClock ( 2020,  1,  1, 13, 15,
                         2020, 10,  5, 11, 20, &cuckoo ) == 1 && cuckoo == 50025 );
  assert ( cuckooClock ( 2019,  1,  1, 13, 15,
                         2019, 10,  5, 11, 20, &cuckoo ) == 1 && cuckoo == 49845 );
  assert ( cuckooClock ( 1900,  1,  1, 13, 15,
                         1900, 10,  5, 11, 20, &cuckoo ) == 1 && cuckoo == 49845 );
  assert ( cuckooClock ( 2020, 10,  1,  0,  0,
                         2020, 10,  1, 12,  0, &cuckoo ) == 1 && cuckoo == 102 ); 
  assert ( cuckooClock ( 2020, 10,  1,  0, 15,
                         2020, 10,  1,  0, 25, &cuckoo ) == 1 && cuckoo == 0 );
  assert ( cuckooClock ( 2020, 10,  1, 12,  0,
                         2020, 10,  1, 12,  0, &cuckoo ) == 1 && cuckoo == 12 );
  assert ( cuckooClock ( 2020, 11,  1, 12,  0,
                         2020, 10,  1, 12,  0, &cuckoo ) == 0 );
  assert ( cuckooClock ( 2020, 10, 32, 12,  0,
                         2020, 11, 10, 12,  0, &cuckoo ) == 0 );
  assert ( cuckooClock ( 2100,  2, 29, 12,  0,
                         2100,  2, 29, 12,  0, &cuckoo ) == 0 );
  assert ( cuckooClock ( 2400,  2, 29, 12,  0,
                         2400,  2, 29, 12,  0, &cuckoo ) == 1 && cuckoo == 12 );


  assert ( cuckooClock ( 2400,  2, 29, 12,  0,
                         2400,  2, 29, 12,  30, &cuckoo ) == 1 && cuckoo == 13 );
  return 0;
}
#endif /* __PROGTEST__ */
