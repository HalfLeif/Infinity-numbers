
#include "stdafx.h"
using namespace std;

string InfintUnitTest::numA="0xFF000A00BC00BBD0234A";

bool InfintUnitTest::testInfint(){
	if(!testInfintString())
		return false;
	else if(!testInfintBooleans())
		return false;

	return true;
}

bool InfintUnitTest::testInfintBooleans(){
	Infint a(numA);
	Infint b = a;
	Infint c(numA);

	if(!(a==b && b==c)){
		cout<<"operator== doesn't work!"<<endl;
		return false;
	} else if( a!=b || b!=c ){
		cout<<"operator!= doesn't work!"<<endl;
		return false;
	} else if(a>b){
		cout<<"operator> doesn't work!"<<endl;
		return false;
	}

	Infint big(numA+"11111111");
	Infint small("0x12");
	if(!(big>a)){
		cout<<"operator> doesn't work: @big"<<endl;
		return false;
	} else if(!(a>small)){
		cout<<"operator> doesn't work: @small"<<endl;
		return false;
	} else if(!(small>-a)){
		cout<<"operator> doesn't work: @-a"<<endl;
		return false;
	}

	return true;
}

bool InfintUnitTest::testInfintString(){

	Infint a(numA);
	string gen = a.toString();
	if(gen == numA){
		//cout<<"Strings equal :)"<<endl;
	} else {
		cout<<"Strings differ!"<<endl;
		cout<<numA<<endl;
		cout<<gen<<endl;
		return false;
	}

	return true;
}