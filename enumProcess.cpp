#include <iostream>
#include <string.h>
#include <stdio.h>

// Function Proto's...
int MyEnumProcess(std::string pszProcessName);

// Main Process
int main(int argc, char **argv)
{
	if (argc != 2) {
		std::cout << "Usage ./enum process_name" << std::endl;
		exit(-1);
	}
	if (MyEnumProcess(argv[1]))
	{
		std::cout << "Process " << argv[1] << " is running" << std::endl;
	}
	else
	{
		std::cout << "Process " << argv[1] << "is not running" << std::endl;
	}
}

// Function
int MyEnumProcess(std::string pszProcessName)
{

	char psBuffer[1024];
	char szCommandline[100] = "";
	int iRc = 0;
	FILE *chkdsk;

	// formate your command line expressionA
	std::string sTemp = pszProcessName.substr(1);
	char first = pszProcessName.at(0);
	sprintf(szCommandline, "ps -aux| grep \"[%c]%s\"", first, sTemp.c_str());


	//Run the command line so that it writes its output to a pipe. Open this
	//pipe with read text attribute so that we can read it
	//like a text file.
	if((chkdsk = popen(szCommandline, "r")) == NULL) {
		std::cout << szCommandline << "didn't work" << std::endl;
		exit(-1);
	}

	// Read pipe their should only be one line output from the command line either
	if(!feof(chkdsk))
	{
		// the process is present or it is not...
		// The buffer I'm using has room for like 10 lines
		// if the output is larger we don't really care..
		// all we're interested in is T/F right...
		if(fgets(psBuffer, 1024, chkdsk) != NULL)
		{
			// Does the process name exist in the output??
			if (strstr(psBuffer, pszProcessName.c_str()) != NULL)
			{ // found your process Eurika
				iRc = 1;
			}
		}
	}

	// Close pipe
	pclose(chkdsk);

	return (iRc);
}
