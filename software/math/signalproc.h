/*
* Copyright (c) 2012 Joey Yore
*
* Permission is hereby granted, free of charge, to any person obtaining a 
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation 
* the rights to use, copy, modify, merge, publish, distribute, sublicense, 
* and/or sell copies of the Software, and to permit persons to whom the 
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included 
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR 
* OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
* OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef SIGNALPROC_H
#define SIGNALPROC_H

#include <math.h>
#include <cmath>
#include "complex.h"
#include "vector.h"
#include "matrix.h"
#include "eml.h"
#include <iostream>

using namespace std;

namespace signalproc {

	using namespace complex_number;

	Complex prod(Vector<Complex> c) {
		Complex result = c[0];

		for(int i=1,l=c.Size();i<l;++i) {
			result *= c[i];
		}

		return result;
	}

	Vector<Complex> real(Vector<Complex> c) {
		Vector<Complex> o(c);
		for(int i=0,l=o.Size();i<l;++i) {
			o[i] = complex_number::real(o[i]);
		}
		return o;
	}

	Vector<Complex> abs(Vector<Complex> v) {
		Vector<Complex> o(v);
		for(int i=0,l=o.Size();i<l;++i) {
			o[i] = complex_number::abs(o[i]);
		}
		return o;
	} 

	Vector<Complex> poly(Vector<Complex> x) {
		int n = x.Size();
		Vector<Complex> c(n);
		c.Push_Front(Complex(1.0));

		for(int j=0;j<n;++j) {
			Vector<Complex> cc(c);
			for(int i=1;i<=j+1;++i) {
				c[i] = c[i] - x[j]*cc[i-1];
			}
		}

		return real(c);
	}	

	Vector<Complex> eig(Vector<Vector<Complex> > x) {
		
		Vector<Vector<Complex> > Ac(x);
		Vector<Vector<Complex> > Bc;
		Vector<Complex> a1,b1;
		eml::zggev(Ac,Bc,a1,b1);
		return a1/b1;
	}

	Vector<Complex> poly(Vector<Vector<Complex> > x) {
		Vector<Complex> e = eig(x);
		return poly(e);
	}


	void buttap(int n, Vector<Complex> &z, Vector<Complex> &p, double &k) {

		Vector<Complex> pm = Vector<Complex>(p);

		for(int i=0;i<n-1;i+=2) {
			double wt = M_PI*(i+1)/(2*n) + M_PI/2;
			p[i] = Complex(cos(wt),sin(wt));
			p[i+1] = conj(p[i]);
			pm[i] = -p[i];
			pm[i+1] = -p[i+1];
		}

		if(n % 2) {
			p[n-1] = Complex(-1,0);
			pm[n-1] = -p[n-1];
		}

		k = complex_number::real(prod(pm));

	}

	void zp2ss(Vector<Complex> z, Vector<Complex> p, double k, Vector<Vector<Complex> > &a, Vector<Vector<Complex> > &b, Vector<Vector<Complex> > &c, Vector<Vector<Complex> > &d) {
		int np = p.Size();
		int nz = z.Size();
		int remp = np % 2;
		int remz = nz % 2;

		if(remp) {
			a = Vector<Vector<Complex> >(1); a[0] = Vector<Complex>(1);
			b = Vector<Vector<Complex> >(1); b[0] = Vector<Complex>(1);
			c = Vector<Vector<Complex> >(1); c[0] = Vector<Complex>(1);
			d = Vector<Vector<Complex> >(1); d[0] = Vector<Complex>(1);
			if(remz) {	
				a[0][0] = p[np-1];
				b[0][0] = 1;
				c[0][0] = p[np] - z[nz];
				d[0][0] = 1;
				--np;
				--nz;
			} else {
				a[0][0] = complex_number::real(p[np-1]);
				b[0][0] = 1.0;
				c[0][0] = 1.0;
				d[0][0] = 0.0;
				--np;
			}
		} else if(remz) {
			throw "zp2ss: Not Implemented";
		}

		int i=0;
		while(i<nz) {
			i += 2;
		}


		while(i<np) {
			Vector<Complex> den = real(poly(p(np-i-2,np-i-1)));
			double wn = complex_number::abs(complex_number::sqrt(prod(abs(p(i,i+1)))));
			if(wn == 0.0) {
				wn = 1.0;
			}
			
			//a1
			Matrix t(2,2);
			t(0,0) = 1.0; t(0,1) = 0.0; 
			t(1,0) = 0.0; t(1,1) = 1/wn;

			cout << den << endl;

			Matrix s(2,2);
			s(0,0) = -static_cast<double>(den[1]);
			s(0,1) = -static_cast<double>(den[2]);
			s(1,0) = 1.0; 
			s(1,1) = 0,0;
			
			Vector<Vector<Complex> > a1 = matrix::vectorize<Complex>(!t*s*t);
			
			//b1  = inv(t) matrix mult [1;0]
			Vector<Vector<Complex> > b1(2); 
			b1[0] = Vector<Complex>(1); b1[1] = Vector<Complex>(1);
			b1[0][0] = t[0][0];
			b1[1][0] = 0.0;

			//c1  = [0 1] matrix mult t
			Vector<Vector<Complex> > c1(1);
			c1[0] = Vector<Complex>(2);
			c1[0][0] = 0.0;
			c1[0][1] = t[1][1];

			//d1  = 0 
			Vector<Vector<Complex> > d1(1);
			d1[0] = Vector<Complex>(1);
			d1[0][0] = 0.0;

			
			//a = [a zeros(ma1,na2);b1*c a1]
			//Find b1*c
			Matrix mb1(b1);
			Matrix mc(c);
			Matrix mbc = mb1*mc;

			//Add b1*c as new rows
			for(int ii=0,r=mbc.getNumRows();ii<r;++ii) {
				Vector<Complex> v;
				for(int jj=0,c=mbc.getNumCols();jj<c;++jj) {
					v.Push_Back(Complex(mbc(ii,jj)));
				}
				a.Push_Back(v);
			}

			//Add 2 cols to each row in a filling with 0+i0
			Complex cc(0.0);
			for(int ii=0,l=a.Size();ii<l;++ii) {
				a[ii].Push_Back(cc);
				a[ii].Push_Back(cc);
			}

			//Add a1 to last 2x2 sub matrix
			int row = a.Size();
			int col = a[0].Size();
			for(int ii=0;ii<2;++ii) {
				for(int jj=0;jj<2;++jj) {
					a[ii+row-2][jj+col-2] = a1[ii][jj];
				}
			}


			//b = [b; b1*d]
			Vector<Complex> bd(1); bd[0] = b[0][0] * d[0][0];
			b.Push_Back(bd);
			bd[0] = b[1][0] * d[0][0];
			b.Push_Back(bd);

			//c = [d1*c c1]
			for(int k=0,l=c[0].Size();k<l;++k) {
				c[0][k] *= d1[0][0];
			}
			c[0].Push_Back(complex_number::real(c1[0][0]));
			c[0].Push_Back(complex_number::real(c1[0][1]));
			
			//d = d1*d
			d[0] = d1[0][0]*d[0];

			i += 2;
		}

		c = c*k;
		d = d*k;

	}

	void lp2lp(Vector<Vector<Complex> > &a, Vector<Vector<Complex> > &b, double wo) {
		a = wo*a;
		b = wo*b;
	}

	void bilinear(Vector<Vector<Complex> > &a, Vector<Vector<Complex> > &b, Vector<Vector<Complex> > &c, Vector<Vector<Complex> > &d, double fs) {

		double t = 1/fs;
		double r = sqrt(t);
		Matrix ma(a); Matrix mb(b); Matrix mc(c); Matrix md(d);
		Matrix t1 = matrix::eye(a.Size()) + ma*t/2;
		Matrix t2 = matrix::eye(a.Size()) - ma*t/2;

		a = matrix::vectorize<Complex>(!t2*t1);
		b = matrix::vectorize<Complex>(t/r*(!t2*mb));
		c = matrix::vectorize<Complex>(r*mc*!t2);
		d = matrix::vectorize<Complex>(mc*!t2*mb*t/2 + md);
	}

	void butter(int n, double Wn, Vector<Complex> &num, Vector<Complex> &den, const char *type="lp") {

		throw "NOT IMPLMENTED: DO NOT USE BUTTER";

		if(n>500) {
			throw "Butter: Filter Order Too Large";
		}

		if(strcmp(type,"lp")) {
			throw "Butter: Not Implemented";
		}

		//Step 1: Get the Pre-Warped Frequency
		double fs = 2;
		Wn = 2*fs*tan(M_PI*Wn/fs);

		cout << Wn << endl;

		//Step 2: Get N-th Order Butterworth LP Prototype
		Vector<Complex> z;
		Vector<Complex> p(n);
		double k;
		buttap(n,z,p,k);

		cout << z << endl;
		cout << p << endl;
		cout << k << endl;

		Vector<Vector<Complex> > a,b,c,d;
		zp2ss(z,p,k,a,b,c,d);

		cout << "a = " << endl;
		cout << Matrix(a) << endl;
		cout << endl << "b = " << endl;
		cout << Matrix(b) << endl;
		cout << endl << "c = " << endl;
		cout << Matrix(c) << endl;
		cout << endl << "d = " << endl;
		cout << Matrix(d) << endl;

		//Step 3: Transform to filter type
		if(!strcmp(type,"lp")) {
			lp2lp(a,b,Wn);
		}

		cout << "a = " << endl;
		cout << Matrix(a) << endl;
		cout << endl << "b = " << endl;
		cout << Matrix(b) << endl;
		cout << endl << "c = " << endl;
		cout << Matrix(c) << endl;
		cout << endl << "d = " << endl;
		cout << Matrix(d) << endl;


		bilinear(a,b,c,d,fs);

		cout << "a = " << endl;
		cout << Matrix(a) << endl;
		cout << endl << "b = " << endl;
		cout << Matrix(b) << endl;
		cout << endl << "c = " << endl;
		cout << Matrix(c) << endl;
		cout << endl << "d = " << endl;
		cout << Matrix(d) << endl;

		//TODO: Implement poly for matrix and buttnum
		//den = poly(a);
		//num = buttnum(btype,n,Wn,Bw,analog,den);
		// -or-
		// num = poly(a-b*c)+(d-1)*den;

	}

	Vector<double> filter(const Vector<double> &b, const Vector<double> &a, const Vector<double> &x) {

		int nx = x.Size();
		Vector<double> y(nx);
		int n = b.Size();

		for(int i=n;i<nx;++i) {

			y[i] = 0.0;
			for(int k=0;k<n;++k) {
				y[i] += b[k]*x[i-k];
			}
			for(int k=1;k<n;++k) {
				y[i] -= a[k]*y[i-k];
			}
		}

		return y;
	}
}

#endif
