#include <math.h>

#ifndef _LBDEFINITIONS_H_
#define _LBDEFINITIONS_H_

  double Initialization_Value_0 = 12.0 / 36.0 ;
  double Initialization_Value_1 =  2.0 / 36.0 ;
  double Initialization_Value_2 =  1.0 / 36.0 ;
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
  static const double LATTICEWEIGHTS[19] = {    Initialization_Value_2 ;
                                                Initialization_Value_2 ;
                                                Initialization_Value_1 ;
                                                Initialization_Value_2 ;
                                                Initialization_Value_2 ;
                                                Initialization_Value_2 ;
                                                Initialization_Value_1 ;
                                                Initialization_Value_2 ;
                                                Initialization_Value_1 ;
                                                Initialization_Value_0 ;
                                                Initialization_Value_1 ;
                                                Initialization_Value_2 ;
                                                Initialization_Value_1 ;
                                                Initialization_Value_2 ;
                                                Initialization_Value_2 ;
                                                Initialization_Value_2 ;
                                                Initialization_Value_1 ;
                                                Initialization_Value_2 ;
                                                Initialization_Value_2 ;
                                           };
  // C_S - is the speed of sound which is equal to 1.0 / sqrt ( 3.0 ) = 0.577350269
  static const double C_S = 0.577350269;
  static const int Dimensions = 3;
  static const int Vel_DOF = 19;

#endif
