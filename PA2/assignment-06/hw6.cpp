#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <memory>
#include <functional>
#include <stdexcept>
#include "ipaddress.h"
using namespace std;
#endif /* __PROGTEST__ */

class CRec {
public:
    explicit CRec(string name) : m_Name(std::move(name)) {};
    virtual ~CRec() = default;
    virtual string Name () const {return m_Name;};
    virtual void Print (ostream& os) const = 0;
    virtual string Type () const = 0;
    virtual bool isEq (const CRec& rec) const = 0;
    virtual CRec* Clone() const = 0;

    bool operator == (const CRec& rec) const {
        return isEq(rec);
    }
    friend ostream & operator << (ostream & os, const CRec& rec) {
        rec.Print(os);
        return os;
    }

protected:
    string m_Name;
};

class CRecA : public CRec
{
public:
    CRecA(string name, const CIPv4& ip) : CRec(std::move(name)) ,m_IPv4(ip) {};
    virtual ~CRecA() = default;
    string Type () const override {return "A";};
    //string Name () const override {return m_Name;};
    bool isEq (const CRec& rec) const override {
        const CRecA* a = dynamic_cast<const CRecA*> (&rec);
        return a!= nullptr && m_Name == a->m_Name && m_IPv4 == a->m_IPv4;
    }
    CRec* Clone () const override {return new CRecA(*this);};
    void Print(ostream &os) const override {
        os <<  m_Name << " " << Type() << " " << m_IPv4;
    }

private:
    CIPv4 m_IPv4;
};

class CRecAAAA : public CRec
{
public:
    CRecAAAA(string name, const CIPv6& ip) :CRec(std::move(name)), m_IPv6(ip) {}
    virtual ~CRecAAAA() = default;
    string Type () const override {return "AAAA";};
    //string Name () const override {return m_Name;};
    bool isEq (const CRec& rec) const override {
        const CRecAAAA* a = dynamic_cast<const CRecAAAA*> (&rec);
        return a!= nullptr && m_Name == a->m_Name && m_IPv6 == a->m_IPv6;
    }
    CRec* Clone () const override {return new CRecAAAA(*this);};
    void Print(ostream &os) const override {
        os <<  m_Name << " " << Type() << " " << m_IPv6;
    }

private:
    CIPv6 m_IPv6;
};

class CRecMX : public CRec
{
public:
    CRecMX(string name, string ip, size_t prior) : CRec(std::move(name)), m_Mail(move(ip)), m_Prior(prior) {}
    virtual ~CRecMX() = default;
    string Type () const override {return "MX";};
    //string Name () const override {return m_Name;};
    bool isEq (const CRec& rec) const override {
        const CRecMX* a = dynamic_cast<const CRecMX*> (&rec);
        return a!= nullptr && m_Name == a->m_Name && m_Mail == a->m_Mail && a->m_Prior == m_Prior;
    }
    CRec* Clone () const override {return new CRecMX(*this);};
    void Print(ostream &os) const override {
        os << m_Name << " " << Type() << " " << m_Prior << " " << m_Mail;
    }

private:
    string m_Mail;
    size_t m_Prior;
};

class CSearcher {
public:
    CSearcher() = default;

    CSearcher(string zone, const vector<shared_ptr<CRec>>& r) : m_Zone(std::move(zone)), m_Rec(r){
    }

    ~CSearcher() = default;

    size_t Count() const {
        return m_Rec.size();
    }

    CRec &operator[](size_t z) const {
        if (z > m_Rec.size() || z < 0 || m_Rec.size() == 0)
            throw out_of_range("Out");
        return *m_Rec[z];
    }

    friend ostream &operator<<(ostream &os, const CSearcher &z) {
        for (const auto &x : z.m_Rec) {
            os << *x << '\n';
        }
        return os;
    }
protected:
    string m_Zone;
    vector<shared_ptr<CRec>> m_Rec;
};

class CZone
{
public:
    explicit CZone(string zone) : m_ZoneName(move(zone)) {};

    ~CZone() = default;

    bool Add(const CRec& rec) {
        auto itS = m_RecSearch.find(rec.Name());
        if (itS != m_RecSearch.end()) {
            auto it = find_if(itS->second.begin(), itS->second.end(), [&](const shared_ptr<CRec>& a) { return *a == rec;});
            if (it != itS->second.end())
                return false;
        }
        m_RecSearch[rec.Name()].push_back(shared_ptr<CRec>(rec.Clone()));
        m_Rec.push_back(shared_ptr<CRec>(rec.Clone()));
        return true;
    }

    bool Del(const CRec& rec) {
        auto itS = m_RecSearch.find(rec.Name());
        if (itS == m_RecSearch.end())
            return false;
        auto it = find_if(itS->second.begin(), itS->second.end(), [&rec] (const shared_ptr<CRec>& a) {return *a == rec;});
        if (it == itS->second.end())
            return false;
        m_RecSearch.find(rec.Name())->second.erase(it);
        auto itV = find_if(m_Rec.begin(), m_Rec.end(), [&rec] (const shared_ptr<CRec>& a) {return *a == rec;});
        m_Rec.erase(itV);
        return true;
    }

    CSearcher Search (const string& str) const {
        auto it = m_RecSearch.find(str);
        if (it == m_RecSearch.end())
            return CSearcher();
        return CSearcher (m_ZoneName, it->second);
    }

    friend ostream& operator << (ostream& os, const CZone& z) {
        os << z.m_ZoneName << endl;
        for (const auto &x : z.m_Rec) {
            if (z.m_Rec.back() != x)
                os << " +- ";
            else
                os << " \\- ";
            os << *x << '\n';
            }
        return os;
    }

    const string &getMZoneName() const {
        return m_ZoneName;
    }

protected:
    string m_ZoneName;
    map<string, vector<shared_ptr<CRec>>> m_RecSearch; //vector
    vector<shared_ptr<CRec>> m_Rec;
};

#ifndef __PROGTEST__
int main ( void )
{
    ostringstream oss;

    CZone z0 ( "fit.cvut.cz" );
    assert ( z0 . Add ( CRecA ( "progtest", CIPv4 ( "147.32.232.142" ) ) ) == true );
    assert ( z0 . Add ( CRecAAAA ( "progtest", CIPv6 ( "2001:718:2:2902:0:1:2:3" ) ) ) == true );
    assert ( z0 . Add ( CRecA ( "courses", CIPv4 ( "147.32.232.158" ) ) ) == true );
    assert ( z0 . Add ( CRecA ( "courses", CIPv4 ( "147.32.232.160" ) ) ) == true );
    assert ( z0 . Add ( CRecA ( "courses", CIPv4 ( "147.32.232.159" ) ) ) == true );
    assert ( z0 . Add ( CRecAAAA ( "progtest", CIPv6 ( "2001:718:2:2902:1:2:3:4" ) ) ) == true );
    assert ( z0 . Add ( CRecMX ( "courses", "relay.fit.cvut.cz.", 0 ) ) == true );
    assert ( z0 . Add ( CRecMX ( "courses", "relay2.fit.cvut.cz.", 10 ) ) == true );
    oss . str ( "" );
    oss << z0;
    //cout << oss.str();
    assert ( oss . str () ==
             "fit.cvut.cz\n"
             " +- progtest A 147.32.232.142\n"
             " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
             " +- courses A 147.32.232.158\n"
             " +- courses A 147.32.232.160\n"
             " +- courses A 147.32.232.159\n"
             " +- progtest AAAA 2001:718:2:2902:1:2:3:4\n"
             " +- courses MX 0 relay.fit.cvut.cz.\n"
             " \\- courses MX 10 relay2.fit.cvut.cz.\n" );
    assert ( z0 . Search ( "progtest" ) . Count () == 3 );
    oss . str ( "" );
    oss << z0 . Search ( "progtest" );
    //cout << z0 . Search ( "progtest" );

    assert ( oss . str () ==
             "progtest A 147.32.232.142\n"
             "progtest AAAA 2001:718:2:2902:0:1:2:3\n"
             "progtest AAAA 2001:718:2:2902:1:2:3:4\n" );
    assert ( z0 . Del ( CRecA ( "courses", CIPv4 ( "147.32.232.160" ) ) ) == true );
    assert ( z0 . Add ( CRecA ( "courses", CIPv4 ( "147.32.232.122" ) ) ) == true );
    oss . str ( "" );
    oss << z0;
    assert ( oss . str () ==
             "fit.cvut.cz\n"
             " +- progtest A 147.32.232.142\n"
             " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
             " +- courses A 147.32.232.158\n"
             " +- courses A 147.32.232.159\n"
             " +- progtest AAAA 2001:718:2:2902:1:2:3:4\n"
             " +- courses MX 0 relay.fit.cvut.cz.\n"
             " +- courses MX 10 relay2.fit.cvut.cz.\n"
             " \\- courses A 147.32.232.122\n" );
    assert ( z0 . Search ( "courses" ) . Count () == 5 );
    oss . str ( "" );
    oss << z0 . Search ( "courses" );
    assert ( oss . str () ==
             "courses A 147.32.232.158\n"
             "courses A 147.32.232.159\n"
             "courses MX 0 relay.fit.cvut.cz.\n"
             "courses MX 10 relay2.fit.cvut.cz.\n"
             "courses A 147.32.232.122\n" );
    oss . str ( "" );
    oss << z0 . Search ( "courses" ) [ 0 ];
    assert ( oss . str () == "courses A 147.32.232.158" );
    assert ( z0 . Search ( "courses" ) [ 0 ] . Name () == "courses" );
    assert ( z0 . Search ( "courses" ) [ 0 ] . Type () == "A" );
    oss . str ( "" );
    oss << z0 . Search ( "courses" ) [ 1 ];
    assert ( oss . str () == "courses A 147.32.232.159" );
    assert ( z0 . Search ( "courses" ) [ 1 ] . Name () == "courses" );
    assert ( z0 . Search ( "courses" ) [ 1 ] . Type () == "A" );
    oss . str ( "" );
    oss << z0 . Search ( "courses" ) [ 2 ];
    assert ( oss . str () == "courses MX 0 relay.fit.cvut.cz." );
    assert ( z0 . Search ( "courses" ) [ 2 ] . Name () == "courses" );
    assert ( z0 . Search ( "courses" ) [ 2 ] . Type () == "MX" );
    try
    {
        oss . str ( "" );
        oss << z0 . Search ( "courses" ) [ 10 ];
        assert ( "No exception thrown!" == nullptr );
    }
    catch ( const out_of_range & e )
    {
    }
    catch ( ... )
    {
        assert ( "Invalid exception thrown!" == nullptr );
    }
    dynamic_cast<const CRecAAAA &> ( z0 . Search ( "progtest" ) [ 1 ] );
    CZone z1 ( "fit2.cvut.cz" );
    z1 . Add ( z0 . Search ( "progtest" ) [ 2 ] );
    z1 . Add ( z0 . Search ( "progtest" ) [ 0 ] );
    z1 . Add ( z0 . Search ( "progtest" ) [ 1 ] );
    z1 . Add ( z0 . Search ( "courses" ) [ 2 ] );
    oss . str ( "" );
    oss << z1;
    assert ( oss . str () ==
             "fit2.cvut.cz\n"
             " +- progtest AAAA 2001:718:2:2902:1:2:3:4\n"
             " +- progtest A 147.32.232.142\n"
             " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
             " \\- courses MX 0 relay.fit.cvut.cz.\n" );
    dynamic_cast<const CRecA &> ( z1 . Search ( "progtest" ) [ 1 ] );

    CZone z2 ( "fit.cvut.cz" );
    assert ( z2 . Add ( CRecA ( "progtest", CIPv4 ( "147.32.232.142" ) ) ) == true );
    assert ( z2 . Add ( CRecAAAA ( "progtest", CIPv6 ( "2001:718:2:2902:0:1:2:3" ) ) ) == true );
    assert ( z2 . Add ( CRecA ( "progtest", CIPv4 ( "147.32.232.144" ) ) ) == true );
    assert ( z2 . Add ( CRecMX ( "progtest", "relay.fit.cvut.cz.", 10 ) ) == true );
    assert ( z2 . Add ( CRecA ( "progtest", CIPv4 ( "147.32.232.142" ) ) ) == false );
    assert ( z2 . Del ( CRecA ( "progtest", CIPv4 ( "147.32.232.140" ) ) ) == false );
    assert ( z2 . Del ( CRecA ( "progtest", CIPv4 ( "147.32.232.142" ) ) ) == true );
    assert ( z2 . Del ( CRecA ( "progtest", CIPv4 ( "147.32.232.142" ) ) ) == false );
    assert ( z2 . Add ( CRecMX ( "progtest", "relay.fit.cvut.cz.", 20 ) ) == true );
    assert ( z2 . Add ( CRecMX ( "progtest", "relay.fit.cvut.cz.", 10 ) ) == false );
    oss . str ( "" );
    oss << z2;
    assert ( oss . str () ==
             "fit.cvut.cz\n"
             " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
             " +- progtest A 147.32.232.144\n"
             " +- progtest MX 10 relay.fit.cvut.cz.\n"
             " \\- progtest MX 20 relay.fit.cvut.cz.\n" );
    assert ( z2 . Search ( "progtest" ) . Count () == 4 );
    oss . str ( "" );
    oss << z2 . Search ( "progtest" );
    assert ( oss . str () ==
             "progtest AAAA 2001:718:2:2902:0:1:2:3\n"
             "progtest A 147.32.232.144\n"
             "progtest MX 10 relay.fit.cvut.cz.\n"
             "progtest MX 20 relay.fit.cvut.cz.\n" );
    assert ( z2 . Search ( "courses" ) . Count () == 0 );
    CZone z8("aaa");
    try
    {
        oss . str ( "" );
        oss << z8.Search("a") [0];;
        assert ( "No exception thrown!" == nullptr );
    }
    catch ( const out_of_range & e )
    {
    }
    catch ( ... )
    {
        assert ( "Invalid exception thrown!" == nullptr );
    }
    
    oss . str ( "" );
    oss << z2 . Search ( "courses" );
    assert ( oss . str () == "" );
    try
    {
        dynamic_cast<const CRecMX &> ( z2 . Search ( "progtest" ) [ 0 ] );
        assert ( "Invalid type" == nullptr );
    }
    catch ( const bad_cast & e )
    {
    }

    CZone z4 ( "fit.cvut.cz" );
    assert ( z4 . Add ( CRecA ( "progtest", CIPv4 ( "147.32.232.142" ) ) ) == true );
    assert ( z4 . Add ( CRecA ( "courses", CIPv4 ( "147.32.232.158" ) ) ) == true );
    assert ( z4 . Add ( CRecA ( "courses", CIPv4 ( "147.32.232.160" ) ) ) == true );
    assert ( z4 . Add ( CRecA ( "courses", CIPv4 ( "147.32.232.159" ) ) ) == true );
    CZone z5 ( z4 );
    assert ( z4 . Add ( CRecAAAA ( "progtest", CIPv6 ( "2001:718:2:2902:0:1:2:3" ) ) ) == true );
    assert ( z4 . Add ( CRecAAAA ( "progtest", CIPv6 ( "2001:718:2:2902:1:2:3:4" ) ) ) == true );
    assert ( z5 . Del ( CRecA ( "progtest", CIPv4 ( "147.32.232.142" ) ) ) == true );
    oss . str ( "" );
    oss << z4;
    assert ( oss . str () ==
             "fit.cvut.cz\n"
             " +- progtest A 147.32.232.142\n"
             " +- courses A 147.32.232.158\n"
             " +- courses A 147.32.232.160\n"
             " +- courses A 147.32.232.159\n"
             " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
             " \\- progtest AAAA 2001:718:2:2902:1:2:3:4\n" );
    oss . str ( "" );
    oss << z5;
    assert ( oss . str () ==
             "fit.cvut.cz\n"
             " +- courses A 147.32.232.158\n"
             " +- courses A 147.32.232.160\n"
             " \\- courses A 147.32.232.159\n" );
    z5 = z4;
    assert ( z4 . Add ( CRecMX ( "courses", "relay.fit.cvut.cz.", 0 ) ) == true );
    assert ( z4 . Add ( CRecMX ( "courses", "relay2.fit.cvut.cz.", 10 ) ) == true );
    oss . str ( "" );
    oss << z4;
    assert ( oss . str () ==
             "fit.cvut.cz\n"
             " +- progtest A 147.32.232.142\n"
             " +- courses A 147.32.232.158\n"
             " +- courses A 147.32.232.160\n"
             " +- courses A 147.32.232.159\n"
             " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
             " +- progtest AAAA 2001:718:2:2902:1:2:3:4\n"
             " +- courses MX 0 relay.fit.cvut.cz.\n"
             " \\- courses MX 10 relay2.fit.cvut.cz.\n" );
    oss . str ( "" );
    oss << z5;
    assert ( oss . str () ==
             "fit.cvut.cz\n"
             " +- progtest A 147.32.232.142\n"
             " +- courses A 147.32.232.158\n"
             " +- courses A 147.32.232.160\n"
             " +- courses A 147.32.232.159\n"
             " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
             " \\- progtest AAAA 2001:718:2:2902:1:2:3:4\n" );

    return 0;
}
#endif /* __PROGTEST__ */