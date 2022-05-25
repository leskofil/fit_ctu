#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <forward_list>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <memory>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */


template<typename T_, typename Cmp_ = std::less<T_>>
class CIntervalMin
{
  public:
    using const_iterator = typename vector<T_>::const_iterator;

    explicit CIntervalMin(Cmp_ c = std::less<T_>()) : comparator(move(c)) {};


    CIntervalMin( const_iterator beg, const_iterator end) {
        for(auto i = beg; i < end; i++ ) {
            m_Data.push_back(*i);
        }
    }

    CIntervalMin( const_iterator beg, const_iterator end, Cmp_ c) : comparator(move (c)){
        for(auto i = beg; i < end; i++ ) {
            m_Data.push_back(*i);
        }
    }

    void push_back (T_ src ) {
        m_Data.emplace_back(move(src));
    }

    void pop_back () {
        m_Data.pop_back();
    }

    size_t size () {
        return m_Data.size();
    }


    T_ min (const_iterator st , const_iterator en) {
        T_ min = *min_element(st, en, comparator);
        if (st == en) {
            throw invalid_argument ("inv");
        }
        return min;
    }

        /*
        vector<T_> tmp;
        size_t size = en - st;
        size_t blk = ceil (sqrt(size));
        T_ m = *st;
        for (size_t i = 0; i < size; i++) {
            if (i % blk == 0) {
                if (i > 0)
                    tmp.push_back(m);
                m = *(st + i);
            } else {
                m = std::min ((*(st + i)), m, comparator);
            }
        }
        tmp.push_back(m);

        size_t iB = 0;
        size_t jB = (size - 1) / blk;
        if (iB == jB) {
            return *min_element(st, en, comparator);
        }
        T_ k = *min_element(st, st + blk * (iB + 1), comparator);

        if (jB < iB + 1) {
            k = std::min (m, *min_element(tmp.begin() + blk * (iB + 1), tmp.begin() + blk * jB), comparator);
        }
        if (size > blk * (jB + 1)) {
            k = std::min (m, *min_element(st + blk * (jB + 1), st + size), comparator);
        }
        return k;
    }
         */




    const_iterator begin () {
        return m_Data.begin();
    }

    const_iterator end () {
        return m_Data.end();
    }

  private:
    vector<T_> m_Data;
    Cmp_ comparator;
};


#ifndef __PROGTEST__
//-------------------------------------------------------------------------------------------------
class CStrComparator
{
  public:
               CStrComparator ( bool byLength = true )
      : m_ByLength ( byLength )
    {
    }
    bool       operator () ( const string & a, const string & b ) const
    {
      return m_ByLength ? a . length () < b . length () : a < b;
    }
  private:
    bool       m_ByLength;
};
//-------------------------------------------------------------------------------------------------
bool strCaseCmpFn ( const string & a, const string & b )
{
  return strcasecmp ( a . c_str (), b . c_str () ) < 0;
}
//-------------------------------------------------------------------------------------------------
int main ( void )
{
  CIntervalMin <int> a1;
  for ( auto x : initializer_list<int> { 5, 15, 79, 62, -3, 0, 92, 16, 2, -4 } )
    a1 . push_back ( x );

  assert ( a1 . size () == 10 );

  ostringstream oss;
  for ( auto x : a1 )
    oss << x << ' ';

  assert ( oss . str () == "5 15 79 62 -3 0 92 16 2 -4 " );
  assert ( a1 . min ( a1 . begin (), a1 . end () ) == -4 );
  assert ( a1 . min ( a1 . begin () + 2, a1 . begin () + 3 ) == 79 );
  assert ( a1 . min ( a1 . begin () + 2, a1 . begin () + 9 ) == -3 );

  try
  {
    a1 . min ( a1 . begin (), a1 . begin () );
    assert ( "Missing an exception" == nullptr );
  }
  catch ( const invalid_argument & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception" == nullptr );
  }

  a1 . pop_back ();
  assert ( a1 . size () == 9 );
  a1 . push_back ( 42 );

  assert ( a1 . min ( a1 . begin (), a1 . end () ) == -3 );

  vector<string> words{ "auto", "if", "void", "NULL" };
  CIntervalMin <string> a2 ( words . begin (), words . end () );
  assert ( a2 . min ( a2 . begin (), a2 . end () ) ==  "NULL" );

  CIntervalMin <string, bool(*)(const string &, const string &)> a3 ( words . begin (), words . end (), strCaseCmpFn );
  assert ( a3 . min ( a3 . begin (), a3 . end () ) == "auto" );

  CIntervalMin <string, CStrComparator> a4 ( words . begin (), words . end () );
  assert ( a4 . min ( a4 . begin (), a4 . end () ) == "if" );

  CIntervalMin <string, CStrComparator> a5 ( words . begin (), words . end (), CStrComparator ( false ) );
  assert ( a5 . min ( a5 . begin (), a5 . end () ) == "NULL" );

  CIntervalMin <string, function<bool(const string &, const string &)> > a6 ( [] ( const string & a, const string & b )
  {
    return a > b;
  } );
  for ( const auto & w : words )
    a6 . push_back ( w );
  assert ( a6 . min ( a6 . begin (), a6 . end () ) == "void" );
  return 0;
}
#endif /* __PROGTEST__ */
