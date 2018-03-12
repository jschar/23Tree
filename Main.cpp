#include <iostream>
#include "TTTree.h"

using namespace std;

int main( void ){

	TwoThreeTree<int> tree_05;

	for(int i=1; i<=15; i++){
		cout << "Add " << i << endl;
		tree_05.addData(i);
		tree_05.printLevelOrder();
		cout << "Press enter to continue." << endl;
		cin.ignore();
	}

	for( int j=15; j>=1; j-- ){
		cout << "Delete " << j << endl;
		tree_05.deleteData(j);
		tree_05.printLevelOrder();
		cout << "Press enter to continue." << endl;
		cin.ignore();
	}

	cout << "End of line." << endl;
	cin.ignore();
	return 0;
}