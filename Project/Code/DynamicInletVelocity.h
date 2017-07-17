#include "helper.h"
#include "initLB.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#ifndef _DYNAMIC_INLET_VELOCITY_H_
#define _DYNAMIC_INLET_VELOCITY_H_

void readInletVelocity( std::vector<double> &TimeVelocity,
						int &LBStep,
						int &EndTimeStep );

void interpolateInletVelocity(  double &NewVelocity,
							    std::vector<double> &TimeVelocity,
							    int TimeStep,
								int LBStep,
								int EndTimeStep );

#endif