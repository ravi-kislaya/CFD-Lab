#include <math.h>

#ifndef _LBDEFINITIONS_H_
#define _LBDEFINITIONS_H_


  static const int LATTICEVELOCITIES[19][3] = { { 0 ,-1 ,-1 } ,
                                                {-1 , 0 ,-1 } ,
                                                { 0 , 0 ,-1 } ,
                                                { 1 , 0 ,-1 } ,
                                                { 0 , 1 ,-1 } ,
                                                {-1 ,-1 , 0 } ,
                                                { 0 ,-1 , 0 } ,
                                                { 1 ,-1 , 0 } ,
                                                {-1 , 0 , 0 } ,
                                                { 0 , 0 , 0 } ,
                                                { 1 , 0 , 0 } ,
                                                {-1 , 1 , 0 } ,
                                                { 0 , 1 , 0 } ,
                                                { 1 , 1 , 0 } ,
                                                { 0 ,-1 , 1 } ,
                                                {-1 , 0 , 1 } ,
                                                { 0 , 0 , 1 } ,
                                                { 1 , 0 , 1 } ,
                                                { 0 , 1 , 1 }
                                              };


  static const double LATTICEWEIGHTS[19] = {  1.0 / 36.0,
                                              1.0 / 36.0,
                                              2.0 / 36.0,
                                              1.0 / 36.0,
                                              1.0 / 36.0,
                                              1.0 / 36.0,
                                              2.0 / 36.0,
                                              1.0 / 36.0,
                                              2.0 / 36.0,
                                              12.0 / 36.0,
                                              2.0 / 36.0,
                                              1.0 / 36.0,
                                              2.0 / 36.0,
                                              1.0 / 36.0,
                                              1.0 / 36.0,
                                              1.0 / 36.0,
                                              2.0 / 36.0,
                                              1.0 / 36.0,
                                              1.0 / 36.0,
                                           };


  // C_S - is the speed of sound which is equal to 1.0 / sqrt ( 3.0 ) = 0.577350269
  static const double C_S = 0.577350269;
  static const int Dimensions = 3;
  static const int Vel_DOF = 19;

  static const int CX[ 19 ] = { 0 , -1 , 0 , 1 , 0 , -1 , 0 , 1 , -1 , \
												        0 , 1 , -1 , 0 , 1 , 0 , -1 , 0 , 1 , 0 };

	static const int CY[ 19 ] = { -1 , 0 , 0 , 0 , 1 , -1 , -1 , -1 , 0 , \
													       0 , 0 , 1 , 1 , 1 , -1 , 0 , 0 , 0 , 1 };

	static const int CZ[ 19 ] = { -1 , -1 , -1 , -1 , -1 , 0 , 0 , 0 , 0 , \
						 							       0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 };

#endif
