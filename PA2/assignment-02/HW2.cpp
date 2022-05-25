#ifndef __PROGTEST__
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <string>
#include <memory>
#include <functional>
#include <vector>
#include <algorithm>
using namespace std;
#endif /* __PROGTEST__ */


class CCar {
public:
    CCar(const string & mRz) : m_RZ(mRz) {}
    bool equalRZ (const string & s) const { return m_RZ == s;}
    void printCar() { cout << m_RZ << " ";}
    const string &getMRz() const {return m_RZ;}

    bool operator < (const CCar& c ) const { return m_RZ < c.m_RZ;}
private:
    string m_RZ;
};

class CPerson {
public:
    CPerson(const string & name, const string & secondName, const string & RZ) :
    m_FirstName(name),
    m_SecondName(secondName)
    {
        m_CarList.emplace_back(RZ);
    };
    bool addRZ (const string & rz);
    bool enqualSecondName (const string & n) const { return m_SecondName == n; }
    bool enqualFirstName (const string & n) const { return m_FirstName == n; }

    int countCars() const {return m_CarList.size();};

    const vector<CCar> &getMCarList() const;

    const string &getMFirstName() const;

    const string &getMSecondName() const;

    int deleteRZ(const string& rz) { //transfer
        auto it = lower_bound(m_CarList.begin(), m_CarList.end(), rz);
        if (it != m_CarList.end() && (*it).equalRZ(rz)) {
            m_CarList.erase(it);
            if (m_CarList.empty())  {
              return -1;
            }
            return true;
        }
        return false;
    }


    void printName() {for (CCar c : m_CarList) c.printCar(); cout << m_FirstName <<"  " << m_SecondName <<endl;}

    bool operator < (const CPerson& p) const {return m_SecondName == p.m_SecondName ? m_FirstName < p.m_FirstName : m_SecondName < p.m_SecondName;}
private:
    vector<CCar> m_CarList;
    string m_FirstName;
    string m_SecondName;
};

bool CPerson::addRZ(const string & rz) {
    auto itt = lower_bound(m_CarList.begin(), m_CarList.end(), rz);
    m_CarList.insert(itt, CCar(rz));
    return true;
}

const vector<CCar> &CPerson::getMCarList() const {
    return m_CarList;
}

const string &CPerson::getMFirstName() const {
    return m_FirstName;
}

const string &CPerson::getMSecondName() const {
    return m_SecondName;
}

class CCarList
{
  public:
    CCarList (const vector<CCar> & cars) : m_CarList(cars) {};
    const string & RZ      (  ) const;
    bool           AtEnd   (  ) const;
    void           Next    (  );
  private:
    vector<CCar> m_CarList;
    size_t i = 0;
};

const string &CCarList::RZ() const {
    return  m_CarList.at(i).getMRz();
}

bool CCarList::AtEnd() const {
    return i == m_CarList.size();
}

void CCarList::Next() {
    i++;
}

class CPersonList
{
  public:
    CPersonList (const vector<CPerson>& p) : m_PersonList(p) {}
    const string & Name    ( void ) const;
    const string & Surname ( void ) const;
    bool           AtEnd   ( void ) const;
    void           Next    ( void );
  private:
    size_t i = 0;
    vector<CPerson> m_PersonList;
};

const string &CPersonList::Name(void) const {
    return  m_PersonList.at(i).getMFirstName();
}

const string &CPersonList::Surname(void) const {
    return  m_PersonList.at(i).getMSecondName();
}

bool CPersonList::AtEnd(void) const {
    return i == m_PersonList.size();
}

void CPersonList::Next(void) {
    if (!AtEnd())
    i++;
}

class CRegister
{
  public:
                CRegister  ( void ) = default;
               ~CRegister  ( void ) = default;
                CRegister  ( const CRegister & src ) = delete;
    CRegister & operator = ( const CRegister & src ) = delete;
    bool        AddCar     ( const string & rz,
                             const string & name,
                             const string & surname );
    bool        DelCar     ( const string & rz );
    bool        Transfer   ( const string & rz,
                             const string & nName,
                             const string & nSurname);
    CCarList    ListCars   ( const string & name,
                             const string & surname ) const;
    int         CountCars  ( const string & name,
                             const string & surname ) const;
    CPersonList ListPersons( void ) const;
    void PrintPeople() const {for (CPerson p : m_PersonList) p.printName();}
  private:
    vector<CPerson> m_PersonList;
    vector<string> m_RZList;
};

bool CRegister::AddCar(const string &rz, const string &name, const string &surname) {
    auto itt = lower_bound(m_RZList.begin(), m_RZList.end(), rz);
    if (itt != m_RZList.end() && *itt == rz)
      return false;

    CPerson p(name, surname, rz);
    auto item = lower_bound(m_PersonList.begin(), m_PersonList.end(), p);

    if (item != m_PersonList.end() && item->enqualFirstName(name) && item->enqualSecondName(surname)) {
        int val = item->addRZ(rz);
        m_RZList.insert(itt, rz);
        return val;
    } else {
        m_PersonList.insert(item, p);
        m_RZList.insert(itt, rz);
        return true;
    }

}

bool CRegister::DelCar(const string &rz) {
    auto itt = lower_bound(m_RZList.begin(), m_RZList.end(), rz);
    if (itt == m_RZList.end() || *itt != rz) {
        return false;
    }
        int i = 0;
         for(CPerson& p : m_PersonList) {
            int val = p.deleteRZ(rz);
            if (val == true || val == -1) {
              m_RZList.erase(itt);
                if (val == -1) {
                    auto n = m_PersonList.begin() + i;
                    m_PersonList.erase(n);
                }
                return true;
            }
            i++;
        }
    return false;
}

bool CRegister::Transfer(const string &rz, const string &nName, const string &nSurname) {
    auto itt = lower_bound(m_RZList.begin(), m_RZList.end(), rz);
    if (itt == m_RZList.end() || *itt != rz) {
        return false;
    }
        int i = 0;
        for(CPerson& p : m_PersonList) {
         if (p.enqualSecondName(nSurname) && p.enqualFirstName(nName)) {
           i++;
           continue;
         }
         int val = p.deleteRZ(rz);
         if (val == true || val == -1) {
           m_RZList.erase(itt);
           AddCar(rz, nName, nSurname);
           if (val == -1) {
               auto n = m_PersonList.begin() + i;
               m_PersonList.erase(n);
             }
             return true;
         }
         i++;
        }

    return false;
}

CCarList CRegister::ListCars(const string &name, const string &surname) const {
    CPerson p(name, surname, name);
    auto item = lower_bound(m_PersonList.begin(), m_PersonList.end(), p);

    if (item != m_PersonList.end() && item->enqualSecondName(surname) && item->enqualFirstName(name)) {
        return CCarList (item->getMCarList());
    }
    vector<CCar> v;
    return CCarList(v);
}

int CRegister::CountCars(const string &name, const string &surname) const {
    CPerson p(name, surname, name);
    auto item = lower_bound(m_PersonList.begin(), m_PersonList.end(), p);

    if (item != m_PersonList.end() && item->enqualSecondName(surname) && item->enqualFirstName(name)) {
        return item->countCars();
    }
    return 0;
}

CPersonList CRegister::ListPersons() const {
    return CPersonList(m_PersonList);
}

#ifndef __PROGTEST__
bool checkPerson           ( CRegister    & r,
                             const string & name,
                             const string & surname,
                             vector<string> result )
{
  for ( CCarList l = r . ListCars ( name, surname ); ! l . AtEnd (); l . Next () )
  {
    auto pos = find ( result . begin (), result . end (), l . RZ () );
    if ( pos == result . end () )
      return false;
    result . erase ( pos );
  }
  return result . size () == 0;
}
int main ( void )
{
    CRegister b1;
    assert ( b1 . AddCar ( "ABC-12-34", "John", "Smith" ) == true );
    assert ( b1 . AddCar ( "ABC-32-22", "John", "Hacker" ) == true );
    assert ( b1 . AddCar ( "XYZ-11-22", "Peter", "Smith" ) == true );
    assert ( b1 . CountCars ( "John", "Hacker" ) == 1 );
    assert ( checkPerson ( b1, "John", "Hacker", { "ABC-32-22" } ) );
    assert ( b1 . Transfer ( "XYZ-11-22", "John", "Hacker" ) == true );
    assert ( b1 . AddCar ( "XYZ-99-88", "John", "Smith" ) == true );
    assert ( b1 . CountCars ( "John", "Smith" ) == 2 );
    assert ( checkPerson ( b1, "John", "Smith", { "ABC-12-34", "XYZ-99-88" } ) );
    assert ( b1 . CountCars ( "John", "Hacker" ) == 2 );
    assert ( checkPerson ( b1, "John", "Hacker", { "ABC-32-22", "XYZ-11-22" } ) );
    assert ( b1 . CountCars ( "Peter", "Smith" ) == 0 );
    assert ( checkPerson ( b1, "Peter", "Smith", {  } ) );
    assert ( b1 . Transfer ( "XYZ-11-22", "Jane", "Black" ) == true );
    assert ( b1 . CountCars ( "Jane", "Black" ) == 1 );
    assert ( checkPerson ( b1, "Jane", "Black", { "XYZ-11-22" } ) );
    assert ( b1 . CountCars ( "John", "Smith" ) == 2 );
    assert ( checkPerson ( b1, "John", "Smith", { "ABC-12-34", "XYZ-99-88" } ) );
    assert ( b1 . DelCar ( "XYZ-11-22" ) == true );
    assert ( b1 . CountCars ( "Jane", "Black" ) == 0 );
    assert ( checkPerson ( b1, "Jane", "Black", {  } ) );
    assert ( b1 . AddCar ( "XYZ-11-22", "George", "White" ) == true );
    //assert (b1 . Transfer ( "EYT-27-60-50", "Mgyphqehtkka", "Iehleowbjbrcca" ) == true );
    CPersonList i1 = b1 . ListPersons ();
    assert ( ! i1 . AtEnd () && i1 . Surname () == "Hacker" && i1 . Name () == "John" );
    i1 . Next ();
    assert ( ! i1 . AtEnd () && i1 . Surname () == "Smith" && i1 . Name () == "John" );
    i1 . Next ();
    assert ( ! i1 . AtEnd () && i1 . Surname () == "White" && i1 . Name () == "George" );
    i1 . Next ();
    assert ( i1 . AtEnd () == 1);
    i1 . Next ();
    assert ( i1 . AtEnd () == 1);

    CRegister b2;
    assert ( b2 . AddCar ( "ABC-12-34", "John", "Smith" ) == true );
    assert ( b2 . AddCar ( "ABC-32-22", "John", "Hacker" ) == true );
    assert ( b2 . AddCar ( "XYZ-11-22", "Peter", "Smith" ) == true );
    assert ( b2 . AddCar ( "XYZ-11-22", "Jane", "Black" ) == false );
    assert ( b2 . DelCar ( "AAA-11-11" ) == false );
    assert ( b2 . Transfer ( "BBB-99-99", "John", "Smith" ) == false );
    assert ( b2 . Transfer ( "ABC-12-34", "John", "Smith" ) == false );
    assert ( b2 . CountCars ( "George", "White" ) == 0 );
    assert ( checkPerson ( b2, "George", "White", {  } ) );
    CPersonList i2 = b2 . ListPersons ();
    assert ( ! i2 . AtEnd () && i2 . Surname () == "Hacker" && i2 . Name () == "John" );
    i2 . Next ();
    assert ( ! i2 . AtEnd () && i2 . Surname () == "Smith" && i2 . Name () == "John" );
    assert ( i2 . AtEnd () == 0 );
    i2 . Next ();
    assert ( ! i2 . AtEnd () && i2 . Surname () == "Smith" && i2 . Name () == "Peter" );
    i2 . Next ();
    assert ( i2 . AtEnd () == 1 );
    i2 . Next ();
    assert ( i2 . AtEnd () == 1 );


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




    return 0;
}
#endif /* __PROGTEST__ */