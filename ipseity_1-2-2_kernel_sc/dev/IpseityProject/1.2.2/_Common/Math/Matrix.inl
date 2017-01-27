/*
 * $Id$
 *
 * Ipseity is an open-source platform dedicated to the development and
 * validation of Artificial Intelligence techniques in Multiagent Systems.
 * It is developed in C++ with the Qt framework (http://qt-project.org).
 * More details on <http://www.ipseity-project.com>
 * Copyright (C) 2015, 2014, 2013 Ipseity Core Developers
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

 
#include "MemDefns.h"
#include "MathToolBox_Misc.h"

#include <math.h>


template <class T>
uint32 GenericMatrix<T>::m_InversionMethod = IM_SVD;

template <class T>
T GenericMatrix<T>::m_DiagonalValue = 0.00001;


template <class T>
void GenericMatrix<T>::setInversionMethod( uint32 im )
{
	m_InversionMethod = im;
}

template <class T>
void GenericMatrix<T>::setDiagonalValue( T diagValue )
{
	m_DiagonalValue = diagValue;
}


template <class T>
GenericMatrix<T>::GenericMatrix()
{
	m_NbrRows = m_NbrColumns = 0;
	m_Values = NULL;
}

template <class T>
GenericMatrix<T>::GenericMatrix( int32 rows, int32 columns )
{
	m_NbrRows = rows;
	m_NbrColumns = columns;
	Create2DArray( m_Values, rows, columns, T );
}

template <class T>
GenericMatrix<T>::GenericMatrix( const GenericMatrix & m )
{
	m_NbrRows = m.getNbrRows();
	m_NbrColumns = m.getNbrColumns();
	Create2DArray( m_Values, m_NbrRows, m_NbrColumns, T );

	int32 i, j;

	for( j = 0; j < m_NbrRows; j++ )
	{
		for( i = 0; i < m_NbrColumns; i++ )
		{
			m_Values[j][i] = m[j][i];
		}
	}
}

template <class T>
GenericMatrix<T>::~GenericMatrix()
{
	destroy();
}


template <class T>
int32 GenericMatrix<T>::getNbrRows() const
{
	return m_NbrRows;
}

template <class T>
int32 GenericMatrix<T>::getNbrColumns() const
{
	return m_NbrColumns;
}


template <class T>
T* & GenericMatrix<T>::operator[]( int32 nrLine ) const
{
	return m_Values[nrLine];
}


template <class T>
void GenericMatrix<T>::setSize( int32 lines, int32 columns )
{
	SafeDelete2DArray( m_Values, lines );

	m_NbrRows = lines;
	m_NbrColumns = columns;
	Create2DArray( m_Values, lines, columns, T );
}


template <class T>
void GenericMatrix<T>::fill( T v )
{
	int32 i, j;

	for( j = 0; j < m_NbrRows; j++ )
	{
		for( i = 0; i < m_NbrColumns; i++ )
		{
			m_Values[j][i] = v;
		}
	}
}


template <class T>
void GenericMatrix<T>::setIdentity( T v )
{
	if( m_NbrRows == m_NbrColumns )
	{
		fill( (T) 0 );
		for( int32 i = 0; i < m_NbrRows; i++ )
		{
			m_Values[i][i] = v;
		}
	}
}


template <class T>
GenericMatrix<T> & GenericMatrix<T>::operator=( const GenericMatrix & m )
{
	if( m.getNbrRows() != getNbrRows() ||
		m.getNbrColumns() != getNbrColumns() )
	{
		setSize( m.getNbrRows(), m.getNbrColumns() );
	}

	int32 i, j;

	for( j = 0; j < m_NbrRows; j++ )
	{
		for( i = 0; i < m_NbrColumns; i++ )
		{
			m_Values[j][i] = m[j][i];
		}
	}

	return (*this);
}

template <class T>
GenericMatrix<T> & GenericMatrix<T>::operator=( const GenericVector<T> & m )
{
	if( getNbrRows() != m.getSize() ||
		getNbrColumns() != 1 )
	{
		setSize( m.getSize(), 1 );
	}

	for( int32 j = 0; j < m_NbrRows; j++ )
	{
		m_Values[j][0] = m[j];
	}

	return (*this);
}

template <class T>
GenericMatrix<T> & GenericMatrix<T>::operator=( T v )
{
	fill( v );

	return (*this);
}

template <class T>
GenericMatrix<T> & GenericMatrix<T>::operator+=( const GenericMatrix & m )
{
	if( m.getNbrRows() == getNbrRows() &&
		m.getNbrColumns() == getNbrColumns() )
	{
		int32 i, j;

		for( j = 0; j < m_NbrRows; j++ )
		{
			for( i = 0; i < m_NbrColumns; i++ )
			{
				m_Values[j][i] += m[j][i];
			}
		}
	}

	return (*this);
}

template <class T>
GenericMatrix<T> & GenericMatrix<T>::operator*=( T v )
{
	int32 i, j;

	for( j = 0; j < m_NbrRows; j++ )
	{
		for( i = 0; i < m_NbrColumns; i++ )
		{
			m_Values[j][i] *= v;
		}
	}

	return (*this);
}

template <class T>
GenericMatrix<T> GenericMatrix<T>::operator+( const GenericMatrix & m )
{
	GenericMatrix<T> r( *this );

	if( m.getNbrRows() == getNbrRows() &&
		m.getNbrColumns() == getNbrColumns() )
	{
		int32 i, j;

		for( j = 0; j < m_NbrRows; j++ )
		{
			for( i = 0; i < m_NbrColumns; i++ )
			{
				r[j][i] += m[j][i];
			}
		}
	}

	return r;
}

template <class T>
GenericMatrix<T> GenericMatrix<T>::operator-( const GenericMatrix & m )
{
	GenericMatrix<T> r( *this );

	if( m.getNbrRows() == getNbrRows() &&
		m.getNbrColumns() == getNbrColumns() )
	{
		int32 i, j;

		for( j = 0; j < m_NbrRows; j++ )
		{
			for( i = 0; i < m_NbrColumns; i++ )
			{
				r[j][i] -= m[j][i];
			}
		}
	}

	return r;
}
template <class T>
GenericMatrix<T> GenericMatrix<T>::operator*( const GenericMatrix & m )
{
	GenericMatrix<T> r( m_NbrRows, m.getNbrColumns() );
	int32 i, j, k;

	for( j = 0; j < m_NbrRows; j++ )
	{
		for( i = 0; i < m.getNbrColumns(); i++ )
		{
			r[j][i] = 0.0f;
			for( k = 0; k < m_NbrColumns; k++ )
			{
				r[j][i] += m_Values[j][k]*m[k][i];
			}
		}
	}

	return r;
}

template <class T>
GenericMatrix<T> GenericMatrix<T>::operator*( T v )
{
	GenericMatrix<T> r( *this );
	int32 i, j;

	for( j = 0; j < m_NbrRows; j++ )
	{
		for( i = 0; i < m_NbrColumns; i++ )
		{
			r[j][i] = m_Values[j][i]*v;
		}
	}

	return r;
}


template <class T>
T GenericMatrix<T>::maxValue() const
{
	int32 i, j;
	T maxValue = m_Values[0][0];

	for( j = 0; j < m_NbrRows; j++ )
	{
		for( i = 0; i < m_NbrColumns; i++ )
		{
			if( m_Values[j][i] > maxValue )
			{
				maxValue = m_Values[j][i];
			}
		}
	}

	return maxValue;
}


template <class T>
T GenericMatrix<T>::dot( const GenericMatrix<T> & m ) const
{
	T sum = 0.0;

	if( m.getNbrRows() == getNbrRows() &&
		m.getNbrColumns() == 1 && getNbrColumns() == 1 )
	{
		for( int32 i = 0; i < m_NbrRows; i++ )
		{
			sum += m[i][0]*m_Values[i][0];
		}
	}
	else if( m.getNbrColumns() == getNbrColumns() &&
		m.getNbrRows() == 1 && getNbrRows() == 1 )
	{
		for( int32 i = 0; i < m_NbrColumns; i++ )
		{
			sum += m[0][i]*m_Values[0][i];
		}
	}

	return sum;
}


template <class T>
GenericMatrix<T> GenericMatrix<T>::transpose() const
{
	GenericMatrix<T> r( m_NbrColumns, m_NbrRows );
	int32 i, j;

	for( j = 0; j < m_NbrColumns; j++ )
	{
		for( i = 0; i < m_NbrRows; i++ )
		{
			r[j][i] = m_Values[i][j];
		}
	}

	return r;
}



template <class T>
void GenericMatrix<T>::initForLaterInversion()
{
	switch( m_InversionMethod )
	{
		case IM_LU:
			setIdentity( m_DiagonalValue );
			break;

		case IM_SVD:
			fill( 0 );
			break;
	}
}



/*
 *  SVD
 *
 *  Returns an error code = 0, if no errors and 'k' if a failure to
 *  converge at the 'kth' singular value.
 */

template <class T>
int GenericMatrix<T>::svd( int m, int n, int withu, int withv, T eps, T tol, T **a, T *q, T **u, T **v) const
{
	int i,j,k,l,l1,iter,retval;
	T c,f,g,h,s,x,y,z;
	T *e;

	e = new T [n];

	retval = 0;

/* Copy 'a' to 'u' */    
	for (i=0;i<m;i++) {
		for (j=0;j<n;j++)
			u[i][j] = a[i][j];
	}
/* Householder's reduction to bidiagonal form. */
	g = x = 0.0;    
	for (i=0;i<n;i++) {
		e[i] = g;
		s = 0.0;
		l = i+1;
		for (j=i;j<m;j++)
			s += (u[j][i]*u[j][i]);
		if (s < tol)
			g = 0.0;
		else {
			f = u[i][i];
			g = (f < 0) ? sqrt(s) : -sqrt(s);
			h = f * g - s;
			u[i][i] = f - g;
			for (j=l;j<n;j++) {
				s = 0.0;
				for (k=i;k<m;k++)
					s += (u[k][i] * u[k][j]);
				f = s / h;
				for (k=i;k<m;k++)
					u[k][j] += (f * u[k][i]);
			} /* end j */
		} /* end s */
		q[i] = g;
		s = 0.0;
		for (j=l;j<n;j++)
			s += (u[i][j] * u[i][j]);
		if (s < tol)
			g = 0.0;
		else {
			f = u[i][i+1];
			g = (f < 0) ? sqrt(s) : -sqrt(s);
			h = f * g - s;
			u[i][i+1] = f - g;
			for (j=l;j<n;j++) 
				e[j] = u[i][j]/h;
			for (j=l;j<m;j++) {
				s = 0.0;
				for (k=l;k<n;k++) 
					s += (u[j][k] * u[i][k]);
				for (k=l;k<n;k++)
					u[j][k] += (s * e[k]);
			} /* end j */
		} /* end s */
		y = fabs(q[i]) + fabs(e[i]);                         
		if (y > x)
			x = y;
	} /* end i */

/* accumulation of right-hand transformations */
	if (withv) {
		for (i=n-1;i>=0;i--) {
			if (g != 0.0) {
				h = u[i][i+1] * g;
				for (j=l;j<n;j++)
					v[j][i] = u[i][j]/h;
				for (j=l;j<n;j++) {
					s = 0.0;
					for (k=l;k<n;k++) 
						s += (u[i][k] * v[k][j]);
					for (k=l;k<n;k++)
						v[k][j] += (s * v[k][i]);

				} /* end j */
			} /* end g */
			for (j=l;j<n;j++)
				v[i][j] = v[j][i] = 0.0;
			v[i][i] = 1.0;
			g = e[i];
			l = i;
		} /* end i */
 
	} /* end withv, parens added for clarity */

/* accumulation of left-hand transformations */
	if (withu) {
		for (i=n;i<m;i++) {
			for (j=n;j<m;j++)
				u[i][j] = 0.0;
			u[i][i] = 1.0;
		}
	}
	if (withu) {
		for (i=n-1;i>=0;i--) {
			l = i + 1;
			g = q[i];
			for (j=l;j<m;j++)  /* upper limit was 'n' */
				u[i][j] = 0.0;
			if (g != 0.0) {
				h = u[i][i] * g;
				for (j=l;j<m;j++) { /* upper limit was 'n' */
					s = 0.0;
					for (k=l;k<m;k++)
						s += (u[k][i] * u[k][j]);
					f = s / h;
					for (k=i;k<m;k++) 
						u[k][j] += (f * u[k][i]);
				} /* end j */
				for (j=i;j<m;j++) 
					u[j][i] /= g;
			} /* end g */
			else {
				for (j=i;j<m;j++)
					u[j][i] = 0.0;
			}
			u[i][i] += 1.0;
		} /* end i*/
	} /* end withu, parens added for clarity */

/* diagonalization of the bidiagonal form */
	eps *= x;
	for (k=n-1;k>=0;k--) {
		iter = 0;
test_f_splitting:
		for (l=k;l>=0;l--) {
			if (fabs(e[l]) <= eps) goto test_f_convergence;
			if (fabs(q[l-1]) <= eps) goto cancellation;
		} /* end l */

/* cancellation of e[l] if l > 0 */
cancellation:
		c = 0.0;
		s = 1.0;
		l1 = l - 1;
		for (i=l;i<=k;i++) {
			f = s * e[i];
			e[i] *= c;
			if (fabs(f) <= eps) goto test_f_convergence;
			g = q[i];
			h = q[i] = sqrt(f*f + g*g);
			c = g / h;
			s = -f / h;
			if (withu) {
				for (j=0;j<m;j++) {
					y = u[j][l1];
					z = u[j][i];
					u[j][l1] = y * c + z * s;
					u[j][i] = -y * s + z * c;
				} /* end j */
			} /* end withu, parens added for clarity */
		} /* end i */
test_f_convergence:
		z = q[k];
		if (l == k) goto convergence;

/* shift from bottom 2x2 minor */
		iter++;
		if (iter > 30) {
			retval = k;
			break;
		}
		x = q[l];
		y = q[k-1];
		g = e[k-1];
		h = e[k];
		f = ((y-z)*(y+z) + (g-h)*(g+h)) / (2*h*y);
		g = sqrt(f*f + 1.0);
		f = ((x-z)*(x+z) + h*(y/((f<0)?(f-g):(f+g))-h))/x;
/* next QR transformation */
		c = s = 1.0;
		for (i=l+1;i<=k;i++) {
			g = e[i];
			y = q[i];
			h = s * g;
			g *= c;
			e[i-1] = z = sqrt(f*f+h*h);
			c = f / z;
			s = h / z;
			f = x * c + g * s;
			g = -x * s + g * c;
			h = y * s;
			y *= c;
			if (withv) {
				for (j=0;j<n;j++) {
					x = v[j][i-1];
					z = v[j][i];
					v[j][i-1] = x * c + z * s;
					v[j][i] = -x * s + z * c;
				} /* end j */
			} /* end withv, parens added for clarity */
			q[i-1] = z = sqrt(f*f + h*h);
			c = f/z;
			s = h/z;
			f = c * g + s * y;
			x = -s * g + c * y;
			if (withu) {
				for (j=0;j<m;j++) {
					y = u[j][i-1];
					z = u[j][i];
					u[j][i-1] = y * c + z * s;
					u[j][i] = -y * s + z * c;
				} /* end j */
			} /* end withu, parens added for clarity */
		} /* end i */
		e[l] = 0.0;
		e[k] = f;
		q[k] = x;
		goto test_f_splitting;
convergence:
		if (z < 0.0) {
/* q[k] is made non-negative */
			q[k] = - z;
			if (withv) {
				for (j=0;j<n;j++)
					v[j][k] = -v[j][k];
			} /* end withv, parens added for clarity */
		} /* end z */
	} /* end k */
	
	delete e;

	return retval;
}


#define EPS 2.2204e-16


template <class T>
GenericMatrix<T> GenericMatrix<T>::invert() const
{
	int32 N = m_NbrRows;
	GenericMatrix<T> r( N, N );

	if( m_NbrRows == m_NbrColumns )
	{
		int32 i, j;

		if( m_InversionMethod == IM_SVD )
		// Using SVD
		{
			T** u;
			T* s;
			T** v;
			int32 k;
			T tol;

			Create2DArray( u, N, N, T );
			CreateArray( s, N, T );
			Create2DArray( v, N, N, T );

			svd( N, N, 1, 1, EPS, N*EPS, m_Values, s, u, v );

			tol = s[0];
			for( i = 1; i < N; i++ )
			{
				if( s[i] > tol )
				{
					tol = s[i];
				}
			}
			tol *= N*EPS;

			for( i = 0; i < N; i++ )
			{
				if( fabs( s[i] ) > tol )
				{
					s[i] = 1.0/s[i];
				}
				else
				{
					s[i] = 0.0;
				}
			}

			for( j = 0; j < N; j++ )
			{
				for( i = 0; i < N; i++ )
				{
					r.m_Values[i][j] = 0.0;
					for( k = 0; k < N; k++ )
					{
						r.m_Values[i][j] += u[j][k]*s[k]*v[i][k];
					}
				}
			}

			SafeDelete2DArray( u, N );
			SafeDeleteArray( s );
			SafeDelete2DArray( v, N );
		}
		else
		// Using LU decomposition and backsubstitution
		{
			T** rm;
			T d, *col;
			int32* indx;

			indx = new int32 [N];
			col = new T [N];

			Create2DArray( rm, N, N, T );
			for( j = 0; j < N; j++ )
			{
				for( i = 0; i < N; i++ )
				{
					rm[j][i] = m_Values[j][i];
				}
			}

			ludcmp( rm, N, indx, &d );

			for( j = 0; j < N; j++ )
			{
				for( i = 0; i < N; i++ )
				{
					col[i] = 0;
				}
				col[j] = 1;
				lubksb( rm, N, indx, col );
				for( i = 0; i < N; i++ )
				{
					r.m_Values[i][j] = col[i];
				}
			}

			SafeDelete2DArray( rm, N );

			delete [] col;
			delete [] indx;
		}
	}

	return r;
}


template <class T>
std::ostream & operator<<( std::ostream & os, const GenericMatrix<T> & m )
{
	int32 i, j;

	for( j = 0; j < m.getNbrRows(); j++ )
	{
		for( i = 0; i < m.getNbrColumns(); i++ )
		{
			os << m[j][i] << " ";
		}
		os << std::endl;
	}

	return os;
}


template <class T>
void GenericMatrix<T>::destroy()
{
	SafeDelete2DArray( m_Values, m_NbrRows );
	m_NbrRows = m_NbrColumns = 0;
}


//************  From Numerical Recipes
//************  LU Decomposition and LU Backsubstitution

#define TINY 1.0e-20

template <class T>
void GenericMatrix<T>::ludcmp( T** a, int32 n, int32* indx, T* d ) const
{
	int32 i, imax, j, k;
	T big, dum, sum, temp;
	T *vv;

	vv = new T [n];
	for( i = 0; i < n; i++ )
	{
		vv[i] = 1.0f;
	}

	*d = 1.0;
	for( i = 0; i < n; i++ )
	{
		big = 0.0f;
		for( j = 0; j < n; j++ )
		{
			if ((temp = fabs(a[i][j])) > big) big = temp;
		}

		if( big != 0.0f ) //cout << "Singular matrix in routine ludcmp" << endl;
		{
			vv[i] = 1.0f/big;
		}
		else
		{
		}
	}

	for( j = 0; j < n; j++ )
	{
		for( i = 0; i < j; i++ )
		{
			sum = a[i][j];
			for( k = 0; k < i; k++ ) sum -= a[i][k]*a[k][j];
			a[i][j] = sum;
		}
		big = 0.0f;
		for( i = j; i < n; i++ )
		{
			sum = a[i][j];
			for( k = 0; k < j; k++ )
				sum -= a[i][k]*a[k][j];
			a[i][j]=sum;
			if( (dum=vv[i]*fabs(sum)) >= big )
			{
				big=dum;
				imax=i;
			}
		}
		if( j != imax )
		{
			for( k = 0; k < n; k++ )
			{
				dum = a[imax][k];
				a[imax][k] = a[j][k];
				a[j][k] = dum;
			}
			*d = -(*d);
			vv[imax] = vv[j];
		}
		indx[j] = imax;
		if( a[j][j] == 0.0f ) a[j][j] = (T) TINY;

		if( j != n )
		{
			dum = 1.0f / (a[j][j]);
			for( i = j+1; i < n; i++ ) a[i][j] *= dum;
		}
	}

	delete [] vv;
}


template <class T>
void GenericMatrix<T>::lubksb( T** a, int32 n, int32* indx, T* b ) const
{
	int32 i, ii = 0, ip, j;
	T sum;

	for( i = 0; i < n; i++ )
	{
		ip = indx[i];
		sum = b[ip];
		b[ip] = b[i];
		if( ii )
		{
			for( j = ii; j < i-1; j++ ) sum -= a[i][j]*b[j];
		}
		else if( sum ) ii = i;
		b[i] = sum;
	}
	for( i = n-1; i >= 0; i-- )
	{
		sum = b[i];
		for( j = i+1; j < n; j++ ) sum -= a[i][j]*b[j];
		b[i] = sum/a[i][i];
	}
}
