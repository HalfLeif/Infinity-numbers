// Numbers.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;



int _tmain(int argc, _TCHAR* argv[])
{
	if(InfintUnitTest::testInfint()){
		cout<<"Infint seems to work"<<endl;
	} else {
		cout<<"Something with Infint doesn't work..."<<endl;
	}
	
	return 0;
}

