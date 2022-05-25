#ifndef __PROGTEST__
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <utility>
using namespace std;
#endif /* __PROGTEST__ */

class CStr {
public:
    CStr(const char * str = "");
    ~CStr() noexcept;

    CStr(const CStr & str);
    CStr(CStr && str) noexcept;
    CStr & operator = (CStr str) noexcept;

    bool operator < (const CStr & str) const;
    bool operator == (const CStr & str) const;
    bool operator == (const char * & str) const;
    bool operator != (const CStr & str) const;
    bool operator != (const char * & str) const;

    char * c_str () const {return m_Data;};

    friend ostream & operator << ( ostream & os, const CStr & x );

private:
    char* m_Data;
    size_t m_Length;
    size_t m_Capacity;

    void append(const char * addStr, size_t addLength);

};

void CStr::append(const char *addStr, size_t addLength) {
    if ( m_Length + addLength + 1 > m_Capacity ) {
        while ( m_Length + addLength + 1 > m_Capacity )
            m_Capacity += m_Capacity / 2 + 10;
        char * tmp = new char [m_Capacity];
        memcpy (tmp, m_Data, m_Length);
        memcpy ( tmp + m_Length, addStr, addLength );
        delete [] m_Data;
        m_Data = tmp;
    }
    else
        memcpy ( m_Data + m_Length, addStr, addLength );
    m_Data[m_Length += addLength] = '\0';
}

CStr::CStr(const char *str) : m_Data (nullptr), m_Length(0), m_Capacity(0) {
    append(str, strlen(str));
}

CStr::~CStr() noexcept {
    delete[] m_Data;
    m_Data = nullptr;
}

CStr::CStr(const CStr &str) : m_Data (new char [str.m_Length + 1]),
                              m_Length (str.m_Length),
                              m_Capacity(str.m_Capacity) {
    memcpy(m_Data, str.m_Data, m_Length + 1);
}

CStr::CStr(CStr &&str) noexcept : m_Data (str.m_Data),
                                  m_Length (str.m_Length),
                                  m_Capacity(str.m_Capacity) {
    str.m_Data = nullptr;
}

CStr &CStr::operator=(CStr str) noexcept {
    swap ( m_Data, str.m_Data );
    swap ( m_Length, str.m_Length );
    swap ( m_Capacity, str.m_Capacity );
    return *this;

}

bool CStr::operator==(const CStr &str) const {
    return strcmp(m_Data, str.m_Data) == 0;
}

bool CStr::operator==(const char *&str) const {
    return strcmp(m_Data, str) == 0;
}

bool CStr::operator!=(const CStr &str) const {
    return strcmp(m_Data, str.m_Data) != 0;
}

bool CStr::operator!=(const char *&str) const {
    return strcmp(m_Data, str) != 0;
}

bool CStr::operator<(const CStr &str) const {
    return strcmp(m_Data, str.m_Data) < 0;
}

ostream &operator<<(ostream &os, const CStr &x) {
    os.write(x.m_Data, x.m_Length);
    return os;
}



template<typename T>
class CVector {
public:
    using iterator          = T*;
    using const_iterator    = T const*;
    using riterator          = std::reverse_iterator<iterator>;
    using rconst_iterator    = std::reverse_iterator<const_iterator>;

    explicit CVector(size_t capacity = 20 ) noexcept;
    ~CVector() noexcept;

    CVector(const CVector & v);
    CVector(CVector && v) noexcept;
    CVector<T> & operator = (const CVector& v);
    CVector<T> & operator = (CVector&& v) noexcept;

    T& operator [] (size_t i);
    T const & operator [] (size_t i) const;

    T* begin() const noexcept {return m_Data;};
    T* end() const noexcept {return  m_Data + m_Size;};
    reverse_iterator<T*> rbegin () const { return reverse_iterator<T*> (end()); };
    reverse_iterator<T*> rend () const { return reverse_iterator<T*> (begin()); };


    void insert (const T * iter, const T & add); 
    void insert(const T * iter, T&& val);
    void erase (const T * iter);
    void pushBack(const T & val);
    void pushBack(T && val);
    T at (size_t pos) const;
    void swap(CVector<T>& v);

    size_t size () const {return m_Size;};
    bool empty () const {return m_Size == 0;};
private:
    T* m_Data;
    size_t m_Size;
    size_t m_Capacity;

    void grow();

};

template<typename T>
CVector<T>::CVector(size_t capacity) noexcept :  m_Data (new T[capacity]), m_Size(0), m_Capacity(capacity) {}

template<typename T>
CVector<T>::~CVector() noexcept {
    delete[] m_Data;
    m_Data = nullptr;
}

template<typename T>
CVector<T>::CVector(const CVector<T> &v) : m_Data(new T [v.m_Size]), m_Size(0), m_Capacity(v.m_Size) {
    //copy(v.m_Data, v.m_Data + m_Size, m_Data);
    for(size_t i = 0; i < v.m_Size; ++i) {
        pushBack(v.m_Data[i]);
    }

}

template<typename T>
T & CVector<T>::operator[](size_t i) {
    return m_Data[i];           //TODO add operator to T
}

template<typename T>
T const& CVector<T>::operator[](size_t i) const {
    return m_Data[i];
}

template<typename T>
void CVector<T>::swap(CVector<T> &v) {
    using std::swap;
    swap(m_Data, v.m_Data);
    swap(m_Size, v.m_Size);
    swap(m_Capacity, v.m_Capacity);
}


template<typename T>
CVector<T>::CVector(CVector &&v) noexcept : m_Data(nullptr), m_Size(0), m_Capacity(){
    v.swap(*this);
}

template<typename T>
CVector<T> &CVector<T>::operator=(const CVector& v)  {
    if (this == &v) {
        return *this;
    }
    CVector<T> tmp (v);
    tmp.swap(*this);
    return *this;
}

template<typename T>
CVector<T> &CVector<T>::operator=(CVector &&v) noexcept {
    v.swap(*this);
    return *this;
}

template<typename T>
void CVector<T>::grow() {
    if (m_Size == m_Capacity) {
        m_Capacity += m_Capacity / 2 + 10;
        T *tmp = new T[m_Capacity];
        for ( size_t i = 0; i < m_Size; i++ )
            tmp[i] = std::move ( m_Data[i] );
        delete[] m_Data;
        m_Data = tmp;
    }
}



template<typename T>
void CVector<T>::pushBack(const T &val) {
    grow();
    m_Data[m_Size++] = val; //TODO check
}

template<typename T>
void CVector<T>::pushBack(T &&val) {
    grow();
    m_Data[m_Size++] = std::move(val); //TODO check
}

template<typename T>
void CVector<T>::insert(const T *iter, T &&val) {
    size_t position = iter - m_Data;
    if (position < 0 || position > m_Size) {
        throw out_of_range("Out of range");
    }
    grow();
    m_Size++;
    for (size_t j = m_Size - 1; j > position; j-- )
        m_Data[j] = std::move(m_Data[j - 1]);

    m_Data[position] = move(val);

    //TODO
}


template<typename T>
void CVector<T>::insert(const T *iter, const T &add) {
    size_t position = iter - m_Data;
    if (position < 0 || position > m_Size) {
        throw out_of_range("Out of range");
    }
    grow();
    m_Size++;
    for (size_t j = m_Size - 1; j > position; j-- )
        m_Data[j] = std::move(m_Data[j - 1]);

    m_Data[position] = add;

     //TODO check
}

template<typename T>
void CVector<T>::erase(const T *iter) {
    size_t position = iter - m_Data;

    if (position < 0 || position > m_Size) {
        throw out_of_range("Out of range");
    }
    for (size_t i = position; i < m_Size - 1; i++) {
        m_Data[i] = std::move(m_Data[i+1]);
    }
    m_Size--;
}



template<typename T>
T CVector<T>::at(size_t pos) const {
    if (pos < m_Size)
        return m_Data[pos];
    throw std::out_of_range{ "Out of range"};
}


//////////////////////////////////////////////////////////////////////////////////

class CCar {
    public:
        CCar() = default;
    explicit CCar(CStr  mRz, const CStr& name, const CStr& surname) : m_RZ(std::move(mRz)) {
        pair<CStr, CStr> o (surname, name);
        m_Owners.pushBack(o);
    }
    explicit CCar(CStr mRz) : m_RZ(std::move(mRz)) {}

    CCar(const CCar& c) : m_RZ(c.m_RZ), m_Owners(c.m_Owners) {}

    CCar(CCar&& c) : m_RZ(move(c.m_RZ)), m_Owners(move(c.m_Owners)) {}

    CCar& operator = (CCar c) {
        swap(m_Owners, c.m_Owners);
        swap(m_RZ, c.m_RZ);
        return *this;
    }

    bool operator == (const CStr & s) const { return m_RZ == s;}
    bool operator != (const CStr & s) const { return m_RZ != s;}
    static bool eqTo (const pair<CStr, CStr>& s, const pair<CStr, CStr>& l) {return s.first == l.first && s.second == l.second;};

    void printCar() { cout << m_RZ << " " << endl;}
    void printOw() { for (const pair<CStr, CStr>& c : m_Owners) cout << c.first << c.second << endl;};


    const CVector<pair<CStr, CStr>> &getMOwners() const {
        return m_Owners;
    }

    const CStr &getMRz() const {return m_RZ;};
    bool addOwner (CStr name, CStr surname) {
        pair<CStr, CStr> o (move(surname), move(name));
        m_Owners.pushBack(o);
        return true;
    };

    bool operator < (const CCar& c ) const { return m_RZ < c.m_RZ;}
private:
    CStr m_RZ;
    CVector<pair<CStr, CStr>> m_Owners;
};

///////////////////////////////////////////////////////////////////////////////////
class CPerson {
public:
    CPerson() = default;
    ~CPerson() = default;
    CPerson(CStr name, CStr secondName, CStr RZ) :
            m_FirstName(std::move(name)),
            m_SecondName(std::move(secondName))
    {
        m_CarList.pushBack(move(RZ));
    };

    CPerson (const CPerson & p) : m_CarList(p.m_CarList), m_FirstName(p.m_FirstName), m_SecondName((p.m_SecondName)) { }

    CPerson(CPerson&& p) {
        swap(m_CarList, p.m_CarList);
        swap(m_FirstName, p.m_FirstName);
        swap(m_SecondName, p.m_SecondName);
    }

    CPerson& operator = (CPerson p) {
        swap(m_CarList, p.m_CarList);
        swap(m_FirstName, p.m_FirstName);
        swap(m_SecondName, p.m_SecondName);
        return *this;
    }

    bool operator == (const CPerson & n) const {return m_FirstName == n.m_FirstName &&  m_SecondName == n.m_SecondName;};
    bool operator != (const CPerson & n) const {return m_FirstName != n.m_FirstName ||  m_SecondName != n.m_SecondName;};

    bool addRZ (const CStr & rz);
    bool enqualSecondName (const CStr & n) const { return m_SecondName == n; }
    bool enqualFirstName (const  CStr & n) const { return m_FirstName == n; }
    bool emptyCarList() const {return m_CarList.empty();};

    int countCars() const {return m_CarList.size();};

    const CVector<CStr> &getMCarList() const {return m_CarList;};

    const CStr &getMFirstName() const {return m_FirstName;};

    const CStr &getMSecondName() const {return m_SecondName;};

    int deleteRZ(const CStr& rz) {
        auto it = lower_bound(m_CarList.begin(), m_CarList.end(), rz);
        if (it != m_CarList.end() && (*it) == (rz)) {
            m_CarList.erase(it);
            return true;
        }
        return false;
    }

    void printName() const {cout << m_FirstName <<"  " << m_SecondName <<endl;}

    bool operator < (const CPerson& p) const {return m_SecondName == p.m_SecondName ? m_FirstName < p.m_FirstName : m_SecondName < p.m_SecondName;}

private:
    CVector<CStr> m_CarList;
    CStr m_FirstName;
    CStr m_SecondName;
};

bool CPerson::addRZ(const CStr & rz) {
    auto itt = lower_bound(m_CarList.begin(), m_CarList.end(), rz);
    m_CarList.insert(itt, rz);
    return true;
}

////////////////////////////////////////////////////////

class CCarList
{
  public:
    // copy cons, op=, dtor ...
    explicit CCarList (const CVector<CStr>& cars) : begin(cars.begin()), end(cars.end()), curr(cars.begin()) {};
    ~CCarList() = default;
    CCarList (const CCarList & src);
    CCarList & operator = (const CCarList& src) noexcept;
    const char *RZ           ( ) const;
    bool           AtEnd        ( ) const;
    void           Next         ( );
private:
    CVector<CStr>::const_iterator begin;
    CVector<CStr>::const_iterator end;
    CVector<CStr>::const_iterator curr;
    size_t i = 0;
};

const char * CCarList::RZ() const {
    return curr->c_str();
}

bool CCarList::AtEnd() const {
    return curr == end;
}

void CCarList::Next() {
    advance(curr, 1);
    i++;
}

CCarList::CCarList(const CCarList &src) : begin(src.begin), end(src.end), curr(src.begin) {}

CCarList &CCarList::operator=(const CCarList& src) noexcept = default;

//////////////////////////////////////////////////////////////////////////////

class COwnerList
{
  public:
    // copy cons, op=, dtor ...
    explicit COwnerList (const CVector<pair<CStr, CStr>>& p) : begin(p.rbegin()), end(p.rend()), curr(p.rbegin()) {}
    ~COwnerList() = default;
    COwnerList (const COwnerList & src) : begin(src.begin), end(src.end), curr(src.begin) {};
    COwnerList & operator = (const COwnerList& src);
    const char   * Name         (  ) const;
    const char   * Surname      (  ) const;
    bool           AtEnd        (  ) const;
    void           Next         (  );
  private:
    CVector<pair<CStr, CStr>>::riterator begin;
    CVector<pair<CStr, CStr>>::riterator end;
    CVector<pair<CStr, CStr>>::riterator curr;
    size_t i = 0;
};

COwnerList &COwnerList::operator=(const COwnerList& src) = default;

const char   * COwnerList::Name() const {
    return curr->second.c_str();
}

const char   * COwnerList::Surname() const {
    return  curr->first.c_str();
}

bool COwnerList::AtEnd() const {
   return curr == end;
}

void COwnerList::Next() {
    advance(curr, 1);
    i++;
}


class CRegister
{
  public:
    // copy cons, op=, dtor ...
    CRegister () = default;
    ~CRegister() = default;
    CRegister (const CRegister & src);
    CRegister (CRegister && src) noexcept;
    CRegister & operator = (CRegister src);
    bool           AddCar       ( const CStr& rz,
                                  const CStr& name,
                                  const CStr& surname );
    bool           DelCar       ( const CStr& rz );
    bool           Transfer     ( const CStr& rz,
                                  const CStr& nName,
                                  const CStr& nSurname );
    CCarList       ListCars     ( const CStr& name,
                                  const CStr& surname ) const;
    int            CountCars    ( const CStr& name,
                                  const CStr& surname ) const;
    COwnerList     ListOwners   ( const CStr& RZ ) const;
    int            CountOwners  ( const CStr& RZ ) const;
  private:
     CVector<CPerson> m_PersonList;
     CVector<CCar> m_CarList;

    void PrintPeople() const {
        for (size_t i = 0; m_PersonList.size() > i; i++) m_PersonList[i].printName();
        for (CCar c : m_CarList)
            c.printCar();
    }

};

CRegister::CRegister(const CRegister &src) : m_PersonList(src.m_PersonList), m_CarList(src.m_CarList) {
}

CRegister &CRegister::operator=(CRegister src) {
    swap(m_PersonList, src.m_PersonList);
    swap(m_CarList, src.m_CarList);
    return *this;
}

CRegister::CRegister(CRegister &&src) noexcept {
    swap(m_PersonList, src.m_PersonList);
    swap(m_CarList, src.m_CarList);
}

bool CRegister::AddCar(const CStr& rz, const CStr& name, const CStr& surname) {
   //PrintPeople();
    CCar c(rz, name, surname);
    auto itt = lower_bound(m_CarList.begin(), m_CarList.end(), c);
    if (itt != m_CarList.end() && *itt == rz)
        return false;

    CPerson p(name, surname, rz);

    auto item = lower_bound(m_PersonList.begin(), m_PersonList.end(), p);
    if (item != m_PersonList.end() && (*item) == p) {
        int val = item->addRZ(rz);
        m_CarList.insert(itt, c);
        return val;
    } else {
        m_PersonList.insert(item, p);
        m_CarList.insert(itt, c);
        return true;
    }
}

bool CRegister::DelCar(const CStr& rz) {
    CCar c(rz);
    //PrintPeople();
    auto itt = lower_bound(m_CarList.begin(), m_CarList.end(), c);
    if (itt == m_CarList.end() || *itt != rz) {
        return false;
    }

    int i = 0;
    for(CPerson& p : m_PersonList) {
        int val = p.deleteRZ(rz);
        if (val == true) {
            m_CarList.erase(itt);
            if (p.emptyCarList()) {
                auto n = m_PersonList.begin() + i;
                m_PersonList.erase(n);
            }
           // PrintPeople();
            return true;
        }
        i++;
    }
    return false;
}

bool CRegister::Transfer(const CStr& rz, const CStr& nName, const CStr& nSurname) {
    //PrintPeople();
    CCar c(rz);
    auto itt = lower_bound(m_CarList.begin(), m_CarList.end(), c);
    if (itt == m_CarList.end() || *itt != rz) {
        return false;
    }
    int i = 0;
    for(CPerson& p : m_PersonList) {
        if (p.enqualSecondName(nSurname) && p.enqualFirstName(nName)) {
            i++;
            continue;
        }
        int val = p.deleteRZ(rz);
        if (p.emptyCarList()) {
            auto n = m_PersonList.begin() + i;
            m_PersonList.erase(n);
        }
        if (val == true ) {
            CPerson l(nName, nSurname, rz);
            auto it = lower_bound(m_PersonList.begin(), m_PersonList.end(), l);
            if (it != m_PersonList.end() && (*it) == l) {
                int vale = it->addRZ(rz);
                itt->addOwner(l.getMFirstName(), l.getMSecondName());
                return vale;
            } else {
                itt->addOwner(l.getMFirstName(), l.getMSecondName());
                m_PersonList.insert(it, l);
                //PrintPeople();
                return true;
            }
        }
        i++;
    }
    return false;
}

CCarList CRegister::ListCars(const CStr&name, const CStr&surname) const {
    CPerson p(name, surname, name);
    auto item = lower_bound(m_PersonList.begin(), m_PersonList.end(), p);

    if (item == m_PersonList.end() || (*item) != p) {
        CVector<CStr> v;
        return CCarList(v);
    }
    return CCarList (item->getMCarList());
    
}

int CRegister::CountCars(const CStr&name, const CStr&surname) const {
    CPerson p(name, surname, name);
    auto item = lower_bound(m_PersonList.begin(), m_PersonList.end(), p);

    if (item == m_PersonList.end() || ((*item) != p)) {
        return 0;
    }
    return item->countCars();;
}

COwnerList CRegister::ListOwners(const CStr &RZ) const {
    //PrintPeople();
    CCar c(RZ);
    auto itt = lower_bound(m_CarList.begin(), m_CarList.end(), c);
    if (itt == m_CarList.end() || (*itt) != RZ) {
        //itt->printOw();
        CVector<pair<CStr, CStr>> v;
        return COwnerList(v);
    }
    return COwnerList(itt->getMOwners());
}

int CRegister::CountOwners(const CStr &RZ) const {
    CCar c(RZ);
    auto itt = lower_bound(m_CarList.begin(), m_CarList.end(), c);
    if (itt == m_CarList.end() || (*itt) != RZ) {
        return 0;
    }
    return itt->getMOwners().size();
}


#ifndef __PROGTEST__
static bool        matchList ( CCarList          && l,
                               const char         * rz1 = nullptr,
                               const char         * rz2 = nullptr )
{
  for ( ; ! l . AtEnd (); l . Next () )
    if ( rz1 && ! strcmp ( l . RZ (), rz1 ) )
      rz1 = nullptr;
    else if ( rz2 && ! strcmp ( l . RZ (), rz2 ) )
      rz2 = nullptr;
    else
      return false;
  return rz1 == nullptr && rz2 == nullptr;
}

int main ( void )
{


CRegister b1;
    assert ( b1 . AddCar ( "ABC-12-34", "John", "Smith" ) == true );
    assert ( b1 . AddCar ( "ABC-32-22", "John", "Hacker" ) == true );
    assert ( b1 . AddCar ( "XYZ-11-22", "Peter", "Smith" ) == true );
    assert ( b1 . DelCar ( "ABC-12-34" ) == true );
    assert ( b1 . AddCar ( "ABC-12-34", "John", "Smith" ) == true );
    assert ( b1 . CountCars ( "John", "Hacker" ) == 1 );
    assert ( b1 . CountCars ( "John", "Hacker" ) == 1 );
    assert ( b1 . CountCars ( "Peter", "Smith" ) == 1 );
    

    assert ( b1 . Transfer ( "XYZ-11-22", "John", "Hacker" ) == true );
    assert ( b1 . AddCar ( "XYZ-99-88", "John", "Smith" ) == true );
    assert ( b1 . CountCars ( "John", "Smith" ) == 2 );
   
    assert ( b1 . CountCars ( "John", "Hacker" ) == 2 );
   // assert ( checkPerson ( b1, "John", "Hacker", { "ABC-32-22", "XYZ-11-22" } ) );
    assert ( b1 . CountCars ( "Peter", "Smith" ) == 0 );
    //assert ( checkPerson ( b1, "Peter", "Smith", {  } ) );
    assert ( b1 . Transfer ( "XYZ-11-22", "Jane", "Black" ) == true );
    assert ( b1 . CountCars ( "Jane", "Black" ) == 1 );
   // assert ( checkPerson ( b1, "Jane", "Black", { "XYZ-11-22" } ) );
    assert ( b1 . CountCars ( "John", "Smith" ) == 2 );
   // assert ( checkPerson ( b1, "John", "Smith", { "ABC-12-34", "XYZ-99-88" } ) );
    assert ( b1 . DelCar ( "XYZ-11-22" ) == true );
    assert ( b1 . CountCars ( "Jane", "Black" ) == 0 );
    //assert ( checkPerson ( b1, "Jane", "Black", {  } ) );
    assert ( b1 . AddCar ( "XYZ-11-22", "George", "White" ) == true );
    //assert (b1 . Transfer ( "EYT-27-60-50", "Mgyphqehtkka", "Iehleowbjbrcca" ) == true );
    

    CRegister b2;
    assert ( b2 . AddCar ( "ABC-12-34", "John", "Smith" ) == true );
    assert ( b2 . AddCar ( "ABC-32-22", "John", "Hacker" ) == true );
    assert ( b2 . AddCar ( "XYZ-11-22", "Peter", "Smith" ) == true );
    assert ( b2 . AddCar ( "XYZ-11-22", "Jane", "Black" ) == false );
    assert ( b2 . DelCar ( "AAA-11-11" ) == false );
    assert ( b2 . Transfer ( "BBB-99-99", "John", "Smith" ) == false );
    assert ( b2 . Transfer ( "ABC-12-34", "John", "Smith" ) == false );
    assert ( b2 . CountCars ( "George", "White" ) == 0 );
   // assert ( checkPerson ( b2, "George", "White", {  } ) );
    

    CRegister b3;
    assert ( b3 . AddCar ( "ABC-12-34", "Aohn", "Cmith" ) == true );
    assert ( b3 . AddCar ( "ABC-32-22", "Aohn", "Cmith" ) == true );
    assert ( b3 . AddCar ( "aXYZ-11-22", "Aohn", "Cmith" ) == true );
     assert ( b3 . AddCar ( "sPYZ-11-22", "Aohn", "Cmith" ) == true );
     assert ( b3 . AddCar ( "dfPYZ-11-22", "Aohn", "Cmith" ) == true );
     assert ( b3 . AddCar ( "AgBC-12-34", "Aohn", "Cmith" ) == true );
    assert ( b3 . AddCar ( "ArBC-32-22", "Aohn", "Cmith" ) == true );
    assert ( b3 . AddCar ( "XhYerwZ-11-22", "Aohn", "Cmith" ) == true );
     assert ( b3 . AddCar ( "PYerwZ-11-22", "Aohn", "Cmith" ) == true );
     assert ( b3 . AddCar ( "PYrewZ-11-22", "Aohn", "Cmith" ) == true );
  
  assert ( b3 . AddCar ( "ABC-1tr2-34", "Aohn", "Cmith" ) == true );
    assert ( b3 . AddCar ( "ABC-rt32-22", "Aohn", "Cmith" ) == true );
    assert ( b3 . AddCar ( "X34YgfggZ-11-22", "Aohn", "Cmith" ) == true );
     assert ( b3 . AddCar ( "PtrYZ-11-22", "Aohn", "Cmith" ) == true );
     assert ( b3 . AddCar ( "PYtrZ-11-22", "Aohn", "Cmith" ) == true );
  
  assert ( b3 . AddCar ( "ABC-12cx-34", "Aohn", "Cmith" ) == true );
    assert ( b3 . AddCar ( "ewABC-32-22", "Aohn", "Cmith" ) == true );
    assert ( b3 . AddCar ( "XYZqw-11-22", "Aohn", "Cmith" ) == true );
     assert ( b3 . AddCar ( "PYZrt-11-22", "Aohn", "Cmith" ) == true );
     assert ( b3 . AddCar ( "PYertZ-11-22", "Aohn", "Cmith" ) == true );
  
  assert ( b3 . AddCar ( "ABC-12-df34", "Aohn", "Cmith" ) == true );
    assert ( b3 . AddCar ( "ABC-3cvb2-22", "Aohn", "Cmith" ) == true );
    assert ( b3 . AddCar ( "XYZ-1bn1-22", "Aohn", "Cmith" ) == true );
     assert ( b3 . AddCar ( "PYZ-11-22", "Aohn", "Cmith" ) == true );
     assert ( b3 . AddCar ( "PYZ -11-22", "Aohn", "Cmith" ) == true );
  
  assert ( b3 . AddCar ( "ABC-1dfs2-34", "Aohn", "Cmith" ) == true );
    assert ( b3 . AddCar ( "ABC-p;32-22", "Aohn", "Cmith" ) == true );
    assert ( b3 . AddCar ( "XYZ-9911-22", "Aohn", "Cmith" ) == true );
     assert ( b3 . AddCar ( "PYZpo-11-22", "Aohn", "Cmith" ) == true );
     assert ( b3 . AddCar ( "PuyYZ-11-22", "Aohn", "Cmith" ) == true );
  
  assert ( b3 . AddCar ( "ABC-12ws-34", "Aohn", "Cmith" ) == true );
    assert ( b3 . AddCar ( "ABC-32dx-22", "Aohn", "Cmith" ) == true );
    assert ( b3 . AddCar ( "XYZ-11-2dc2", "Aohn", "Cmith" ) == true );
     assert ( b3 . AddCar ( "edPYZ-11-22", "Aohn", "Cmith" ) == true );
     assert ( b3 . AddCar ( "PYtfgvZ-11-22", "Aohn", "Cmith" ) == true );
  assert ( b3 . AddCar ( "ABC-12-34yg", "Aohn", "Cmith" ) == true );
    assert ( b3 . AddCar ( "ABC-32-yhb22", "Aohn", "Cmith" ) == true );
    assert ( b3 . AddCar ( "XYZ-11-2322", "Aohn", "Cmith" ) == true );
     assert ( b3 . AddCar ( "PYZ-11-243242", "Aohn", "Cmith" ) == true );
     assert ( b3 . AddCar ( "PYZ-11-jhgfc22","Aohn", "Cmith" ) == true );
  
/*

  char name[50], surname[50];
  CRegister  b0;
  assert ( b0 . AddCar ( "ABC-12-34", "John", "Smith" ) == true );
  strncpy ( name, "John", sizeof ( name ) );
  strncpy ( surname, "Hacker", sizeof ( surname ) );
  assert ( b0 . AddCar ( "ABC-32-22", name, surname ) == true );
  strncpy ( name, "Peter", sizeof ( name ) );
  strncpy ( surname, "Smith", sizeof ( surname ) );
  assert ( b0 . AddCar ( "XYZ-11-22", name, surname ) == true );
  assert ( b0 . CountCars ( "John", "Hacker" ) == 1 );
  assert ( matchList ( b0 . ListCars ( "John", "Hacker" ), "ABC-32-22" ) );
  assert ( b0 . CountOwners ( "ABC-12-34" ) == 1 );
  COwnerList ol0 = b0 . ListOwners ( "ABC-12-34" );
  assert ( ! ol0 . AtEnd () && ! strcmp ( ol0 . Name (), "John" ) && ! strcmp ( ol0 . Surname (), "Smith" ) );
  ol0 . Next ();

  assert ( ol0 . AtEnd () );
  CRegister b1 ( b0 );
  assert ( b0 . Transfer ( "XYZ-11-22", "John", "Hacker" ) == true );
  assert ( b0 . Transfer ( "XYZ-11-22", "Will", "Smith" ) == true );
  assert ( b1 . Transfer ( "XYZ-11-22", "John", "Smith" ) == true );
  assert ( b0 . CountOwners ( "XYZ-11-22" ) == 3 );
  COwnerList ol1 = b0 . ListOwners ( "XYZ-11-22" );
  assert ( ! ol1 . AtEnd () && ! strcmp ( ol1 . Name (), "Will" ) && ! strcmp ( ol1 . Surname (), "Smith" ) );
  ol1 . Next ();
  assert ( ! ol1 . AtEnd () && ! strcmp ( ol1 . Name (), "John" ) && ! strcmp ( ol1 . Surname (), "Hacker" ) );
  ol1 . Next ();
  assert ( ! ol1 . AtEnd () && ! strcmp ( ol1 . Name (), "Peter" ) && ! strcmp ( ol1 . Surname (), "Smith" ) );
  ol1 . Next ();
  assert ( ol1 . AtEnd () );
  assert ( b1 . CountOwners ( "XYZ-11-22" ) == 2 );
  COwnerList ol2 = b1 . ListOwners ( "XYZ-11-22" );
  assert ( ! ol2 . AtEnd () && ! strcmp ( ol2 . Name (), "John" ) && ! strcmp ( ol2 . Surname (), "Smith" ) );
  ol2 . Next ();
  assert ( ! ol2 . AtEnd () && ! strcmp ( ol2 . Name (), "Peter" ) && ! strcmp ( ol2 . Surname (), "Smith" ) );
  ol2 . Next ();
  assert ( ol2 . AtEnd () );
  b1 = b0;
  assert ( b0 . DelCar ( "XYZ-11-22" ) == true );
  assert ( b1 . DelCar ( "ABC-12-34" ) == true );
  assert ( b0 . CountCars ( "John", "Smith" ) == 1 );
  assert ( matchList ( b0 . ListCars ( "John", "Smith" ), "ABC-12-34" ) );
  assert ( b1 . CountCars ( "John", "Smith" ) == 0 );
  assert ( matchList ( b1 . ListCars ( "John", "Smith" ) ) );

  CRegister  b2;
  assert ( b2 . AddCar ( "ABC-12-34", "John", "Smith" ) == true );
  assert ( b2 . AddCar ( "ABC-32-22", "John", "Hacker" ) == true );
  assert ( b2 . AddCar ( "XYZ-11-22", "Peter", "Smith" ) == true );
  assert ( b2 . AddCar ( "XYZ-11-22", "Jane", "Black" ) == false );
  assert ( b2 . DelCar ( "AAA-11-11" ) == false );
  assert ( b2 . DelCar ( "XYZ-11-22" ) == true );
  assert ( b2 . Transfer ( "BBB-99-99", "John", "Smith" ) == false );
  assert ( b2 . Transfer ( "ABC-12-34", "John", "Smith" ) == false );
  assert ( b2 . Transfer ( "XYZ-11-22", "John", "Smith" ) == false );
  assert ( b2 . CountCars ( "George", "White" ) == 0 );
  assert ( matchList ( b2 . ListCars ( "George", "White" ) ) );
  assert ( b2 . CountOwners ( "AAA-AA-AA" ) == 0 );
  COwnerList ol3 = b2 . ListOwners ( "AAA-AA-AA" );
  assert ( ol3 . AtEnd () );
*/
  return 0;
}
#endif /* __PROGTEST__ */