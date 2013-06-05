#include "stdafx.h"

using namespace std;

/**
* Leif Schelin
* 2013-06-01
*/
Infint::Infint(string indata):_positive(true){
	_first = new Node(0x0);

	bool start = false;
	char nextC = '0';
	for(unsigned ix=0; ix<indata.size(); ix++){
		char c = indata[ix];
		
		if(!start){
			if(c==nextC){
				if(nextC=='0'){
					nextC = 'x';
				} else if(nextC=='x'){
					start = true;
				}
			} else if(c=='-'){
				_positive = false;
			} else if(c=='+'){
			} else {
				cout<<"! in Infint(): Found unexpected char "<<c<<" in string "<<indata<<endl;
			}
		} else {
			int hex = interpret(c);
			if(hex<0){
				cout<<"! in Infint(): Found unexpected char "<<c<<" in string "<<indata<<endl;
			} else {
				_first->shift4(hex);
			}
		}
		
	}

	printNodes();
}

/**
* Leif Schelin
* 2013-06-01
*/
Infint::Infint(const Infint& b){
	_positive=b._positive;
	_first = new Node(*b._first);
}

/**
* Leif Schelin
* 2013-06-01
*/
Infint::~Infint(){
	//cout<<"Infint: destructor"<<endl;
	//printNodes();
	delete _first;
	_first = NULL;
}

/**
* Leif Schelin
* 2013-06-01
*/
Infint::Node::Node(int d):_data(d){
	_next = NULL;
}

/**
* Leif Schelin
* 2013-06-01
*/
Infint::Node::Node(const Node& b){
	_data = b._data;
	if(b._next==NULL){
		_next=NULL;
	} else {
		_next=new Node(*b._next);
	}
}

/**
* Leif Schelin
* 2013-06-01
*/
Infint::Node::~Node(){
	//cout<<"Node: destructor"<<endl;
	if(_next!=NULL){
		//cout<<"Node: I'm deleted"<<endl;
		delete _next;
		_next = NULL;
	}
}

/**
* Leif Schelin
* 2013-06-01
*/
Infint& Infint::operator=(const Infint& b){
	delete _first;
	_positive = b._positive;
	_first = new Node(*b._first);
	return *this;
}

Infint Infint::operator-() const{
	Infint a = *this;
	a._positive = !_positive;
	return a;
}
/////////////////////////////////////////////

/**
* Leif Schelin
* 2013-06-01
*/
bool Infint::Node::isZero(){
	if(_data !=0){
		return false;
	}
	return _next==NULL || _next->isZero();
}

/**
* Leif Schelin
* 2013-06-01
*/
void Infint::Node::shift4(int repl){
	int top = _data>>(INT_BITS -4 -1);
	if(top>0xF || top <0){
		cout<<"in shift4: top irregular: "+top<<endl;
	}

	if(_next!=NULL || top>0){
		if(_next==NULL){
			_next = new Node(0x0);
		}
		_next->shift4(top);
	}
	_data = (((_data<<4)+repl ));
	// rensa kontrollbiten
	_data = _data&~INT_ANTI;
}

/**
* Leif Schelin
* 2013-06-01
*/
int Infint::Node::back4(){
	int send = _data & 0xF;
	//cout<<"Back4() called"<<endl;
	int repl = 0;
	if(_next !=NULL){
		repl = (_next->back4())<<(INT_BITS -4 -1);

		if(_next->isZero()){
			//cout<<"Next is zero"<<endl;
			delete _next;
			_next = NULL;
		}
	}
	_data = (_data>>4)+repl;

	return send;
}

/**
* Leif Schelin
* 2013-06-01
*/
string Infint::toString() const{
	string ut = "";

	Infint copy = *this;

	while(! (copy._first->_data==0 && copy._first->_next==NULL) ){
		int hex = copy._first->back4();
		char chex = interpretToChar(hex);

		ut=chex+ut;
		//cout<<"Inside loop, ut: "<<ut<<endl;
	}

	ut = "0x"+ut;
	if(!_positive){
		ut = "-"+ut;
	}

	return ut;
}

/////////////////////////////////////////////
/**
* Leif Schelin
* 2013-06-01
* not finished...
*/
Infint Infint::operator+(const Infint& b) const{

	bool sameSign = (this->_positive && b._positive) || (!_positive && !b._positive);
	const int mult = sameSign? 1 : -1;

	Infint sum("0x0");

	Node* curra(this->_first);
	Node* currb(b._first);
	Node* currsum(sum._first);

	int overfl = 0;
	while(true){
		int nsum = overfl + curra->_data + mult*(currb->_data);

		if(nsum & Node::INT_ANTI > 0){
			overfl = 1;
			nsum = nsum & ~Node::INT_ANTI;
		} else {
			overfl = 0;
		}

		currsum->_data=nsum;

		if(curra->_next!=NULL && !curra->_next->isZero()){
			//...
		}
	}
	//TODO
	//...

	return sum;
}

inline Infint Infint::operator-(const Infint& b) const{
	return this->operator+(-b);
}

/////////////////////////////////////////////

/**
* Leif Schelin
* 2013-06-05
*/
bool Infint::operator==(const Infint& b) const{
	Node* curra(this->_first);
	Node* currb(b._first);

	if(this->_positive!=b._positive){
		return false;
	}

	while(( !curra==NULL ) && ( !currb==NULL )){
		if(curra->_data != currb->_data){
			return false;
		}
		curra=curra->_next;
		currb=currb->_next;
	}

	return true;
}

/**
* Leif Schelin
* 2013-06-05
*/
bool Infint::operator>(const Infint& b) const{
	if(this->_positive && !b._positive){
		return true;
	}

	Node* curra = this->_first;
	Node* currb = b._first;
	bool aBigger = true;

	while(curra!=NULL && currb!=NULL){

		aBigger = curra->_data > currb->_data;
		
		if(curra->_next==NULL||curra->_next->isZero()){
			if(currb->_next==NULL||currb->_next->isZero()){
				return aBigger;
			} else {
				return false;
			}
		} else if(currb->_next==NULL||currb->_next->isZero()){
			return true;
		}

		curra = curra->_next;
		currb = currb->_next;

	}
	cout<<"Warning! in Infint::operator>(), End reached!"<<endl;
	return aBigger;
}
	
bool Infint::operator!=(const Infint& b) const{
	return !this->operator==(b);
}
bool Infint::operator>=(const Infint& b) const{
	return this->operator>(b) || this->operator==(b);
}
bool Infint::operator<(const Infint& b) const{
	return !this->operator>=(b);
}
bool Infint::operator<=(const Infint& b) const{
	return !this->operator>(b);
}
/////////////////////////////////////////////

inline Infint& Infint::operator+=(const Infint& b){
	*this = this->operator+(b);
	return *this;
}

inline Infint& Infint::operator-=(const Infint& b){
	*this = this->operator-(b);
	return *this;
}

inline Infint& Infint::operator++(){
	*this = this->operator+( Infint("0x1"));
	return *this;
}

inline Infint& Infint::operator--(){
	*this = this->operator-( Infint("0x1"));
	return *this;
}
/////////////////////////////////////////////

/**
* Leif Schelin
* 2013-06-01
*/
void Infint::printNodes() const{
	cout<<"Nodes: "<<endl;
	Node* now = _first;
	do {
		cout<<now->_data<<endl;
		now = now->_next;
	} while(now!=NULL);
	cout<<endl;
}

/**
* Leif Schelin
* 2013-06-01
*/
int Infint::interpret(char c){
	switch(c){
	case 'A':
		return 0xA;
	case 'B':
		return 0xB;
	case 'C':
		return 0xC;
	case 'D':
		return 0xD;
	case 'E':
		return 0xE;
	case 'F':
		return 0xF;
	case '0':
		return 0x0;
	case '1':
		return 0x1;
	case '2':
		return 0x2;
	case '3':
		return 0x3;
	case '4':
		return 0x4;
	case '5':
		return 0x5;
	case '6':
		return 0x6;
	case '7':
		return 0x7;
	case '8':
		return 0x8;
	case '9':
		return 0x9;
	default:
		return -1;
	}
}

/**
* Leif Schelin
* 2013-06-01
*/
char Infint::interpretToChar(int x){
	switch(x){
	case 0xA:
		return 'A';
	case 0xB:
		return 'B';
	case 0xC:
		return 'C';
	case 0xD:
		return 'D';
	case 0xE:
		return 'E';
	case 0xF:
		return 'F';
	case 0x0:
		return '0';
	case 0x1:
		return '1';
	case 0x2:
		return '2';
	case 0x3:
		return '3';
	case 0x4:
		return '4';
	case 0x5:
		return '5';
	case 0x6:
		return '6';
	case 0x7:
		return '7';
	case 0x8:
		return '8';
	case 0x9:
		return '9';
	default:
		return -1;
	}
}