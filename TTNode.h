#ifndef _TTNODE
#define _TTNODE

#include <exception>
#include <iomanip>
#include <string>
#include <sstream>

#define MAX_D 3
#define MAX_C 4

template<class T>
class TwoThreeNode{
private:
	//const int MAX_D = 3; // Maximum number of data.
	//const int MAX_C = 4; // Maximum number of children.
	T data[MAX_D];
	TwoThreeNode<T> *children[MAX_C], *parentPtr;
	bool isLeaf;
	int numElements; // indicates the # of objects stored in this node. If the node is not a leaf, numElements + 1 is the # of children...

public:
	TwoThreeNode();
	TwoThreeNode( const T& sData );
	~TwoThreeNode();

	void setChild( TwoThreeNode<T> *nPtr, int index );
	TwoThreeNode<T>* getChild( int index );

	void setData( const T& inData, int index );
	T getData( int index );

	void setLeaf( bool flag );
	bool getLeaf();

	void setElements( int inData );
	void iterateElements( int inValue );
	int getElements() const;

	void setParent( TwoThreeNode<T> *inNode );
	TwoThreeNode<T>* getParent();

	void printData();
	void printChildren();
	void printPNC();

	// Debug
	void setAllData(const T& a, const T& b, const T& c, const T& n);
	void setAllChild(TwoThreeNode<T> *a, TwoThreeNode<T> *b, TwoThreeNode<T> *c, TwoThreeNode<T> *d, TwoThreeNode<T> *p, bool flag);

};

template<class T>
TwoThreeNode<T>::TwoThreeNode()
	: children(), parentPtr(nullptr), isLeaf(false), numElements(0)
{

}

template<class T>
TwoThreeNode<T>::TwoThreeNode( const T& sData )
	: children(), parentPtr(nullptr), isLeaf(false), numElements(1)
{
	data[0] = sData;
}

template<class T>
TwoThreeNode<T>::~TwoThreeNode(){
	parentPtr = nullptr;
	for( int i=0; i<MAX_C; i++ ){
		children[i] = nullptr;
	}
}

template<class T>
void TwoThreeNode<T>::setChild(TwoThreeNode<T> *nPtr, int index){
	if( index >= 0 && index < MAX_C ){
		children[index] = nPtr;
	} else {
		throw logic_error("Index out of range.");
	}
}

template<class T>
TwoThreeNode<T>* TwoThreeNode<T>::getChild(int index){
	if( index >= 0 && index < MAX_C ){
		return children[index];
	} else {
		throw logic_error("Index out of range.");
	}
}

template<class T>
void TwoThreeNode<T>::setData(const T& inData, int index){
	if( index >= 0 && index < MAX_D ){
		data[index] = inData;
	} else {
		throw logic_error("Index out of range.");
	}
}

template<class T>
T TwoThreeNode<T>::getData(int index){
	if( index >= 0 && index < MAX_D ){
		return data[index];
	} else {
		throw logic_error("Index out of range.");
	}
}

template<class T>
void TwoThreeNode<T>::setLeaf(bool flag){
	isLeaf = flag;
}

template<class T>
bool TwoThreeNode<T>::getLeaf(){
	return isLeaf;
}

template<class T>
void TwoThreeNode<T>::setElements( int inData ){
	numElements = inData;
}

template<class T>
void TwoThreeNode<T>::iterateElements(int inValue){
	if( inValue == 1 || inValue == -1 ){
		numElements += inValue;
	} else {
		throw logic_error("Invalid value.");
	}
}

template<class T>
int TwoThreeNode<T>::getElements() const {
	return numElements;
}

template<class T>
void TwoThreeNode<T>::setParent(TwoThreeNode<T> *inNode){
	parentPtr = inNode;
}

template<class T>
TwoThreeNode<T>* TwoThreeNode<T>::getParent(){
	return parentPtr;
}

template<class T>
void TwoThreeNode<T>::printData() {
	stringstream ss;
	cout << setw(10);
	switch( numElements ){
		case 0: ss << "(X,X,X)";
				break;

		case 1: ss << '(' << data[0] << ",X,X)";
				break;

		case 2: ss << '(' << data[0] << ',' << data[1] << ",X)";
				break;

		case 3: ss << '(' << data[0] << ',' << data[1] << ',' << data[2] << ')';
				break;

		default: throw logic_error("Node element count out of range.");
				 //cout << "Wrong number of elements.";
	}
	cout << ss.str();
}

template<class T>
void TwoThreeNode<T>::printChildren(){
	for( int i=0; i<MAX_C; i++ ){
		if( children[i] ){
			children[i]->printData();
		} else {
			cout << setw(10) << "(NULL)";
		}
	}
}

template<class T>
void TwoThreeNode<T>::printPNC(){
	if( parentPtr ){
		parentPtr->printData();
	} else {
		cout << setw(10) << "ROOT";
	}
	cout << "|";
	printData();
	cout << "|";
	printChildren();
}

// Debug

template<class T>
void TwoThreeNode<T>::setAllData(const T& a, const T& b, const T& c, const T& n){
	data[0] = a;
	data[1] = b;
	data[2] = c;
	numElements = n;
}

template<class T>
void TwoThreeNode<T>::setAllChild(TwoThreeNode<T> *a, TwoThreeNode<T> *b, TwoThreeNode<T> *c, TwoThreeNode<T> *d, TwoThreeNode<T> *p, bool flag){
	children[0] = a;
	children[1] = b;
	children[2] = c;
	children[3] = d;
	parentPtr = p;
	isLeaf = flag;
}

#endif