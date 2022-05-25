#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cmath>
#include <cfloat>
#include <cassert>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#if defined ( __cplusplus ) && __cplusplus > 199711L /* C++ 11 */
#include <memory>
#endif /* C++ 11 */
using namespace std;
//#include <complex>
#endif /* __PROGTEST__ */

class CPolynomial
{
public:
    CPolynomial() {m_Coefficients.push_back(0);};
    CPolynomial operator + (const CPolynomial& p) const;
    CPolynomial operator - (const CPolynomial& p) const;
    CPolynomial operator * (const CPolynomial& p) const;
    CPolynomial operator * (double n) const;
    bool operator == (const CPolynomial& p ) const;
    bool operator != (const CPolynomial& p ) const;
    double operator [](size_t idx) const;
    double& operator [](size_t idx);
    double operator () (double x) const;
    size_t Degree() const;
    friend ostream& operator << (ostream& os, const CPolynomial & p );

    struct Complex {
    public:
        double real;
        double imag;

        explicit Complex(double r = 0, double i = 0);

        Complex operator+(const Complex &) const;
        Complex operator-(const Complex &) const;
        Complex operator*(const Complex &) const;
        Complex operator*(double ) const;
        Complex operator/(double ) const;
    };

    friend void fft(vector<CPolynomial::Complex> & a, bool invert);

private:
    vector<double> m_Coefficients;
};

CPolynomial::Complex::Complex(double r, double i)  : real(r), imag(i) {}

CPolynomial::Complex CPolynomial::Complex::operator+(const CPolynomial::Complex &c) const {
    return Complex(real + c.real, imag +c.imag);
}

CPolynomial::Complex CPolynomial::Complex::operator-(const CPolynomial::Complex & c) const {
    return Complex(real - c.real, imag - c.imag);
}

CPolynomial::Complex CPolynomial::Complex::operator*(const CPolynomial::Complex &c) const {
    Complex p{};
    p.real = real * c.real - imag * c.imag;
    p.imag = (real * c.imag) + (c.real * imag);
    return p;
}

CPolynomial::Complex CPolynomial::Complex::operator*(double d) const {
    return Complex(d * real, d*imag);
}

CPolynomial::Complex CPolynomial::Complex::operator/(double d) const {
    return Complex(real/d, imag/d);
}


bool CPolynomial::operator==(const CPolynomial &p) const {
    size_t minS = min(m_Coefficients.size(), p.m_Coefficients.size());
    for (size_t i = 0; i < minS; i++) {
        if (m_Coefficients[i] != p.m_Coefficients[i])
            return false;
    }
    if (m_Coefficients.size() < p.m_Coefficients.size()) {
        for (size_t i = minS; i < p.m_Coefficients.size(); i++) {
            if (p.m_Coefficients[i] != 0)
                return false;
        }
    }
    if (m_Coefficients.size() > p.m_Coefficients.size()) {
        for (size_t i = minS; i < m_Coefficients.size(); i++) {
            if (m_Coefficients[i] != 0)
                return false;
        }
    }
    return true;
}

bool CPolynomial::operator!=(const CPolynomial &p) const {
    size_t minS = min(m_Coefficients.size(), p.m_Coefficients.size());
    for (size_t i = 0; i < minS; i++) {
        if (m_Coefficients[i] != p.m_Coefficients[i])
            return true;
    }
    if (m_Coefficients.size() < p.m_Coefficients.size()) {
        for (size_t i = minS; i < p.m_Coefficients.size(); i++) {
            if (p.m_Coefficients[i] != 0)
                return true;
        }
    }
    if (m_Coefficients.size() > p.m_Coefficients.size()) {
        for (size_t i = minS; i < m_Coefficients.size(); i++) {
            if (m_Coefficients[i] != 0)
                return true;
        }
    }
    return false;
}


double CPolynomial::operator[](size_t idx) const {
    if (m_Coefficients.size() <= idx)
        return 0;
    return m_Coefficients.at(idx);
}


double &CPolynomial::operator[](size_t idx) {
    if (m_Coefficients.size() <= idx)
        m_Coefficients.resize(idx+1);
    return m_Coefficients.at(idx);
}

double CPolynomial::operator()(double x) const{
    double y = 0;
    for(size_t i = 0; i < m_Coefficients.size(); i++) {
        y += m_Coefficients.at(i)*(pow(x, i));
    }
    return y;
}

CPolynomial CPolynomial::operator*(double n) const {
    CPolynomial pol;
    for (size_t i = 0; i < m_Coefficients.size(); i++) {
        pol[i] = n*m_Coefficients.at(i);
    }
    return pol;
}

size_t CPolynomial::Degree() const {
    auto r = std::find_if(std::rbegin(m_Coefficients), std::rend(m_Coefficients), [](auto& v) { return std::fabs(v - 0) > DBL_EPSILON; } );
    if (r != rend(m_Coefficients)) {
        size_t d = distance(begin(m_Coefficients), (r+1).base());
        return d;
    }
    return 0;
}

CPolynomial CPolynomial::operator+(const CPolynomial &p) const {
    CPolynomial pol;
    for (size_t i = 0; i < m_Coefficients.size(); i++) {
        pol[i] = m_Coefficients.at(i);
    }
    for (size_t i = 0; i < p.m_Coefficients.size(); i++) {
        pol[i] += p.m_Coefficients.at(i);
    }
    return pol;
}

CPolynomial CPolynomial::operator-(const CPolynomial &p) const{
    CPolynomial pol;
    for (size_t i = 0; i < m_Coefficients.size(); i++) {
        pol[i] = m_Coefficients.at(i);
    }
    for (size_t i = 0; i < p.m_Coefficients.size(); i++) {
        pol[i] -= p.m_Coefficients.at(i);
    }
    return pol;
}

void fft(vector<CPolynomial::Complex> & a, bool invert) {
    static const double PI = acos(-1);
    int n = a.size();
    if (n == 1)
        return;

    vector<CPolynomial::Complex> a0(n / 2), a1(n / 2);
    for (int i = 0; 2 * i < n; i++) {
        a0[i] = a[2*i];
        a1[i] = a[2*i+1];
    }
    fft(a0, invert);
    fft(a1, invert);

    double ang = 2 * PI / n * (invert ? -1 : 1);
    CPolynomial::Complex w(1), wn(cos(ang), sin(ang));
    for (int i = 0; 2 * i < n; i++) {
        a[i] = a0[i] + w * a1[i];
        a[i + n/2] = a0[i] - w * a1[i];
        if (invert) {
            a[i] = a[i]/2;
            a[i + n/2] = a[i + n/2]/2;
        }
        w = wn * w;
    }
}

CPolynomial CPolynomial::operator*(const CPolynomial &p) const {
    CPolynomial r;
    vector<Complex> fa(p.m_Coefficients.begin(), p.m_Coefficients.end()), fb(m_Coefficients.begin(), m_Coefficients.end());
    size_t n = 1;
    while (n < p.m_Coefficients.size() + m_Coefficients.size())
        n <<= 1;
    fa.resize(n);
    fb.resize(n);

    fft(fb, false);
    fft(fa, false);
    for (size_t i = 0; i < n; i++)
        fa[i] = fb[i] * fa[i];
    fft(fa, true);

    vector<double> result(n);

    r.m_Coefficients.resize(n);
    for (size_t i = 0; i < n; i++)
        r.m_Coefficients[i] = round((fa[i].real * pow(10, 4)))/pow(10, 4);
    return r;
}

/*
CPolynomial CPolynomial::operator*(const CPolynomial &p) const {
    CPolynomial r;
    size_t sizeP = p.m_Coefficients.size();
    size_t sizeT = m_Coefficients.size();
    //r.m_Coefficients.resize(sizeP+sizeT);

    for (size_t i=0; i<sizeP; i++)
    {
        for (size_t j=0; j<sizeT; j++)
            r[i+j] += p[i]*m_Coefficients[j];
    }
    return r;
}
*/

//              x^3 + 3.5*x^1 - 10
ostream &operator<<(ostream &os, const CPolynomial &p) {
    int j = (int)p.m_Coefficients.size() - 1;
    while (p[j] == 0) {
        if ( p[j] != 0 )
            break;
        if ( j == 0 )
            break;
        j--;
    }

    if ( j == -1 ) {
        os << "0";
        return os;
    } else if (j == 0) {
        if (p.m_Coefficients.at(j) < 0) {
            os << "- ";
        }
        os << abs(p.m_Coefficients.at(j));
    } else {
        if (p.m_Coefficients.at(j) < 0) {
            os << "- ";
        }
        if (abs(p.m_Coefficients.at(j)) != 1) {
            os << abs(p.m_Coefficients.at(j)) << "*";
        }
        os << "x^" << j;
    }

    for(int i = (int)j - 1; i >= 1 ; i--) {
        if (p.m_Coefficients.at(i) == 0)
            continue;
        else {
            if (p.m_Coefficients.at(i) < 0) {
                os << " - ";
            }
            if (p.m_Coefficients.at(i) >= 0) {
                os << " + ";
            }
            if (abs(p.m_Coefficients.at(i)) != 1) {
                os << abs(p.m_Coefficients.at(i)) << "*";
            }
            os << "x^" << i;
        }
    }
    if (j > 0 && p[0] != 0) {
        if (p[0] > 0) {
            os << " + " << p[0];
        } else {

            os << " - " << abs(p[0]);
        }
    }
    return os;

}

#ifndef __PROGTEST__

bool               smallDiff                               ( double            a,
                                                             double            b )
{
    return fabs(a-b) < DBL_EPSILON * a;
}

bool               dumpMatch                               ( const CPolynomial & x,
                                                             const vector<double> & ref )
{
    return true;
}

int                main                                    ( void )
{
  CPolynomial a, b, c, d;
  ostringstream out;
    a.Degree();
  a[0] = -10;
  a[1] = 3.5;
  a[3] = 1;
    a.Degree();
  assert ( smallDiff ( a ( 2 ), 5 ) );
  out . str ("");
a.Degree();
    //std::ostringstream output;
    //output << a;
    //cout << output.str() << endl;
    //cout << a << endl;
  out << a;
    cout << a << endl;
  assert ( out . str () == "x^3 + 3.5*x^1 - 10" );
    out . str ("");
    out << d;
    assert ( out . str () == "0" );
    out . str ("");
    d[3] = 1;
    cout << d << endl;
    out << d;
    assert ( out . str () == "x^3" );
    out . str ("");
    d[3] = -11;
    out << d;
    cout << d << endl;
    assert ( out . str () == "- 11*x^3" );
    out . str ("");
    d[3] = -1;
    out << d;
    cout << d << endl;
    assert ( out . str () == "- x^3" );
    out . str ("");
    d[3] = -1;
    d[0] = -1;
    out << d;
    cout << d << endl;
    assert ( out . str () == "- x^3 - 1" );
    d[10] = 0;
    d[5];
    d.Degree();
  a = a * -2;

   assert ( a . Degree () == 3
            && dumpMatch ( a, vector<double>{ 20.0, -7.0, -0.0, -2.0 } ) );

   out . str ("");
    //cout << a << endl;
   out << a ;
   assert ( out . str () == "- 2*x^3 - 7*x^1 + 20" );
   out . str ("");
    //cout << b << endl;
   out << b;
   assert ( out . str () == "0" );
   b[5] = -1;
   out . str ("");
    //cout << b << endl;
   out << b;
   assert ( out . str () == "- x^5" );
   c = a + b;
   assert ( c . Degree () == 5
            && dumpMatch ( c, vector<double>{ 20.0, -7.0, 0.0, -2.0, 0.0, -1.0 } ) );

   out . str ("");
   out << c;
   assert ( out . str () == "- x^5 - 2*x^3 - 7*x^1 + 20" );

   c = a - b;
   assert ( c . Degree () == 5
            && dumpMatch ( c, vector<double>{ 20.0, -7.0, -0.0, -2.0, -0.0, 1.0 } ) );

   out . str ("");
   out << c;
   assert ( out . str () == "x^5 - 2*x^3 - 7*x^1 + 20" );

   c = a * b;
   assert ( c . Degree () == 8
            && dumpMatch ( c, vector<double>{ -0.0, -0.0, 0.0, -0.0, 0.0, -20.0, 7.0, -0.0, 2.0 } ) );

   out . str ("");
   out << c;
    //cout << c;
   assert ( out . str () == "2*x^8 + 7*x^6 - 20*x^5" );

   assert ( a != b );
   b[5] = 0;
   assert ( !(a == b) );
   a = a * 0;
   assert ( a . Degree () == 0
            && dumpMatch ( a, vector<double>{ 0.0 } ) );

   assert ( a == b );
  return 0;
}

#endif /* __PROGTEST__ */
/*
vector<complex<double>> ffft (const vector<complex<double>>& p, complex<double> w) {
    size_t n = p.size();
    if (p.size() == 1) {
        return vector<complex<double>> (1, p[0]);
    } else {
        vector<complex<double>> A_even(n / 2), A_odd(n / 2);
        for (int i = 0; i < n / 2; i++) {

            // even indexed coefficients
            A_even[i] = p[i * 2];

            // odd indexed coefficients
            A_odd[i] = p[i * 2 + 1];
        }


        //complex<double> w = (cos(2*M_PI/n), sin(2*M_PI/n));
        vector<complex<double>> F_even = ffft(A_even, w*w);
        vector<complex<double>> F_odd = ffft(A_odd, w*w);

        vector<complex<double>> F;
        F.resize(p.size());

        complex<double> x = 1;
        for (int i = 0; i < n / 2; ++i) {
            complex<double> t = pow(polar(1.0, -2 * M_PI * i /n), -1) * F_odd[i];
            F[i] = F_even[i] + t;//x*F_odd[i];
            F[i + n / 2] = F_even[i] - t;//x * F_odd[i];
            x = x * w;
            int p = 0;
        }
        return F;
    }
}
 */
/*
CPolynomial CPolynomial::operator*( CPolynomial &p) {
    uint16_t sizeP = nextPowerOf2(p.m_Coefficients.size());
    uint16_t sizeT = nextPowerOf2(m_Coefficients.size());
    p.m_Coefficients.resize(sizeP);
    m_Coefficients.resize(sizeT);
    CPolynomial pol;
    complex<double> w = (cos(2*M_PI/m_Coefficients.size()), sin(2*M_PI/m_Coefficients.size()));
    vector<complex<double>> fa = fft(m_Coefficients, w);
    double n = abs (complex<double> (20, 5));

    complex<double> v = (cos(2*M_PI/p.m_Coefficients.size()), sin(2*M_PI/p.m_Coefficients.size()));
    vector<complex<double>> fb = fft(p.m_Coefficients, v);


    size_t maxs = 2*max(m_Coefficients.size(), p.m_Coefficients.size());
    size_t sizeX = nextPowerOf2(maxs);

    vector<complex<double>> fc(sizeX);

    fb.resize(maxs, 0);
    fa.resize(maxs, 0);
    for (int i = 0; i < maxs; i++) {
        fc[i] = fa[i] * fb[i];
    }

   complex<double> l = (cos(2*M_PI/fc.size()), sin(2*M_PI/fc.size()));
    l = pow(l,-1);
    vector<complex<double>> fx = ffft (fc, l);

    for (int i = 0; i < fx.size(); i++) {
        fc[i] = abs(fx[i]) * 1/fx.size();
    }

    return pol;
}
*/