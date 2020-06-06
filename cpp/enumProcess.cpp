/* 
 *
 */

#include <iostream>
#include <cstring>
#include <sstream>
#include <stdio.h>
#include <vector>

// Function Proto's...
std::vector<std::string> MyEnumProcess(std::string pszProcessName, std::string programName);

// Main Process
int main(int argc, char **argv)
{
	if (argc != 2) {
		std::cout << "Usage ./enum process_name" << std::endl;
		exit(-1);
	}
	std::vector<std::string> result = MyEnumProcess(argv[1], argv[0]);
	for (std::vector<std::string>::iterator it = result.begin(); 
		it != result.end(); ++it){
		std::cout << *it << std::endl;
	}
	if (result.size() != 0)
	{
		std::cout << "Process " << argv[1] << " is running" << std::endl;
	}
	else
	{
		std::cout << "Process " << argv[1] << " is not running" << std::endl;
	}
	
}

// Function
std::vector<std::string> MyEnumProcess(std::string pszProcessName, std::string programName)
{

	char psBuffer[1024];
	char szCommandline[100] = "";
	FILE *chkdsk;
	std::vector<std::string> fields;

	// formate your command line expressionA
	std::string sTemp = pszProcessName.substr(1);
	char first = pszProcessName.at(0);
	sprintf(szCommandline, "ps -aux| grep -v %s | grep [%c]%s", programName.c_str(), first, sTemp.c_str());


	if((chkdsk = popen(szCommandline, "r")) == NULL) {
		std::cout << szCommandline << "didn't work" << std::endl;
		exit(-1);
	}

	if(!feof(chkdsk))
	{
		if(fgets(psBuffer, 1024, chkdsk) != NULL)
		{
			// Does the process name exist in the output??
			if (strstr(psBuffer, pszProcessName.c_str()) != NULL)
			{
				std::stringstream tmp(psBuffer);
				std::string data;
				while(tmp >> data)
				{
					fields.push_back(data);
				}
			}
			else {
				return fields;
			}
		}
	}

	// Close pipe
	pclose(chkdsk);

	return fields;
}
