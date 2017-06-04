close all
clear
clc

%{ 
                                Table
______________________________________________________________________
||				||					||			||					||
||	Re = 0.1	||	Ux = 0.04655	||	L = 20	||	tau = 1.8		||
||______________||__________________||__________||__________________||
||				||					||			||					||
||	Re = 1		||	Ux = 0.06		||	L = 46	||	tau = 0.995		||
||______________||__________________||__________||__________________||
||				||					||			||					||
||	Re = 10		||	Ux = 0.08		||	L = 155	||	tau = 0.798		||
||______________||__________________||__________||__________________||
||				||					||			||					||
||	Re = 100	||	Ux = 0.085		||	L = 460	||	tau = 0.6		||
||______________||__________________||__________||__________________||
||				||					||			||					||
||	Re = 1000	||	Ux = 1.0		||	L = 850	||	tau = 0.5255 	||
||______________||__________________||__________||__________________||
%}


% compute Reynolds Number based on L - length, U - velocity, V - viscosity
getReynoldsNumber = @( L, V, U ) ( U^2 * L ) ./ V;  

%checkRe = @(U,L,T) 3.0 * ( U^2 * L ) / ( T -  0.5 );

% square of the speed of sound
CS_SQR = 1.0 / 3.0;

% Velocity magnitude
U = 1.0;

% range of the length and tau
length = [ 20.0:10.0:1000.0 ];
tau = [ 0.5:0.05:1.8 ];

[L,T] = meshgrid( length, tau );
V = CS_SQR * ( T -  0.5 );

% compute the Reynolds Number Field
ReynoldsNumber = getReynoldsNumber( L, V, U );

% display the results
surf( L, T, ReynoldsNumber );
xlabel('Length');
ylabel('Tau');
zlabel('Re');
colorbar();

%% Compute the required memory in GB

ReynoldsNumberTarget = [ 0.1 1.0 10.0 100.0 1000.0 ];
xlength = [ 20 46 155 460 850 ]

NumberOfFields = 2;
NumberOfFlags = 1;
D3Q19 = 19;
double = 8;
int = 4;

FieldsAllocationMemory = NumberOfFields * D3Q19 * xlength.^3 * double / ( 1024 ^ 3 );
FlagsAllocationMemory = NumberOfFlags * xlength.^3 * int / ( 1024 ^ 3 );

Memory = FieldsAllocationMemory + FlagsAllocationMemory



