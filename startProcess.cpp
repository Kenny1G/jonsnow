/* runs a process passing it parameters*/

#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>


int main(int argc, char** argv)
{
	std::string command = "./companion -i companion.cfg &";
	int a = system(command.c_str());	
}