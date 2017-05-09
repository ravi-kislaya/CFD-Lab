#ifndef __MATRIX__
#define __MATRIX__


class Matrix {
    public:
        Matrix() : m_Rows(0), m_Columns(0), m_Matrix(0) {}
        Matrix( unsigned Rows, unsigned Columns );
        ~Matrix();
        
        double& operator()( int aRow, int aColumn );
		
		unsigned getRowNumber();
		unsigned getColumnNumber();
		double** getMatrixPointer();

	private:
        unsigned m_Rows;
        unsigned m_Columns;
        double** m_Matrix;

};


#endif
