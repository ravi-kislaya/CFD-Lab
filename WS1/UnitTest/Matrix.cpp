#include <iostream>
#include "Matrix.h"


Matrix::Matrix( unsigned Rows, unsigned Columns ) : m_Rows(Rows),
                                                    m_Columns(Columns) {
    
    m_Matrix = new double*[m_Rows];
    for( int i = 0; i < m_Rows; ++i ) {
        m_Matrix[i] = new double[m_Columns];
    }    
                                                        
    double InitialValue = 0.0;
    for ( int i = 0; i < m_Rows; ++i ) {
        for ( int j = 0; j < m_Columns; ++j ) {
            m_Matrix[i][j] = InitialValue;
        }
    }        
}


Matrix::~Matrix() {
    
    if ( m_Matrix ) {
        for ( int i = 0; i < m_Rows; ++i ) delete [] m_Matrix[i];   
    }
}


double& Matrix::operator()( int aRow, int aColumn ) {

    // throw an error if the indexes out of the bound
    if ( ( aRow > m_Rows ) || ( aColumn > m_Columns ) ) {
        std::string Error("Matirx::setElement: an index is out of the bound");
        throw Error;
    }
    return m_Matrix[aRow][aColumn];

}


unsigned Matrix::getRowNumber() { return m_Rows; };


unsigned Matrix::getColumnNumber() { return m_Columns; };


double** Matrix::getMatrixPointer() { return m_Matrix; };
