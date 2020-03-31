#include "stdio.h"
#include "stdlib.h"
#include "string.h"

// Function Proto's...
int MyEnumProcess( char *pszProcessName );

// Main Process
int main( void )
{
	if ( MyEnumProcess( "companion") )
	{
		printf( "Process dclock is running");
	}
	else
	{
		printf( "Process dclock is not running");
	}
}

// Function
int MyEnumProcess( char *pszProcessName )
{

	char psBuffer[1024];
	char szCommandline[100] = "";
	int iRc = 0;
	FILE *chkdsk;

	// formate your command line expression
	sprintf( szCommandline, "ps | grep %s", pszProcessName );


	//Run the command line so that it writes its output to a pipe. Open this
	//pipe with read text attribute so that we can read it
	//like a text file.
	if( (chkdsk = popen( szCommandline, "rt" )) == NULL )
		exit( -1 );

	// Read pipe their should only be one line output from the command line either
	if( !feof( chkdsk ) )
	{
		// the process is present or it is not...
		// The buffer I'm using has room for like 10 lines
		// if the output is larger we don't really care..
		// all we're interested in is T/F right...
		if( fgets( psBuffer, 1024, chkdsk ) != NULL )
		{
			// Does the process name exist in the output??
			if ( strstr( psBuffer, pszProcessName ) != NULL )
			{ // found your process Eurika
				iRc = 1;
			}
		}
	}

	// Close pipe
	pclose( chkdsk );

	return ( iRc );
}
