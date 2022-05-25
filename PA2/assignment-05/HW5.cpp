#ifndef __PROGTEST__
#include <cassert>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <set>
#include <list>
#include <map>
#include <algorithm>
using namespace std;

class CResult
{
public:
    CResult     ( const string & name,
                  unsigned int   studentID,
                  const string & test,
                  int            result )
            : m_Name ( name ),
              m_StudentID ( studentID ),
              m_Test ( test ),
              m_Result ( result )
    {
    }
    bool           operator == ( const CResult& other ) const
    {
        return m_Name == other . m_Name
               && m_StudentID == other . m_StudentID
               && m_Test == other . m_Test
               && m_Result == other . m_Result;
    }
    string         m_Name;
    unsigned int   m_StudentID;
    string         m_Test;
    int            m_Result;
};
#endif /* __PROGTEST__ */

class CStudent {
public:
    CStudent() = default;

    CStudent(size_t id, string name, const set<string>& cards) : m_ID((id)), m_Name(move (name)) {
        copy(cards.begin(), cards.end(), inserter(m_CardID, m_CardID.begin()) );
    };

    size_t getMId() const {
        return m_ID;
    }

    const string &getMName() const {
        return m_Name;
    }

private:
    size_t m_ID;
    string m_Name;
    set<string> m_CardID;
};

class CExam
{
public:
    static const int SORT_NONE   = 0;
    static const int SORT_ID     = 1;
    static const int SORT_NAME   = 2;
    static const int SORT_RESULT = 3;

    bool           Load        ( istream      & cardMap );
    bool           Register    ( const string & cardID,
                                 const string & test );
    bool           Assess      ( unsigned int   studentID,
                                 const string & test,
                                 int            result );
    list<CResult>  ListTest    ( const string & testName,
                                 int            sortBy ) const;
    set<unsigned int> ListMissing ( const string & testName ) const;

private:
    map<size_t, CStudent> m_Students; //stud, name
    map<string, size_t> m_StudentsCards;

    map<string, set<size_t>> m_Reg;

    map<string, vector<CResult>> m_TestRes;
    map<string, set<size_t>> m_ResStud;


};

bool CExam::Load(istream &cardMap) {
    CStudent s;
    map<size_t, CStudent> m_StudentsI;
    map<string, size_t> cards;
    set<string> c;
    string line;
    string card;
    size_t id;
    string name;
    char c1;
    while (getline(cardMap, line)) {
        istringstream iss(line);
        if (! (iss >> id >> c1 ) || m_Students.find(id) != m_Students.end() || c1 != ':' )
            return false;
        if (!getline ( iss, name,':' ))
            return false;
        while (getline(iss >> ws, card, ',') || getline(iss >> ws, card, '\n')) {
            if (m_StudentsCards.find(card) != m_StudentsCards.end())
                return false;
            cards.insert({card, id});
            c.insert(card);
        }
        m_StudentsI.insert({id, CStudent(id, name, c)});
        c.clear();
    }
    m_Students.insert(m_StudentsI.begin(), m_StudentsI.end());
    m_StudentsCards.insert(cards.begin(), cards.end());
    return true;
}

bool CExam::Register(const string &cardID, const string &test) {
    auto cardIt = m_StudentsCards.find(cardID); //existuje karta?
    if (cardIt == m_StudentsCards.end())
        return false;
    auto regIt = m_Reg.find(test);
    if (regIt != m_Reg.end()) {
        auto studIt = regIt->second.find(cardIt->second); //student uz prihlaseny?
        if (studIt != regIt->second.end())
            return false;
        regIt->second.insert(cardIt->second); //pridat studenta k testu
        return true;
    }
    m_Reg.insert({test, {cardIt->second}}); //novy test
    return true;
}

bool CExam::Assess(unsigned int studentID, const string &test, int result) {
    auto studIt = m_Students.find(studentID); // existuje student?
    if (studIt == m_Students.end())
        return false;
    if (m_ResStud[test].count(studentID) != 0 || m_Reg[test].count(studentID) == 0)
        return false;
     m_TestRes[test].push_back(CResult(studIt->second.getMName(), studIt->second.getMId(), test, result ));
     m_ResStud[test].insert(studentID);
    return true;

    /*
    map<string, set<size_t>> m_Reg;

    map<string, vector<CResult>> m_ResStud;
    map<string, set<size_t>> m_TestRes;
*/
 }

list<CResult> CExam::ListTest(const string &testName, int sortBy) const {
    list<CResult> res;
    auto itRes = m_TestRes.find(testName);
    if (itRes == m_TestRes.end()) {
        return res;
    }
    copy(itRes->second.begin(), itRes->second.end(), back_inserter(res));
    switch (sortBy) {
        case SORT_NAME :
            res . sort ( [] ( const CResult& a, const CResult & b ) { return a . m_Name < b .m_Name; } );
            return res;
        case SORT_ID:
            res . sort ( [] ( const CResult & a, const CResult & b ) { return a . m_StudentID < b .m_StudentID; } );
            return res;
        case SORT_RESULT:
            res . sort ( [] ( const CResult & a, const CResult & b ) { return a . m_Result > b .m_Result; } );
            return res;
        case SORT_NONE:
            return res;
            break;
        default:
            return res;
    }
}

set<unsigned int> CExam::ListMissing(const string &testName) const {
    set<unsigned int> miss;
    auto test1 = m_ResStud.find(testName);
    if (test1 == m_ResStud.end())
        return miss;
    set_difference(m_Reg.find(testName)->second.begin(), m_Reg.find(testName)->second.end(),test1->second.begin(),test1->second.end() ,inserter(miss, miss.end()));
    return miss;


}


#ifndef __PROGTEST__
int main ( void )
{
    istringstream iss;
    CExam         m;
    iss . clear ();
    iss . str ( "123456:Smith John:er34252456hjsd2451451, 1234151asdfe5123416, asdjklfhq3458235\n"
                "654321:Nowak Jane: 62wtsergtsdfg34\n"
                "456789:Nowak Jane: okjer834d34\n"
                "987:West Peter Thomas:sdswertcvsgncse\n" );
    assert ( m . Load ( iss ) );

    assert ( m . Register ( "62wtsergtsdfg34", "PA2 - #1" ) );
    assert ( m . Register ( "62wtsergtsdfg34", "PA2 - #2" ) );
    assert ( m . Register ( "er34252456hjsd2451451", "PA2 - #1" ) );
    assert ( m . Register ( "er34252456hjsd2451451", "PA2 - #3" ) );
    assert ( m . Register ( "sdswertcvsgncse", "PA2 - #1" ) );
    assert ( !m . Register ( "1234151asdfe5123416", "PA2 - #1" ) );
    assert ( !m . Register ( "aaaaaaaaaaaa", "PA2 - #1" ) );
    assert ( m . Assess ( 123456, "PA2 - #1", 50 ) );
    assert ( m . Assess ( 654321, "PA2 - #1", 30 ) );
    assert ( m . Assess ( 654321, "PA2 - #2", 40 ) );
    assert ( m . Assess ( 987, "PA2 - #1", 100 ) );
    assert ( !m . Assess ( 654321, "PA2 - #1", 35 ) );
    assert ( !m . Assess ( 654321, "PA2 - #3", 35 ) );
    assert ( !m . Assess ( 999999, "PA2 - #1", 35 ) );
    assert ( m . ListTest ( "PA2 - #1", CExam::SORT_RESULT ) == (list<CResult>
            {
                    CResult ( "West Peter Thomas", 987, "PA2 - #1", 100 ),
                    CResult ( "Smith John", 123456, "PA2 - #1", 50 ),
                    CResult ( "Nowak Jane", 654321, "PA2 - #1", 30 )
            } ) );
    assert ( m . ListTest ( "PA2 - #1", CExam::SORT_NAME ) == (list<CResult>
            {
                    CResult ( "Nowak Jane", 654321, "PA2 - #1", 30 ),
                    CResult ( "Smith John", 123456, "PA2 - #1", 50 ),
                    CResult ( "West Peter Thomas", 987, "PA2 - #1", 100 )
            } ) );
    assert ( m . ListTest ( "PA2 - #1", CExam::SORT_NONE ) == (list<CResult>
            {
                    CResult ( "Smith John", 123456, "PA2 - #1", 50 ),
                    CResult ( "Nowak Jane", 654321, "PA2 - #1", 30 ),
                    CResult ( "West Peter Thomas", 987, "PA2 - #1", 100 )
            } ) );
    assert ( m . ListTest ( "PA2 - #1", CExam::SORT_ID ) == (list<CResult>
            {
                    CResult ( "West Peter Thomas", 987, "PA2 - #1", 100 ),
                    CResult ( "Smith John", 123456, "PA2 - #1", 50 ),
                    CResult ( "Nowak Jane", 654321, "PA2 - #1", 30 )
            } ) );
    assert ( m . ListMissing ( "PA2 - #3" ) == (set<unsigned int>{ 123456 }) );
    iss . clear ();
    iss . str ( "888:Watson Joe:25234sdfgwer52, 234523uio, asdf234235we, 234234234\n" );
    assert ( m . Load ( iss ) );

    assert ( m . Register ( "234523uio", "PA2 - #1" ) );
    assert ( m . Assess ( 888, "PA2 - #1", 75 ) );
    iss . clear ();
    iss . str ( "555:Gates Bill:ui2345234sdf\n"
                "888:Watson Joe:2345234634\n" );
    assert ( !m . Load ( iss ) );

    assert ( !m . Register ( "ui2345234sdf", "PA2 - #1" ) );
    iss . clear ();
    iss . str ( "555:Gates Bill:ui2345234sdf\n"
                "666:Watson Thomas:okjer834d34\n" );
    assert ( !m . Load ( iss ) );

    assert ( !m . Register ( "ui2345234sdf", "PA2 - #3" ) );
    iss . clear ();
    iss . str ( "555:Gates Bill:ui2345234sdf\n"
                "666:Watson Thomas:jer834d3sdf4\n" );
    assert ( m . Load ( iss ) );

    assert ( m . Register ( "ui2345234sdf", "PA2 - #3" ) );
    assert ( m . ListMissing ( "PA2 - #3" ) == (set<unsigned int>{ 555, 123456 }) );

    return 0;
}
#endif /* __PROGTEST__ */
