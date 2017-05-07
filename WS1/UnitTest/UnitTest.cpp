#include <iostream>
#include "../helper.h"
#include <gtest/gtest.h>
#include <string>
#include "Matrix.h"
#include <ctime>


// HelperInterface - the set of functions that has to be tested

// AbsMaxSearcher - the function defined by Ravi that has to return the 
// absolute maximum within a two dimensional array

TEST( TestHelperInterface, AbsMaxSearcher_SquareMatrix ) {

	// test with a positive test number
    Matrix SquareMatrix(10,10);

	double MaximumValue = 100.001;
	SquareMatrix(4,4) = MaximumValue;


	double Result = absoluteMaxInArray( SquareMatrix.getMatrixPointer(),
										(int)SquareMatrix.getRowNumber(),
								    	(int)SquareMatrix.getColumnNumber() );
	
	EXPECT_EQ( Result, MaximumValue );

}


TEST( TestHelperInterface, AbsMaxSearcher_RectangularMatrix_OneColumn ) {

	// test with a negative test number
    Matrix RectangularMatrix(100,1);

	double MaximumValue = -220.022;
	RectangularMatrix(99,0) = MaximumValue;


	double Result = absoluteMaxInArray( RectangularMatrix.getMatrixPointer(),
										(int)RectangularMatrix.getRowNumber(),
								    	(int)RectangularMatrix.getColumnNumber() );
	
	EXPECT_EQ( Result, fabs(MaximumValue) );
}


TEST( TestHelperInterface, AbsMaxSearcher_RectangularMatrix_OneRow ) {

	// test with the test number is equal to zero
    Matrix RectangularMatrix(1,1000);

	double MaximumValue = 0.0;

	double Result = absoluteMaxInArray( RectangularMatrix.getMatrixPointer(),
										(int)RectangularMatrix.getRowNumber(),
								    	(int)RectangularMatrix.getColumnNumber() );
	
	EXPECT_EQ( Result, fabs(MaximumValue) );
}


TEST( TestHelperInterface, AbsMaxSearcher_NegativeRandomValue_Win ) {

	// test with the test number is equal to zero

	unsigned Rows = 400;
	unsigned Columns = 1000;
    Matrix RectangularMatrix(Rows,Columns);

	// define the range of random values
	double MIN = -1000.0;
	double MAX = 500.0;
	double RANGE = MAX - MIN;

	// fill in the matrix with randome variables
	std::srand(std::time(0));
	double tmp = 0.0;
	for ( int i = 0; i < Rows; ++i ) {
		for( int j = 0; j < Columns; ++j ) {
			tmp = (double)rand() / RAND_MAX;
			RectangularMatrix(i,j) = ( tmp * RANGE ) + MIN;
		}
	}

	// Set the pick value
	double PickValue = -1000.25;
	RectangularMatrix(255,735) = PickValue;

	double Result = absoluteMaxInArray( RectangularMatrix.getMatrixPointer(),
										(int)RectangularMatrix.getRowNumber(),
								    	(int)RectangularMatrix.getColumnNumber() );
	
	EXPECT_EQ( Result, fabs(PickValue) );
}


TEST( TestHelperInterface, AbsMaxSearcher_PositiveRandomValue_Win ) {

	// test with the test number is equal to zero

	unsigned Rows = 400;
	unsigned Columns = 1000;
    Matrix RectangularMatrix(Rows,Columns);

	// define the range of random values
	double MIN = -1000.0;
	double MAX = 500.0;
	double RANGE = MAX - MIN;

	// fill in the matrix with randome variables
	std::srand(std::time(0));
	double tmp = 0.0;
	for ( int i = 0; i < Rows; ++i ) {
		for( int j = 0; j < Columns; ++j ) {
			tmp = (double)rand() / RAND_MAX;
			RectangularMatrix(i,j) = ( tmp * RANGE ) + MIN;
		}
	}

	// Set the pick value
	double PickValue = 1000.01;
	RectangularMatrix(255,735) = PickValue;

	double Result = absoluteMaxInArray( RectangularMatrix.getMatrixPointer(),
										(int)RectangularMatrix.getRowNumber(),
								    	(int)RectangularMatrix.getColumnNumber() );
	
	EXPECT_EQ( Result, PickValue );
}


// RUN ALL TEST
int main(int argc, char **argv) {
    
    try {
        testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    }
    catch ( std::string anError ) {
        std::cout << anError << std::endl;
    }
    catch ( ... ) {
        std::cout << "Undocumented Error" << std::endl;
    }
}

