#ifndef string
#include <string>
#endif

#define INFINT_BITS 32

/**
* Infinity integer class
* Define bitlength of "int" above, recommended 32. Must be larger than 4.
* Leif Schelin
* 2013-06-01
*/
class Infint{
public:

	Infint(std::string indata);
	Infint(const Infint&);
	~Infint();

	Infint& operator=(const Infint&);
	Infint operator-() const;

	bool operator==(const Infint&) const;
	bool operator>(const Infint&) const;
	
	bool operator!=(const Infint&) const;
	bool operator>=(const Infint&) const;
	bool operator<(const Infint&) const;
	bool operator<=(const Infint&) const;

	Infint operator+(const Infint&) const;
	Infint operator-(const Infint&) const;

	Infint& operator+=(const Infint&);
	Infint& operator-=(const Infint&);

	Infint& operator++();
	Infint& operator--();

	void printNodes() const;
	std::string toString() const;
	
private:
	static int interpret(char c);
	static char interpretToChar(int x);

	class Node{
	public:
		Node(int d);
		Node(const Node&);
		~Node();

		bool isZero();
		void shift4(int repl);
		int back4();

		Node* _next;
		int _data;

		static const int INT_BITS = INFINT_BITS;
		static const int INT_ANTI = -1<<(INT_BITS-1);
	};

	Node* _first;
	bool _positive;
};