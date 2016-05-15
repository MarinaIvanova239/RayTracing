#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "Handler.h"

using namespace std;

int main(int argc, char** argv)
{
	HandlerClass handler;

	handler.parseArgumentsString(argc, argv);
	if (handler.wrongArguments)
	{
		cout << "Wrong argument's string" << endl;
		return 1;
	}

	handler.parseSceneFile();
	if (handler.wrongSceneFile)
	{
		cout << "Wrong scene file" << endl;
		return 1;
	}

	handler.working();

	return 0;
}