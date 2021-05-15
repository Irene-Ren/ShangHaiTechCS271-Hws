#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <cstdlib>
#include <vector>
#include <algorithm>

///////////////////////////////////////
// struct MatrixElement
// elements of Matrix class, represent non-zero entry in a matrix
struct MatrixElement
{
	int row, col;	// row and column of the element
	double value;	// value of the element

	// constructor
	MatrixElement(int r, int c, double v)
		: row(r), col(c), value(v) {}

	// compare function for Matrix::SortMatrix() function
	static bool order (MatrixElement e1, MatrixElement e2)
	{
		if (e1.row < e2.row) return true;
		if (e1.row == e2.row) return (e1.col < e2.col);
		return false;
	}
};

// class declaration
typedef std::vector<MatrixElement> MatrixElementList;

// class Matrix definition
class Matrix 
{
private:
	int m, n;	// size of matrix (m = # of rows, n # of columns)
	int * rowIndex;				// row indice of non-zero entries

	// fields for CG method
	double* diagInv;
	double* r;
	double* r2;
	double* d;
	double* d2;
	double* q;
	double* s;
	double* s2;

public:
	MatrixElementList elements;	// list of non-zero entries
	// constructor & destructor
	Matrix(int m, int n) : m(m), n(n) 
	{
		rowIndex = new int[m+1]; 
		diagInv = new double[m];
		r = new double[m];
		r2 = new double[m];
		d = new double[m];
		d2 = new double[m];
		q = new double[m];
		s = new double[m];
		s2 = new double[m];
	}
	~Matrix() 
	{
		delete[] rowIndex; 
		delete[] r;
		delete[] r2;
		delete[] d;
		delete[] d2;
		delete[] q;
		delete[] s;
		delete[] s2;
	}

	/////////////////////////////////////
	// function AddElement
	// add a new entry into the matrix
	void AddElement(int row, int col, double value)
	{
		elements.push_back(MatrixElement(row, col, value));
	}

	/////////////////////////////////////
	// function SortMatrix
	// sort the matrix elements after you add ALL elements into the matrix
	void SortMatrix()
	{
		sort(elements.begin( ), elements.end( ), MatrixElement::order);

		for (int i=0; i<m+1; i++)
			rowIndex[i] = 0;
		for (int i=0; i<(int)elements.size(); i++)
			rowIndex[elements[i].row + 1] = i + 1;

		for (int i=0; i<m; i++)
			diagInv[i] = 0;
		for (int i=0; i<(int)elements.size(); i++)
			if (elements[i].row == elements[i].col)
				diagInv[elements[i].row] = 1.0 / elements[i].value;
	}


	/////////////////////////////////////
	// function Multiply
	// compute A * xIn = xOut
	// the arrays pointed by xIn and xOut have to be pre-allocated 
	// and have enough space
	void Multiply(double* xIn, double* xOut)
	{
		for (int i=0; i<m; i++)
		{
			double sum = 0;
			for (int j=rowIndex[i]; j<rowIndex[i+1]; j++)
				sum += elements[j].value * xIn[elements[j].col];
			xOut[i] = sum;
		}
	}
	/////////////////////////////////////
	// Multiply PreMultiply
	// compute xIn * A = xOut
	// the arrays pointed by xIn and xOut have to be pre-allocated 
	// and have enough space
	void PreMultiply(double* xIn, double* xOut)
	{
		for (int i=0; i<n; i++) xOut[i] = 0;

		for (int i=0; i<m; i++)
		{
			for (int j=rowIndex[i]; j<rowIndex[i+1]; j++)
				xOut[elements[j].col] += elements[j].value * xIn[i];
		}
	}
	double VectorDotProduct(double* v1, double* v2, int n)
	{
		double result = 0.0;
		for (int i = 0; i < n; i++)
		{
			result += v1[i] * v2[i];
		}
		return result;
	}
	void VectorMinus(double* v1, double* v2, double* result, int n)
	{
		for (int i = 0; i < n; i++)
		{
			result[i] = v1[i] - v2[i];
		}
	}
	void VectorAddEqual(double* self, double* v2, int n)
	{
		for (int i = 0; i < n; i++)
		{
			self[i] += v2[i];
		}
	}
	void VectorMultiply(double* v, double num, double* result, int n)
	{
		for (int i = 0; i < n; i++)
		{
			result[i] = v[i] * num;
		}
	}
	/**********************************************/
	/* function: BCG                              */
	/* description: solve Ax = b for unknowns x   */
	/**********************************************/
	void BCG(double* b, double* x, int maxIterCnt, double tolerance)
	{
		/*************************/
		/* insert your code here */
		/*************************/
		int iter = 0;
		double error = 0.0;

		// Calculate A*x and put the result in vector r (double*)
		Multiply(x, r);
		// Setup for the initial values for r, r^T(r2), d, d^T(d2)
		// Note that 
		VectorMinus(b, r, r, m);
		VectorMinus(b, r, r2, m);
		VectorMinus(b, r, d, m);
		VectorMinus(b, r, d2, m);

		// The numerator of alpha
		double numerator_a = VectorDotProduct(r2, r, m);

		// Setup initial error value
		error = abs(numerator_a);

		while (iter < maxIterCnt)
		{
			if (abs(numerator_a) < tolerance*error)
			{
				break;
			}
			// Save A*d in vector q
			Multiply(d, q);

			// begin to calculate alpha
			// alpha(i) = Dot(r(i),r(i))/d(i)*A*d(i)
			double denominator_a = VectorDotProduct(d2, q, m);
			double alpha = numerator_a / denominator_a;

			// calculate x(i)
			VectorMultiply(d, alpha, s, m); //Temporary saver s
			VectorAddEqual(x, s, m);
			if (iter % 50 == 0)
			{
				Multiply(x, r);
				VectorMinus(b, r, r, m);
			}
			else
			{
				VectorMultiply(q, -alpha, s, m);
				VectorAddEqual(r, s, m);
			}
			PreMultiply(d2, q);
			VectorMultiply(q, -alpha, s, m);
			VectorAddEqual(r2, s, m);

			//Calculate for beta
			double denominator_b = numerator_a;
			numerator_a = VectorDotProduct(r2, r, m);
			double beta = numerator_a / denominator_b;

			//Calculate for d and d2
			VectorMultiply(d, -beta, s, m);
			VectorMinus(r, s, d, m);
			VectorMultiply(d2, -beta, s2, m);
			VectorMinus(r2, s2, d2, m);

			iter++;
		}
	}

	// friend operators
	friend ostream & operator<< (ostream & out, const Matrix & r) 
	{
		for (int i=0; i<r.m; i++)
		{
			for(int j=r.rowIndex[i]; j<r.rowIndex[i+1]; j++)
				out << r.elements[j].value << " ";
			out << endl;
		}
		return out;
	}
};

#endif __MATRIX_H__
