#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <string>
#include <mpi.h>

#include "helper.h"
#include "DataStructure.h"

/* ----------------------------------------------------------------------- */
/*                             auxiliary functions                         */
/* ----------------------------------------------------------------------- */
void allocateFields( double** collideField,
					 double** streamField,
				   	 int** flagField,
				  	 int** IdField,
				  	 unsigned* Length ) {

    int CellNumber = ( Length[ 0 ] + 2 )
                    * ( Length[ 1 ] + 2 )
                    * ( Length[ 2 ] + 2 );

    *collideField = ( double* ) calloc( Vel_DOF * CellNumber, sizeof( double ) );
    *streamField = ( double* ) calloc( Vel_DOF * CellNumber, sizeof( double ) );
    *flagField = ( int* ) calloc( CellNumber, sizeof(int) );
    *IdField = ( int* ) calloc( CellNumber, sizeof( int ) );

}


void getLengthFromString( unsigned* Length, std::string String ) {
	std::string Buffer = "";
	const char DELIMITER = ' ';

	std::vector<std::string> Container;
    for( std::string::iterator Character = String.begin();
		 Character != String.end();
		 ++Character ) {

		if ( ( *Character ) == DELIMITER ) {
			Container.push_back( Buffer );
			Buffer = "";
		}

		Buffer += ( *Character );
    }
	Container.push_back( Buffer );

    Length[ 0 ] = atoi( Container[ 0 ].c_str() );
	Length[ 2 ] = atoi( Container[ 1 ].c_str() );
	Length[ 1 ] = atoi( Container[ 2 ].c_str() );

}


int computeCPUCoordinateX( int* Proc,
                           int Rank ) {

 //   return ( unsigned ) ( Rank % Proc[0] );
    return ( int ) ( ( Rank % ( Proc[0] * Proc[1] ) ) % Proc[ 0 ] );
}


int computeCPUCoordinateY( int* Proc,
                           int Rank ) {

    //return ( ( ( unsigned ) ( Rank / Proc[0] ) ) % Proc[1] );
    return ( int ) ( ( Rank % ( Proc[0] * Proc[1] ) ) / Proc[ 0 ] );
}


int computeCPUCoordinateZ( int* Proc,
                           int Rank ) {

	return ( int ) ( Rank / ( Proc[0] * Proc[1] ));
}


int getGlobalCPUIndex( int i, int j, int k, int* Proc ) {
    return k * Proc[ 0 ] * Proc[ 1 ] + j * Proc[ 0 ] + i;
}


template< class T >
T min( T a, T b ) {
    if( a < b ) return a;
    return b;
}

template< class T >
T max( T a, T b ) {
    if( a > b ) return a;
    return b;
}


/* ----------------------------------------------------------------------- */
/*                         local auxiliary functions                       */
/* ----------------------------------------------------------------------- */

clock_t last_timer_reset;

int min_int( const int n1, const int n2 )
{
    if( n1 < n2 ) return n1;
    return n2;
}



/* ----------------------------------------------------------------------- */
/*                             read datafile                               */
/* ----------------------------------------------------------------------- */


int read_parameters( const char *INPUT_FILE_NAME,        /* the name of the data file */
                     unsigned* Length,                   /* number of cells along x direction */
					 int* PROC,							 /* CPU layout */
                     double *tau,                        /* relaxation time */
                     double *WallVelocity,               /* lid velocity along all direction*/
                     double *InletVelocity,              /* Inlet velocity along all direction */
                     double *DeltaDensity,               /* density difference */
                     unsigned *timesteps,                /* number of simulation time steps */
                     unsigned *timestepsPerPlotting,     /* number of visualization time steps */
                     int CPU_RANK = 0 ) {  

   int TempLength[ 3 ] = { 0 };
   read_int( INPUT_FILE_NAME, "xlength", &TempLength[ 0 ], CPU_RANK );
   read_int( INPUT_FILE_NAME, "ylength", &TempLength[ 1 ], CPU_RANK );
   read_int( INPUT_FILE_NAME, "zlength", &TempLength[ 2 ], CPU_RANK );

   if (   ( TempLength[ 0 ] <= 1 )
		|| ( TempLength[ 1 ] <= 1 )
		|| ( TempLength[ 2 ] <= 1 ) ) {
		std::string Error = "ERROR: provided geometrical parameters are wrong";
		throw Error;
	}
	else {
		Length[ 0 ] = ( unsigned )TempLength[ 0 ];
		Length[ 1 ] = ( unsigned )TempLength[ 1 ];
		Length[ 2 ] = ( unsigned )TempLength[ 2 ];
	}


    read_int( INPUT_FILE_NAME, "iProc", &PROC[ 0 ], CPU_RANK );
    read_int( INPUT_FILE_NAME, "jProc", &PROC[ 1 ], CPU_RANK );
    read_int( INPUT_FILE_NAME, "kProc", &PROC[ 2 ], CPU_RANK );


#ifndef TEST

	int PROCESSORS_NUMBER = 0;
 	MPI_Comm_size(MPI_COMM_WORLD, &PROCESSORS_NUMBER);

	int LAYOUT_REQUEST = PROC[ 0 ] * PROC[ 1 ] * PROC[ 2 ];
	if ( PROCESSORS_NUMBER != LAYOUT_REQUEST ) {
       std::string Error = "ERROR: processor topology doen't match to the processors number";
 	   throw Error;
	}

#endif


   read_double( INPUT_FILE_NAME, "tau", tau, CPU_RANK );
   if ( ( *tau ) <= 0.5 || ( *tau >= 2.0 ) ) {
	   std::string Error = "ERROR: tau should be between greater than 0.5 and  less than 2.0";
	   throw Error;
   }

   double VelocityMagnitude = 0.0;

   read_double( INPUT_FILE_NAME, "U", &WallVelocity[ 0 ], CPU_RANK );
   read_double( INPUT_FILE_NAME, "V", &WallVelocity[ 1 ], CPU_RANK );
   read_double( INPUT_FILE_NAME, "W", &WallVelocity[ 2 ], CPU_RANK );

   VelocityMagnitude = WallVelocity[ 0 ] * WallVelocity[ 0 ]
					 + WallVelocity[ 1 ] * WallVelocity[ 1 ]
					 + WallVelocity[ 2 ] * WallVelocity[ 2 ];
   if ( VelocityMagnitude >= 0.01 ) {
	   std::string Error = "ERROR: Lid Velocity should be small, LBM fails at high Mach Number";
	   throw Error;
   }


   read_double( INPUT_FILE_NAME, "Uin", &InletVelocity[ 0 ], CPU_RANK );
   read_double( INPUT_FILE_NAME, "Vin", &InletVelocity[ 1 ], CPU_RANK );
   read_double( INPUT_FILE_NAME, "Win", &InletVelocity[ 2 ], CPU_RANK );

   VelocityMagnitude = InletVelocity[ 0 ] * InletVelocity[ 0 ]
   					 + InletVelocity[ 1 ] * InletVelocity[ 1 ]
   					 + InletVelocity[ 2 ] * InletVelocity[ 2 ];

   if ( VelocityMagnitude >= 0.01 ) {
	   std::string Error = "ERROR: Inlet Velocity should be small, LBM fails at high Mach Number";
	   throw Error;
   }


   read_double( INPUT_FILE_NAME, "DeltaDensity", DeltaDensity, CPU_RANK );
   if ( ( *DeltaDensity ) > 0.05 ) {
	   std::string Error = "ERROR: Delta Density should be small, LBM fails at high Mach Number";
	   throw Error;
   }

   int TemporaryVariable = 0;
   read_int( INPUT_FILE_NAME, "timesteps", &TemporaryVariable, CPU_RANK );
   if ( TemporaryVariable <= 0 ) {
	   std::string Error = "ERROR: Time Steps cannot be negative";
	   throw Error;
   }
   else {
		( *timesteps ) = TemporaryVariable;
   }


   read_int( INPUT_FILE_NAME, "timestepsPerPlotting", &TemporaryVariable, CPU_RANK );

   if ( TemporaryVariable <= 0 ) {
	   std::string Error = "ERROR: Time Steps Per Plotting cannot be negative";
	   throw Error;
   }
   else {
	 	( *timestepsPerPlotting ) = ( TemporaryVariable );
   }


   return 1;
}


void copyToVector( std::list<Fluid*> &aList, std::vector<Fluid*> &aVector ) {

	aVector.resize( aList.size() );

	unsigned Counter = 0;
	for ( std::list<Fluid*>::iterator Iterator = aList.begin();
 		  Iterator != aList.end();
 		  ++Iterator, ++Counter ) {
		aVector[ Counter ] = ( *Iterator );
	}

}


void errhandler( int nLine, const char *szFile, const char *szString )
{
    int err = errno;

    fprintf( ERROUT, "%s:%d Error : %s", szFile, nLine, szString );
    fprintf( ERROUT, "\n" );

    /* if an error within the c-library occured, an error code can be   */
    /* found in the global variable err                                 */
    if( err != 0 )
    {
        fprintf( ERROUT, "C-Lib   errno    = %d\n", err);
        fprintf( ERROUT, "C-Lib   strerror = %s\n", strerror( err ) );
    }
    exit(1);
}


/*  for comfort */
#define READ_ERROR(szMessage, szVarName, szFileName, nLine) \
  { char szTmp[80]; \
    if( nLine ) \
        sprintf( szTmp, " %s  File: %s   Variable: %s  Line: %d", szMessage, szFileName, szVarName, nLine ); \
    else \
        sprintf( szTmp, " %s  File: %s   Variable: %s ", szMessage, szFileName, szVarName); \
    ERROR( szTmp ); \
  }


/* --------------------------------------------------------------------------*/
/* The function searches the datafile fh for the line defining the variable  */
/* szVarName and returns the respctive string including the value of the     */
/* variable. If there's no appropriate line within the datafile, the program */
/* stops with an error messsage.                                             */
/* ATTENTION: The pointer returned refers to a static variable within the    */
/* function. To maintain the string over several program calls, it has to be */
/* copied!!!                                                                 */
/*                                                                           */
char* find_string( const char* szFileName, const char *szVarName ) {
    int nLine = 0;
    unsigned i;
    FILE *fh = NULL;

    static char szBuffer[MAX_LINE_LENGTH];      /* containes the line read  */
                                                /* from the datafile        */

    char* szLine = szBuffer;
    char* szValue = NULL;
    char* szName = NULL;

    /* open file */
    fh = fopen( szFileName, "rt" );
    if( fh == 0 )
        READ_ERROR("Could not open file", szVarName, szFileName, 0);

    /* searching */
    while( ! feof(fh) )
    {
        if(fgets( szLine, MAX_LINE_LENGTH, fh ));
        ++nLine;

        /* remove comments */
        for( i = 0; i < strlen(szLine); i++)
            if( szLine[i] == '#' )
            {
                szLine[i] = '\0'; /* Stringende setzen */
                break;
            }

        /* remove empty lines */
        while( isspace( (int)*szLine ) && *szLine) ++szLine;
        if( strlen( szLine ) == 0) continue;

        /* now, the name can be extracted */
        szName = szLine;
        szValue = szLine;
        while( (isalnum( (int)*szValue ) || *szValue == '_') && *szValue) ++szValue;

        /* is the value for the respective name missing? */
        if( *szValue == '\n' || strlen( szValue) == 0)
            READ_ERROR("wrong format", szName, szFileName, nLine);

        *szValue = 0;           /* complete szName! at the right place */
        ++szValue;

        /* read next line if the correct name wasn't found */
        if( strcmp( szVarName, szName)) continue;

        /* remove all leading blnkets and tabs from the value string  */
        while( isspace( (int)*szValue) ) ++szValue;
        if( *szValue == '\n' || strlen( szValue) == 0)
            READ_ERROR("wrong format", szName, szFileName, nLine);

        fclose(fh);
        return szValue;
    }

    READ_ERROR("variable not found", szVarName, szFileName, nLine);

    return NULL;                /* dummy to satisfy the compiler  */
}

void read_string( const char* szFileName,
                  const char* szVarName,
                  char*   pVariable,
                  int CPU_RANK = 0 ) {
    char* szValue = NULL;       /* string containg the read variable value */

    if( szVarName  == 0 )  ERROR("null pointer given as variable name" );
    if( szFileName == 0 )  ERROR("null pointer given as filename" );
    if( pVariable  == 0 )  ERROR("null pointer given as variable" );

    if( szVarName[0] == '*' )
        szValue = find_string( szFileName, szVarName +1 );
    else
        szValue = find_string( szFileName, szVarName );

    if( sscanf( szValue, "%s", pVariable) == 0)
        READ_ERROR("wrong format", szVarName, szFileName,0);

    if ( CPU_RANK == MASTER_CPU ) {

        printf( "File: %s\t\t%s%s= %s\n",
                szFileName,
                szVarName,
                &("               "[min_int( (int)strlen(szVarName), 15)]),
                pVariable );

    }
}

void read_int( const char* szFileName, 
               const char* szVarName, 
               int* pVariable,
               int CPU_RANK = 0 ) {
    
    char* szValue = NULL;       /* string containing the read variable value */

    if( szVarName  == 0 )  ERROR("null pointer given as varable name" );
    if( szFileName == 0 )  ERROR("null pointer given as filename" );
    if( pVariable  == 0 )  ERROR("null pointer given as variable" );

    if( szVarName[0] == '*' )
        szValue = find_string( szFileName, szVarName +1 );
    else
        szValue = find_string( szFileName, szVarName );

    if( sscanf( szValue, "%d", pVariable) == 0)
        READ_ERROR("wrong format", szVarName, szFileName, 0);

    
    if ( CPU_RANK == MASTER_CPU ) {
        printf( "File: %s\t\t%s%s= %d\n", 
                szFileName,
                szVarName,
                &("               "[min_int( (int)strlen(szVarName), 15)]),
                *pVariable );
    }
}


void read_unsigned( const char* szFileName, 
                    const char* szVarName, 
                    unsigned* pVariable, 
                    int CPU_RANK ) {

    char* szValue = NULL;       /* string containing the read variable value */

    if( szVarName  == 0 )  ERROR("null pointer given as varable name" );
    if( szFileName == 0 )  ERROR("null pointer given as filename" );
    if( pVariable  == 0 )  ERROR("null pointer given as variable" );

    if( szVarName[0] == '*' )
        szValue = find_string( szFileName, szVarName +1 );
    else
        szValue = find_string( szFileName, szVarName );

    if( sscanf( szValue, "%u", pVariable) == 0)
        READ_ERROR("wrong format", szVarName, szFileName, 0);


    if ( CPU_RANK == MASTER_CPU ) {
        printf( "File: %s\t\t%s%s= %u\n", 
                szFileName,
                szVarName,
                &("               "[min_int( (unsigned)strlen(szVarName), 15)]),
                *pVariable );
    }
}


void read_double( const char* szFileName, 
                  const char* szVarName, 
                  double* pVariable,
                  int CPU_RANK ) {

    char* szValue = NULL;       /* String mit dem eingelesenen Variablenwert */

    if( szVarName  == 0 )  ERROR("null pointer given as varable name" );
    if( szFileName == 0 )  ERROR("null pointer given as filename" );
    if( pVariable  == 0 )  ERROR("null pointer given as variable" );

    if( szVarName[0] == '*' )
        szValue = find_string( szFileName, szVarName +1 );
    else
        szValue = find_string( szFileName, szVarName );

    if( sscanf( szValue, "%lf", pVariable) == 0)
        READ_ERROR("wrong format", szVarName, szFileName, 0);

    if ( CPU_RANK == MASTER_CPU ) {

        printf( "File: %s\t\t%s%s= %f\n",
                szFileName,
                szVarName,
                &("               "[min_int( (int)strlen(szVarName), 15)]),
                *pVariable );
    }
}


/* ----------------------------------------------------------------------- */
/*                   write matrices to a file                              */
/* ----------------------------------------------------------------------- */

void write_matrix( const char* szFileName,     /* filename */
                   double **m,                 /* matrix */
                   int nrl,                    /* first column */
                   int nrh,                    /* last column */
                   int ncl,                    /* first row */
                   int nch,                    /* last row */
                 double Length,               /* size of the geometry in */
                                               /* x-direction */
                 double ylength,               /* size of the geometry in */
                                               /* y-direction  */
                   int fFirst )                /* 0 == append, else overwrite*/
{
   int i, j;
   FILE * fh = 0;
   int nSize = (nrh-nrl+1) * (nch-ncl+1);
   float *tmp = (float *)malloc( (size_t)(nSize) * sizeof(float));
   int k = 0;

   if( fFirst )                         /* first call of the function ? */
   {
       fh = fopen( szFileName, "w");    /* overwrite file/write new file */
       if( fh == NULL )                 /* opening failed ? */
       {
           char szBuff[80];
           sprintf( szBuff, "Outputfile %s cannot be created", szFileName );
           ERROR( szBuff );
       }

/*       fprintf( fh,"%f\n%f\n%d\n%d\n%d\n%d\n", Length, ylength, nrl, nrh, ncl, nch ); */
   }
   else
   {
       fh = fopen( szFileName ,"a");    /* append to the file */
       if( fh == NULL )                 /* opening failed ? */
       {
           char szBuff[80];
           sprintf( szBuff, "Outputfile %s cannot be opened", szFileName );
           ERROR( szBuff );
       }
   }

   for( j = ncl; j <= nch; j++)
       for( i = nrl; i <= nrh; i++)
           tmp[k++] = (float)m[i][j];

   fwrite( tmp, sizeof(float), (size_t)nSize, fh);

   if( fclose(fh) )
   {
       char szBuff[80];
       sprintf( szBuff, "Outputfile %s cannot be closed", szFileName );
       ERROR( szBuff );
   };

   free( tmp );
}


void read_matrix( const char* szFileName,     /* filename */
                  double **m,                 /* matrix */
                  int nrl,                    /* first column */
                  int nrh,                    /* last column */
                  int ncl,                    /* first row */
                  int nch                     /* last row */
                 )
{
   int i, j;
   FILE * fh = 0;
   int nSize = (nrh-nrl+1) * (nch-ncl+1);
   float *tmp = (float *)malloc( (size_t)(nSize) * sizeof(float));
   int k = 0;

       fh = fopen( szFileName, "r");    /* overwrite file/write new file */
       if( fh == NULL )                 /* opening failed ? */
       {
           char szBuff[80];
           sprintf( szBuff, "Can not read file %s !!!", szFileName );
           ERROR( szBuff );
       }


   if(fread( tmp, sizeof(float), (size_t)nSize, fh));

   for( j = ncl; j <= nch; j++)
       for( i = nrl; i <= nrh; i++)
           m[i][j]=tmp[k++];

   if( fclose(fh) )
   {
       char szBuff[80];
       /*orig bug:
       sscanf( szBuff, "Inputfile %s cannot be closed", szFileName );*/
       sprintf( szBuff, "Inputfile %s cannot be closed", szFileName );
       ERROR( szBuff );
   };

   free( tmp );
}


/* ----------------------------------------------------------------------- */
/*                      general matrix functions                           */
/* ----------------------------------------------------------------------- */

/*  allocates storage for a matrix                                         */
double **matrix( int nrl, int nrh, int ncl, int nch )
{
   int i;
   int nrow = nrh - nrl + 1;    /* compute number of lines */
   int ncol = nch - ncl + 1;    /* compute number of columns */

   double **pArray  = (double **) malloc((size_t)( nrow ) * sizeof(double*) );
   double  *pMatrix = (double *)  malloc((size_t)( nrow * ncol ) * sizeof( double ));

   if( pArray  == 0)  ERROR("Storage cannot be allocated");
   if( pMatrix == 0)  ERROR("Storage cannot be allocated");

   /* first entry of the array points to the value corrected by the
      beginning of the column */
   pArray[0] = pMatrix - ncl;

   /* compute the remaining array entries */
   for( i = 1; i < nrow; i++ )
   {
       pArray[i] = pArray[i-1] + ncol;
   }

   /* return the value corrected by the beginning of a line */
   return pArray - nrl;
}


/* deallocates the storage of a matrix  */
void free_matrix( double **m, int nrl, int nrh, int ncl, int nch )
{
   double **pArray  = m + nrl;
   double  *pMatrix = m[nrl]+ncl;

   free( pMatrix );
   free( pArray );
}

void init_matrix( double **m, int nrl, int nrh, int ncl, int nch, double a)
{
   int i,j;
   for( i = nrl; i <= nrh; i++)
       for( j = ncl; j <= nch; j++)
           m[i][j] = a;
}


/* allocates storage for a matrix */
int **imatrix( int nrl, int nrh, int ncl, int nch )
{
   int i;

   int nrow = nrh - nrl + 1;    /* compute number of rows */
   int ncol = nch - ncl + 1;    /* compute number of columns */

   int **pArray  = (int **) malloc((size_t)( nrow ) * sizeof( int* ));
   int  *pMatrix = (int *)  malloc((size_t)( nrow * ncol ) * sizeof( int ));


   if( pArray  == 0)  ERROR("Storage cannot be allocated");
   if( pMatrix == 0)  ERROR("Storage cannot be allocated");

   /* first entry of the array points to the value corrected by the
      beginning of the column */
   pArray[0] = pMatrix - ncl;

   /* compute the remaining array entries */
   for( i = 1; i < nrow; i++ )
   {
       pArray[i] = pArray[i-1] + ncol;
   }

   /* return the value corrected by the beginning of a line */
   return pArray - nrl;
}

/* deallocates the storage of a matrix  */
void free_imatrix( int **m, int nrl, int nrh, int ncl, int nch )
{
   int **pArray  = m + nrl;
   int  *pMatrix = m[nrl]+ncl;

   free( pMatrix );
   free( pArray );
}

void init_imatrix( int **m, int nrl, int nrh, int ncl, int nch, int a)
{
   int i,j;
   for( i = nrl; i <= nrh; i++)
       for( j = ncl; j <= nch; j++)
           m[i][j] = a;
}


int **read_pgm(const char *filename)
{
    FILE *input = NULL;
    char line[1024];
    int levels;
    int xsize, ysize;
    int i1, j1;
    int **pic = NULL;


    if ((input=fopen(filename,"rb"))==0)
    {
       char szBuff[80];
           sprintf( szBuff, "Can not read file %s !!!", filename );
           ERROR( szBuff );
    }

    /* check for the right "magic number" */
    if ( fread(line,1,3,input)!=3 )
    {
            fclose(input);
            ERROR("Error Wrong Magic field!");
    }

    /* skip the comments */
    do
    if(fgets(line,sizeof line,input));
    while(*line=='#');

    /* read the width and height */
    sscanf(line,"%d %d\n",&xsize,&ysize);

    printf("Image size: %d x %d\n", xsize,ysize);

    /* read # of gray levels */
    if(fgets(line,sizeof line,input));
    sscanf(line,"%d\n",&levels);


    /* allocate memory for image */
    pic = imatrix(0,xsize+2,0,ysize+2);
    printf("Image initialised...\n");

    /* read pixel row by row */
    for(j1=1; j1 < ysize+1; j1++)
    {
        for (i1=1; i1 < xsize+1; i1++)
        {
            int byte;
            if(fscanf(input, "%d", &byte));

            if (byte==EOF)
            {
                fclose(input);
                ERROR("read failed");
            }
            else
            {
                pic[i1][ysize+1-j1] = byte;
                printf("%d,%d: %d\n", i1,ysize+1-j1,byte);
            }
         }
    }
    for (i1 = 0; i1 < xsize+2; i1++)
    {
        pic[i1][0] = 0;
    }
    for (i1 = 0; i1 < xsize+2; i1++)
    {
        pic[i1][ysize+1] = 0;
    }
    for (j1 = 0; j1 < ysize+2; j1++)
    {
        pic[0][j1] = 0;
        pic[xsize+1][j1] = 0;
    }

    /* close file */
    fclose(input);

    return pic;
}


/* ----------------------------------------------------------------------- */
/*                      general index calculation                          */
/* ----------------------------------------------------------------------- */

// function to calculate lexicographical co-ordinates of the lattices in the Flag


// function to calculate lexicographical co-ordinates of the lattices in the field
