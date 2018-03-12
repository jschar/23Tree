#ifndef _TTTREE
#define _TTTREE

#include <exception>
#include "TTNode.h"

// 

template<class T>
class TwoThreeTree{
private:
	TwoThreeNode<T> *root;
	void insertData(TwoThreeNode<T> *ttn, const T& inData);
	void insertChild(TwoThreeNode<T> *parent, TwoThreeNode<T> *child);
	void splitNode( TwoThreeNode<T> * ttn );
	int height( TwoThreeNode<T> *ttn ) const;
	void fixTree( TwoThreeNode<T> *nPtr );
	void printGivenLevel(TwoThreeNode<T> *root, int level);

public:
	TwoThreeTree();
	TwoThreeTree( TwoThreeNode<T> *inRoot );
	TwoThreeNode<T>* searchSubTreeForLeaf( TwoThreeNode<T> *ttn, T key );
	TwoThreeNode<T>* searchSubTreeForData( TwoThreeNode<T> *ttn, T key );
	TwoThreeNode<T>* searchSubTreeForSuccessor( TwoThreeNode<T> *ttn, T key );
	bool searchNode( TwoThreeNode<T> *ttn, const T& key) const;
	void addData( const T& inData );
	bool deleteData( const T& inData );
	void printTree();
	void printPostOrder( TwoThreeNode<T> *root );
	void printLevelOrder();
	

};

template<class T>
TwoThreeTree<T>::TwoThreeTree()
	: root(nullptr)
{

}

template<class T>
TwoThreeTree<T>::TwoThreeTree( TwoThreeNode<T>* inRoot )
	: root( inRoot )
{
}

// Searches a tree for a leaf that would have had the key value.
// Should never return nullptr.
template<class T>
TwoThreeNode<T>* TwoThreeTree<T>::searchSubTreeForLeaf(TwoThreeNode<T> *ttn, T key){
	if( ttn->getLeaf() ){
		return ttn;
	} else { // Is a internal node.
		if (key <= ttn->getData(0)){
			// The search smallest child for leaf.
			return searchSubTreeForLeaf(ttn->getChild(0),key);
		} else if( ttn->getElements() < 2 || key <= ttn->getData(1) ) {
			// when the node has only 1 element, or it has 2 elements and ttn.elements[2] <= k, we know to check the second child...
			return searchSubTreeForLeaf( ttn->getChild(1), key );
		} else {
			// if we reach this clause, we've got 2 elements, and k > ttn.elements[2], so we know to check the third child...
			return searchSubTreeForLeaf( ttn->getChild(2), key );
		}
	}
}

//Recursively search a tree for data and return the containing node. Null if data doesn't exist in tree.
template<class T>
TwoThreeNode<T>* TwoThreeTree<T>::searchSubTreeForData(TwoThreeNode<T> *ttn, T key){
	bool found = false;
	found = searchNode( ttn, key );
	if( found ){ // 
		return ttn;
	} else { // Check children.
		if( ttn->getLeaf() ){ // Node is a leaf and has no children to check. Data must not exist in tree.
			return nullptr;
		} else { // internal node check children.
			if( key < ttn->getData(0) ){
				return searchSubTreeForData( ttn->getChild(0), key );
			} else if ( ttn->getElements() < 2 || key < ttn->getData(1) ) {
				return searchSubTreeForData( ttn->getChild(1), key );
			} else {
				return searchSubTreeForData( ttn->getChild(2), key );
			}
		}
	}
}

template<class T>
bool TwoThreeTree<T>::searchNode( TwoThreeNode<T> *ttn, const T& key ) const {
	bool found = false;
	int i=0, end = ttn->getElements();
	while( !found && i<end ){
		if( key == ttn->getData(i) ){
			found = true;
		}
		i++;
	}
	return found;	
}

template<class T>
void TwoThreeTree<T>::insertData(TwoThreeNode<T> *ttn, const T& inData){
	if (ttn->getElements() == 0 || inData <= ttn->getData(0)){
		// Data goes in the front or node is empty, so everything shifts over.
		ttn->setData( ttn->getData(1), 2 );
		ttn->setData( ttn->getData(0), 1 );
		ttn->setData( inData, 0 );
	}
	else if (ttn->getElements() == 1  || inData <= ttn->getData(1)){
		// Data goes after the first element of the node.
		ttn->setData( ttn->getData(1), 2 );
		ttn->setData( inData, 1 );
	} else {
		// Data goes in the last node.
		ttn->setData( inData, 2 );
	}
	ttn->iterateElements( 1 ); //elements + 1
}

template<class T>
void TwoThreeTree<T>::insertChild( TwoThreeNode<T> *parent, TwoThreeNode<T> *child ){
	if( child->getData(0) < parent->getData(0) ){
		// Child is placed in the first slot.
		// This should never happen. When nodes are split the larger child is pasted along with the median value.
		parent->setChild(parent->getChild(2), 3);
		parent->setChild(parent->getChild(1), 2);
		parent->setChild(parent->getChild(0), 1);
		parent->setChild(child, 0);
	} else if( child->getData(0) < parent->getData(1) ){
		// Child is placed in the second slot.
		parent->setChild(parent->getChild(2), 3);
		parent->setChild(parent->getChild(1), 2);
		parent->setChild(child, 1);
	} else if( child->getData(0) < parent->getData(2) ){
		// Child is placed in the third slot.
		parent->setChild(parent->getChild(2), 3);
		parent->setChild(child, 2);
	} else {
		// Child is placed in the forth slot.
		parent->setChild( child, 3 );
	}
}

template<class T>
void TwoThreeTree<T>::splitNode(TwoThreeNode<T> * ttn){
	TwoThreeNode<T> *parent = ttn->getParent();
	TwoThreeNode<T> *sibling = new TwoThreeNode<T>();
	T k = ttn->getData(1); // Copy middle data value.
	sibling->setLeaf( ttn->getLeaf() );

	sibling->setData(ttn->getData(2), 0); // Right most data goes to sibling.
	ttn->setElements(1);	// original node now only holds one element.
	sibling->setElements(1); // new sibling only holds one element.

	if( !ttn->getLeaf() ){ // If this node is not(!) a leaf then it is an internal node and might have any number of children.
		sibling->setChild(ttn->getChild(2), 0); // sibling gets two right most children.
		sibling->setChild(ttn->getChild(3), 1);
		sibling->getChild(0)->setParent(sibling); // The siblings new children could still hold old parent.
		sibling->getChild(1)->setParent(sibling);
		ttn->setChild(nullptr, 2); // Clear out moved children.
		ttn->setChild(nullptr, 3);
	}

	if( parent == nullptr ){
		// We invent a new node, and make that the root, and update it to properly reflect our data structres...
		root = new TwoThreeNode<T>();
		root->setLeaf(false);
		root->setParent( nullptr );
		root->setChild( ttn, 0 );
		root->setChild( sibling, 1 );
		root->setData( k, 0 );
		root->setElements(1);
		ttn->setParent(root);
		sibling->setParent(root);
	} else {
		// Update parent with new median value and child node.
		if (k <= parent->getData(0)){
			// Median value goes infront, shift everything over.
			parent->setData(parent->getData(1), 2);
			parent->setData(parent->getData(0), 1);
			parent->setData(k, 0);
			// Shift children over.
			parent->setChild(parent->getChild(2), 3);
			parent->setChild(parent->getChild(1), 2);
			parent->setChild(sibling, 1);
		}
		else if (k <= parent->getData(1) || parent->getElements() == 1){
			// Median value goes after front, shift over.
			parent->setData(parent->getData(1), 2);
			parent->setData(k, 1);
			// Shift child over.
			parent->setChild(parent->getChild(2), 3);
			parent->setChild(sibling, 2);
		} else {
			// Value must go on the end. Should be split later.
			parent->setData( k, 2 );
			// Child goes at the end.
			parent->setChild( sibling, 3 );
		}
		sibling->setParent( parent );
		parent->iterateElements(1); // numElements++;
		if(parent->getElements() >= 3){ // Check if the parent needs to split.
			splitNode( parent );
		}
	}
}

// TODO:
// The tree should always be balanced so just check the left child.
template<class T>
int TwoThreeTree<T>::height(TwoThreeNode<T> *ttn) const {
	if( ttn == nullptr ){
		return 0;
	} else if( ttn->getLeaf() ){
		return 1;
	} else {
		int lHeight = height(ttn->getChild(0));
		int mHeight = height(ttn->getChild(1));
		int rHeight = height(ttn->getChild(2));

		if( lHeight >= mHeight && lHeight >= rHeight ){
			return lHeight+1;
		} else if( mHeight >= rHeight ){
			return mHeight+1;
		} else {
			return rHeight+1;
		}
	}
}

template<class T>
void TwoThreeTree<T>::fixTree(TwoThreeNode<T> *nPtr){
	TwoThreeNode<T> *pPtr = nPtr->getParent(); // Parent pointer.
	TwoThreeNode<T> *sPtr = nullptr; // Sibling node to nPtr.
	//TwoThreeNode<T> *dPtr = nullptr; // Node to delete.
	if( pPtr == nullptr ){
		if(nPtr->getChild(0)){
			// Root
			//		 (X)           (# , #)
			//      /   \     ->   /  |  \
			//   (#,#) (---)      x   y   z
			//   / | \
			//  x  y  z
			root = nPtr->getChild(0); // First child is root now.
			root->setParent(nullptr); // Remove node from child.
		} else {
			// Tree has no children and is now empty.
			root = nullptr;
		}
		delete nPtr;
	} else if ( nPtr->getLeaf() ){ // Check for error in leaf.
		if( nPtr->getElements() == 0 ){ // Empty leaf node.
			if( pPtr->getElements() == 1 ){ // Parent has one element and two children.
				//	  p(#,---)
				//     /  |  \
			    //   (?) (?) (---)
				if( nPtr == pPtr->getChild(0) ){
					// Node is in first child
					//	   p(#  , ---)
					//      /   |   \    
					//   n(X) s(?)  (---)
					sPtr = pPtr->getChild(1);	// Sibling is middle child.
					if( sPtr->getElements() > 1 ){		// Works
						// Right child has data to spare.
						//		p(2)           p(3)
						//      /   \    ->   /   \
						//   n(X) s(3,4)    n(2)  s(4)
						nPtr->setData(pPtr->getData(0),0);	// Copy parent data into node.
						nPtr->setElements(1);				// Node holds data.
						pPtr->setData(sPtr->getData(0),0);	// Copy sibling's first data into parent.
						sPtr->setData(sPtr->getData(1),0);	// Shift sibling's data over.
						sPtr->setElements(1);				// sibling only holds one element;
					} else {
						// Right child has no data to spare.
						//		 p(2)             p(X)
						//      /    \    ->     /    \
						//   n(X)   s(3)     n(2,3)  s(---)
						nPtr->setData(pPtr->getData(0),0);	// Copy parent to node.
						nPtr->setData(sPtr->getData(0),1);	// Copy sibling to node.
						pPtr->setElements(0);				// Parent holds no data.
						sPtr->setElements(0);				// Sibling holds no data.
						delete sPtr;						// Remove sibling.
						pPtr->setChild(nullptr,1);			// Point to nothing.
						fixTree( pPtr );					// Fix parent.
					}
				} else {
					// Node is in second child
					//	   p(# , ---)  
					//      /  |   \    
					//   s(?) n(X) (---)
					sPtr = pPtr->getChild(0);
					if( sPtr->getElements() > 1){	// Works
						// Sibling has data to spare.
						//		p(3)           p(2)
						//      /   \    ->   /   \
						// s(1,2)   n(X)    s(1)  n(3)
						nPtr->setData(pPtr->getData(0),0);	// Copy parent data into node.
						nPtr->setElements(1);				// Node  holds data.
						pPtr->setData(sPtr->getData(1),0);	// Copy second data element in sibling to parent.
						sPtr->setElements(1);				// Sibling only holds one element.
					} else {
						// Left child has no data to spare.
						//		p(3)           p(X)
						//      /   \    ->    /   \
						//   s(2)   n(X)    s(2,3) n(---)
						sPtr->setData(pPtr->getData(0),1);	// Copy parent into sibling second element.
						sPtr->setElements(2);				// Node holds two elements.
						pPtr->setElements(0);				// Parent holds no data.
						delete nPtr;						// Delete node.
						pPtr->setChild(nullptr,1);			// Point second child to nothing.
						fixTree( pPtr );					// Fix parent.
					}
				}
			} else { // Parent has two elements and a additional child.
				//	   p(#,#)
				//     /  |  \
				//   (?) (?) (?)
				if( pPtr->getChild(0) == nPtr ){ // Empty node is in first child.
					//		p(#,#)
					//      /  |  \
					//   n(X) (?) (?)
					sPtr = pPtr->getChild(1);	// Sibling is middle child.
					if( pPtr->getChild(1)->getElements() > 1 ){		// Works
						// Second child has data to spare.
						//		p( 2, 5)			 p(3 , 5)
						//      /   |   \	->        /  |  \
						//   n(X) s(3,4) (6)	  n(2) s(4) (6)
						nPtr->setData( pPtr->getData(0), 0 );	// Move parent first element to node first element.
						nPtr->setElements(1);					// Node now has one element.
						pPtr->setData( sPtr->getData(0), 0);	// Copy first element from second child to parent first element.
						sPtr->setData( sPtr->getData(1), 0 );	// Copy second element in child to first.
						sPtr->setElements(1);					// Remove second element in child.
						// No node to delete
					} else {										// Works
						// Second child doesn't have data to space.
						//		p( 2, 4)	       p(4 , ---)
						//      /   |  \	->     /   |  \
						//   n(X)  s(3) (?)	   n(2,3) (?) (---)
						nPtr->setData( pPtr->getData(0) , 0);	// Copy first element of parent in first element of first child.
						nPtr->setData( sPtr->getData(0) , 1);	// Copy first element of sibling into second element of first child.
						delete sPtr;							// Remove sibling.
						pPtr->setChild( pPtr->getChild(2), 1);	// Move Third child into second slot.
						pPtr->setChild( nullptr, 2 );			// point third slot to null.
						nPtr->setElements(2);					// Node holds two data elements.
						pPtr->setData( pPtr->getData(1), 0 );	// Shift second element in parent.
						pPtr->setElements(1);					// Parent only holds one element.
						
					}
				} else if( pPtr->getChild(1) == nPtr ){				// Works
					// Empty node is in second child.
					//		(#,#)
					//     /  |  \
					//   (?) (X) (?)
					if( pPtr->getChild(2)->getElements() > 1 ){		// Works
						// Right sibling has data to spare.
						//		(3,5)				(3,6)
						//     /  |  \		->	    / | \
						//   (?) (X) (6,7)		 (?) (5) (7)
						sPtr = pPtr->getChild(2);						// Sibling is parent third child.
						nPtr->setData( pPtr->getData(1), 0);			// Copy second element of parent into first element of second child.
						nPtr->setElements(1);							// Node holds data again.
						pPtr->setData( sPtr->getData(0), 1); // Copy third childs first data into parent second data.
						pPtr->getChild(2)->setData( sPtr->getData(1), 0); // Shift third childs data over.
						pPtr->getChild(2)->setElements(1);				// Third child only holds one data element now.
					}
					else if (pPtr->getChild(0)->getElements() > 1) { // Works
						// Left child has data to spare.
						//		  (3,5)		      (2,5)
						//       /  |  \    ->   /  |  \
						//   (1,2) (X) (6)	   (1) (3) (6)
						sPtr = pPtr->getChild(0);			// Sibling if first child.
						nPtr->setData(pPtr->getData(0),0);	// Copy first element of parent into first element of second child.
						nPtr->setElements(1);				// Node now holds one data element.
						pPtr->setData(sPtr->getData(1),0);	// Copy first child second data element into parent first data element.
						sPtr->setElements(1);				// Child only holds one element now.
					} else {										// Works
						// Other children don't have data to give. Take from right child.
						//		p(2,4)		     p(4, ---)
						//      /  |  \    ->     /  |   \
						//   s(1) n(X) (5)	  s(1,2) n(5) (---)
						sPtr = pPtr->getChild(0);				// Sibling will be first child.
						sPtr->setData(pPtr->getData(0),1);		// Copy first element of parent to second element of first child.
						sPtr->setElements(2);					// Sibling holds two elements now.
						pPtr->setData( pPtr->getData(1), 0 );	// Copy second element in parent to first.
						pPtr->setElements(1);					// Parent now holds one element.
						delete nPtr;							// Remove second child
						pPtr->setChild(pPtr->getChild(2), 1);	// Copy third child to second child
						pPtr->setChild(nullptr,2);				// Point third to nullptr.
					}
				} else {											// Works
					// Empty node is in third child.
					//		(#,#)
					//     /  |  \
					//   (?) (?) (X)
					sPtr = pPtr->getChild(1);	// Sibling is middle child.
					if( sPtr->getElements() > 1 ){		// Works
						// Left child has data to spare.
						//		(2 , 5)         (2 , 4)
						//     /   |  \    ->   /  |  \
						//   (1) (3,4) (X)    (1) (3) (5)
						nPtr->setData( pPtr->getData(1), 0 );	// Copy parent second element into node first element.
						nPtr->setElements(1);					// Node holds one element now.
						pPtr->setData( sPtr->getData(1), 1 );	// Copy siblings extra data to parent.
						sPtr->setElements(1);					// Sibling only holds one element.						
					} else {									// Works
						// Left child doesn't have data to spare.
						//		(2 , 4)         ( 2 , ---)
						//     /   |  \    ->   /   |   \
						//   (1)  (3) (X)     (1) (3,4) (---)
						sPtr->setData(pPtr->getData(1),1);  // Move second element from parent to second element of sibling.
						sPtr->setElements(2);				// Node holds two elements.
						pPtr->setElements(1);				// Node holds one element now.
						delete pPtr->getChild(2);			// Remove third node.
						pPtr->setChild(nullptr,2);			// Point third child to null.
					}
				}
			}
		} // else 
			// Node has data value. Nothing illegal.
			//	   (2 , 4)  
			//     /  |  \  
			//   (1) [3] (5)
	} else if (nPtr->getElements() == 0){
		// Internal node. It's assumed that all children from empty node are stored in the first child(0).
		if( pPtr->getElements() ==  1 ){
			// Parent only has one data element.
			//      p(#)
			//      /  \
			//   s(?)   n(X)
			//   /  \     |
			//  a    b    c
			if( nPtr == pPtr->getChild(0) ){ 
				// Node to fix is in first child.
				//       p(#)
				//      /   \
				//   n(X)    s(?)
				//     |      / \
                //     a     b   c
				sPtr = pPtr->getChild(1); // Sibling is second child.
				if( sPtr->getElements() == 1 ) { // Works
					// Sibling has no data to spare. Everything goes into one node and that node is placed in the first child.
					//       p(10)                 p(X)
					//      /    \                /   \
					//   n(X)    s(20)  ->  n(10,20)  s(---)
					//     |      /  \       /  |  \   
					//     a     b    c     a   b   c
					nPtr->setData(pPtr->getData(0),0);		// Copy parent first element into node first element.
					pPtr->setElements(0);					// Parent holds no data.
					nPtr->setData(sPtr->getData(0),1);		// Copy sibling first element into node second element.
					nPtr->setElements(2);					// Node now holds two elements.
					nPtr->setChild(sPtr->getChild(0),1);	// Copy sibling's first child to node's second slot.
					nPtr->getChild(1)->setParent(nPtr);		// Point new child to new parent.
					nPtr->setChild(sPtr->getChild(1),2);	// Copy sibling's second child to node's third slot.
					nPtr->getChild(2)->setParent(nPtr);		// Point new child to new parent.
					pPtr->setChild(nullptr,1);				// Remove sibling from parent.
					delete sPtr;							// Delete sibling.
					fixTree( pPtr );						// Fix parent.
				} else {	// Works
					// Sibling has data to spare. Data gets rotated over.
					//      (10)                (20)
					//      /  \                /  \
					//   (X)   (20,30)   ->  (10)  (30)
					//    |    /  |  \       /  \  /  \
					//    a   b   c   d     a   b  c   d
					sPtr = pPtr->getChild(1);				// Store sibling.
					nPtr->setData(pPtr->getData(0),0);		// Copy first element of parent into first element of node.
					nPtr->setElements(1);					// Node now holds one element;
					nPtr->setChild(sPtr->getChild(0),1);	// Copy first child of sibling into second child of node.
					nPtr->getChild(1)->setParent(nPtr);     // Point new child to new parent.
					pPtr->setData(sPtr->getData(0),0);		// Copy first element of sibling into first element of parent.
					// Shift everything in sibling over.
					sPtr->setData(sPtr->getData(1),0);		// Shift data over.
					sPtr->setElements(1);					// Only holds one element.
					sPtr->setChild(sPtr->getChild(1),0);	// Move second child to first.
					sPtr->setChild(sPtr->getChild(2),1);	// Move third child to second.
					sPtr->setChild(nullptr,2);				// Set third child to null.
				}
			} else {
				// Node to fix is in second child.
				//      (#)
				//     /   \
				//   (?)   (X)
				//   / \    |
				//  a   b   c
				sPtr = pPtr->getChild(0); // Sibling is first child.
				if( sPtr->getElements() == 1 ){		// Works
					// Sibling has no data to spare.
					//       (20)              (X)
					//      /    \            /  \
					//   (10)    (X)  -> (10,20) (---)
					//   /  \     |      /  |  \
					//  a    b    c     a   b   c
					sPtr->setData(pPtr->getData(0),1);		// Copy parent data into second element sibling.
					sPtr->setElements(2);					// sibling now hold two elements.
					pPtr->setElements(0);					// Parent holds no data.
					pPtr->setChild(nullptr,1);				// Remove node from parent.
					sPtr->setChild( nPtr->getChild(0),2);	// Copy node's first child to siblings third slot.
					sPtr->getChild(2)->setParent(sPtr);		// Point new child to new parent.
					delete nPtr;
					fixTree(pPtr);							// Fix parent.
				} else {	//	Works
					// Sibling has data to spare.
					//         (30)             (20)
					//        /   \            /   \
					//   (10,20)   (X)  -> (10)    (30)
					//   /  |  \    |      /  \    /  \
					//  a   b   c   d     a    b  c    d
					nPtr->setData(pPtr->getData(0),0);		// Copy parent data into node.
					nPtr->setElements(1);					// Node now holds data.
					nPtr->setChild(nPtr->getChild(0),1);	// Move child over.
					nPtr->setChild(sPtr->getChild(2),0);	// Copy sibling child over to node.
					nPtr->getChild(0)->setParent(nPtr);		// Point new child to new parent.
					sPtr->setChild(nullptr,2);				// Remove child from sibling.
					pPtr->setData(sPtr->getData(1),0);		// Copy Extra sibling data to parent.
					sPtr->setElements(1);					// sibling holds one element.
				}
			}
		} else {
			// Parent has two data elements.
			//    p(# , #)
			//     /  |  \
			//   (?) (?) (X)
			if( nPtr == pPtr->getChild(0) ){
				// Node to remove is in first child.
				//   p(# , #)
				//    /  |  \
				// n(X) (?) (?)
				sPtr = pPtr->getChild(1); // Sibling is middle child.
				if( sPtr->getElements() == 1 ){	//	Works
					// Sibling has no data to spare.
					//     p(10 , 30)                  p(30)
					//     /    |     \               /     \
					//   n(X)  s(20)   (?)  ->  n(10,20)    (?)
					//     |   /  \    / \       /  |  \    /  \ 
					//     a  b    c  x   y     a   b   c  x    y
					nPtr->setData(pPtr->getData(0),0);	 // Copy parent first element into node first element.
					nPtr->setData(sPtr->getData(0),1);	 // Copy sibling first element into node second element.
					nPtr->setElements(2);				 // Node now holds two elements.
					nPtr->setChild(sPtr->getChild(0),1); // Copy sibling first child to nodes middle child.
					nPtr->getChild(1)->setParent(nPtr);	 // Point new child to new parent.
					nPtr->setChild(sPtr->getChild(1),2); // Copy sibling second child to nodes third child.
					nPtr->getChild(2)->setParent(nPtr);	 // Point new child to new parent.
					pPtr->setData(pPtr->getData(1),0);	 // Copy second element over first.
					pPtr->setElements(1);				 // Node holds one element.
					pPtr->setChild(pPtr->getChild(2),1); // Copy third child to middle child.
					pPtr->setChild(nullptr,2);			 // Point to null.
					delete sPtr;						 // Remove sibling.
				} else {	// Works
					// Sibling has data to spare.
					//      p(10 , 40)               p(20 , 40)
					//     /     |     \            /     |     \
					//  n(X) s(20,30)   (?)  -> n(10)   s(30)    (?)
					//    |   /  |  \   / \      /  \    /  \    / \ 
					//    a  b   c   d x   y    a    b  c    d  x   y
					nPtr->setData(pPtr->getData(0),0);		// Copy parent first element to node first element.
					nPtr->setElements(1);					// Node now holds one element.
					nPtr->setChild(sPtr->getChild(0),1);	// Copy sibling first child to node middle child.
					nPtr->getChild(1)->setParent(nPtr);		// Point new child to new parent.
					pPtr->setData(sPtr->getData(0),0);		// Copy sibling first element into parent.
					sPtr->setData(sPtr->getData(1),0);		// Copy sibling second element into first.
					sPtr->setElements(1);					// Sibling holds one element now.
					sPtr->setChild(sPtr->getChild(1),0);	// Shift sibling child over.
					sPtr->setChild(sPtr->getChild(2),1);	// Shift sibling child over.
					sPtr->setChild(nullptr,2);				// Set third child to null.
				}
			} else if (nPtr == pPtr->getChild(1)){
				// Node to remove is in second child.
				//    p(# , #)
				//    /   |  \
				//  (?) n(X) (?)
				if(pPtr->getChild(2)->getElements() > 1) { // Works
					// Third child has data to spare.
					//    p(10 , 20)                p(10 , 30)
					//    /    |   \               /     |    \
					//  (?)  n(X) s(30,40)   ->  (?)   n(20)  s(40)
					//  / \    |   /  |  \       / \    / \    / \
					// x   y   a  b   c   d     x   y  a   b  c   d
					sPtr = pPtr->getChild(2);			 // Sibling is third child.
					nPtr->setData(pPtr->getData(1),0);	 // Copy parent second element into node first element.
					nPtr->setElements(1);				 // Node holds one element.
					nPtr->setChild(sPtr->getChild(0),1); // Move sibling's first child to node's second child.
					nPtr->getChild(1)->setParent(nPtr);	 // Point new child to new parent.
					pPtr->setData(sPtr->getData(0),1);	 // Copy sibling's first element into parents second element.
					sPtr->setElements(1);				 // Sibling only hold one element.
					// Shift sibling's children over.
					sPtr->setData(sPtr->getData(1),0);	 // Copy sibling's second element into the first element.
					sPtr->setChild(sPtr->getChild(1),0); // Second child becomes first.
					sPtr->setChild(sPtr->getChild(2),1); // Third child becomes second.
					sPtr->setChild(nullptr,2);			 // Third child points to null.					
				} else if(pPtr->getChild(0)->getElements() > 1) {	// Works
					// First child has data to spare.
					//         p(30 , 40)            p(20 , 40)
					//        /     |   \           /     |     \
					//  s(10,20)  n(X)  (?)   -> s(10)  n(30)   (?)
					//   /  |  \    |   / \       / \    / \    / \
					//  a   b   c   d  x   y     a   b  c   d  x   y
					sPtr = pPtr->getChild(0);			 // Sibling is first child.
					nPtr->setData(pPtr->getData(0),0);	 // Copy parent's first element into nodes first element.
					nPtr->setElements(1);				 // Node now holds one element.
					pPtr->setData(sPtr->getData(1),0);	 // Copy sibling's second element into parent's first element
					sPtr->setElements(1);				 // Sibling holds one elements.
					nPtr->setChild(nPtr->getChild(0),1); // Move node first child over.
					nPtr->setChild(sPtr->getChild(2),0); // Move sibling's third child to nodes first child.
					nPtr->getChild(0)->setParent(nPtr);	 // Point new child to new parent.
					sPtr->setChild(nullptr,2);			 // point sibling's third child to null.
				} else {	// Works
					// No sibling has data to spare.
					//      p(20 , 30)               p(20)
					//     /     |   \           /     |    \
					//   (10)  n(X) s(40)  -> (10) n(30,40) s(---)
					//   / \     |   / \      / \   /  |  \    
					//  x   y    a  b   c    x   y a   b   c
					sPtr = pPtr->getChild(2);			 // Sibling is third child.
					nPtr->setData(pPtr->getData(1),0);	 // Copy parent's second element into node's first element.
					nPtr->setData(sPtr->getData(0),1);	 // Copy sibling's first element into node's second element.
					nPtr->setElements(2);				 // Node holds two data elements.
					pPtr->setElements(1);				 // Parent only holds one element.
					sPtr->setElements(0);				 // Sibling holds no elements.
					nPtr->setChild(sPtr->getChild(0),1); // Move sibling's first child to node's second child.
					nPtr->getChild(1)->setParent(nPtr);	 // Point new child to new parent.
					nPtr->setChild(sPtr->getChild(1),2); // Move sibling's second child to node's third child.
					nPtr->getChild(2)->setParent(nPtr);	 // Point new child to new parent.
					pPtr->setChild(nullptr,2);			 // Remove sibling from parent.
					delete sPtr;						 // Remove sibling.
				}
			} else {
				// Node to remove is in third child.
				//    (# , #)
				//    /  |  \
				//  (?) (?) (X)
				sPtr = pPtr->getChild(1); // Sibling is second child.
				if( sPtr->getElements() > 1 ){ // Works
					// Sibling has data to spare.
					//      p(10 , 40)            p(10 , 30)
					//    /      |     \         /     |    \
					//  (?)  s(20,30) n(X)  -> (?)   s(20) n(40)
					//  / \   /  |  \   |      / \    / \   / \
					// x   y a   b   c  d     x   y  a   b c   d
					nPtr->setData(pPtr->getData(1),0);	 // Copy parent's second element into node's first element.
					pPtr->setData(sPtr->getData(1),1);	 // Copy sibling's second element into parents second element.
					nPtr->setElements(1);				 // Node holds one element.
					sPtr->setElements(1);				 // Sibling holds one element.
					nPtr->setChild(nPtr->getChild(0),1); // Shift first child over.
					nPtr->setChild(sPtr->getChild(2),0); // Move sibling's third child over to node's first child.
					nPtr->getChild(0)->setParent(nPtr);	 // Point new child to new parent.
					sPtr->setChild(nullptr,2);			 // Sibling's third child points to null.			
				} else {	// Works
					// Sibling has no data to spare.
					//    p(10 , 30)            p(10)
					//    /    |   \          /     |    \
					//  (?)  s(20) n(X)  -> (?) s(20,30) n(---)
					//  / \   / \    |      / \   / | \   
					// x   y a   b   c     x   y a  b  c  
					sPtr->setData(pPtr->getData(1),1);	 // Copy parent's second element into sibling's second element.
					sPtr->setElements(2);				 // Sibling holds two elements.
					pPtr->setElements(1);				 // Parent holds one element.
					sPtr->setChild(nPtr->getChild(0),2); // Move first child of node to third child of sibling.
					sPtr->getChild(2)->setParent(sPtr);	 // Point new child to new parent.
					pPtr->setChild(nullptr,2);			 // Remove node from parent.
					delete nPtr;						 // Delete node.
				}
			}
		}
	}
}

template<class T>
void TwoThreeTree<T>::printTree() {
	if( root == nullptr ){
		cout << "Tree is empty." << endl;
	} else {
		printPostOrder( root );
	}
}

template<class T>
void TwoThreeTree<T>::printPostOrder(TwoThreeNode<T> *root){
	if( root == nullptr ){
		cout << "NULL" << endl;
	} else {
		printPostOrder(root->getChild(0));
		printPostOrder(root->getChild(1));
		printPostOrder(root->getChild(2));
		printPostOrder(root->getChild(3));
		cout << "--";
		root->printData();
		cout << "--" << root->getElements() << endl;
	}
}

template<class T>
void TwoThreeTree<T>::printLevelOrder(){
	int h = height( root );
	if( h > 0 ){
		//cout << "Height: " << h << endl;
		int i;
		for( i=1; i<=h; i++ ){
			cout << "--Level " << i << "--" << endl;
			printGivenLevel( root, i );
			cout << endl;
		}
	} else {
		cout << "Tree is empty." << endl;
	}
}

template<class T>
void TwoThreeTree<T>::printGivenLevel( TwoThreeNode<T> *root, int level ){
	if( root == nullptr ){
		return;
	}
	if( level == 1 ){
		try{
			root->printPNC();
		} catch( logic_error &e ){
			cout << "Logic error: " << e.what() << endl;
		}
		cout << endl;
	} else if( level > 1 ){
		printGivenLevel( root->getChild(0), level-1 );
		printGivenLevel( root->getChild(1), level-1 );
		printGivenLevel( root->getChild(2), level-1 );
	}
}

template<class T>
void TwoThreeTree<T>::addData(const T& inData){
	if( root == nullptr ){	// Empty tree.
		root = new TwoThreeNode<T>( inData );
		root->setLeaf(true);
	} else {
		TwoThreeNode<T> *nLeaf = nullptr;
		// Travers tree until leaf node that will hold value.
		nLeaf = searchSubTreeForLeaf(root, inData);
		// Add value to node.
		insertData(nLeaf, inData);
		// If node is full
		if( nLeaf->getElements() >= 3 ){
			splitNode(nLeaf);
		}
	}
}

template<class T>
bool TwoThreeTree<T>::deleteData( const T& inData ){
	bool success = false;
	TwoThreeNode<T> *nPtr = searchSubTreeForData( root, inData ); // Pointer to node containing data.
	if( nPtr ){
		if( nPtr->getLeaf() ){	// Data is in a leaf node.
			if( nPtr->getElements() == 1 ){ // Data only has element to remove.
				nPtr->setElements(0);	// Node is now empty.
				fixTree(nPtr);
			} else { // Node has two elements. Shift over, no need to fix.
				// (?,?)
				if( inData == nPtr->getData(0) ){	// Data to delete is in first index.
					// (D,?) -> (?,?)
					nPtr->setData(nPtr->getData(1), 0); // Shift second data over.
					// (?,?)
				} // else data is in second index and can just be incremented out.
				// (?,?) -> (?)
				nPtr->setElements(1);
			}
		} else { 
			// Data is in an internal node.
			// The inorder successor will be the first element in a leaf node.
			TwoThreeNode<T> *lPtr; // Pointer to leaf to switch data.
			// If we search the child to the right of the data we'll find our successor.
			if( nPtr->getData(0) == inData ){
				//	  (D)		  (D, #)
				//   /   \\  or   / ||  \
				// (#)   [S]    (#) [S] (#)
				// Search second child.
				lPtr = searchSubTreeForLeaf( nPtr->getChild(1), inData ); // Search second child.
				nPtr->setData(lPtr->getData(0), 0); // Copy over the data(swap)
			} else {
				//	  (#,D)
				//   /  |  \\
				// (#) (#) [S]
				// Search third child.
				lPtr = searchSubTreeForLeaf( nPtr->getChild(2), inData ); // Search third child
				nPtr->setData(lPtr->getData(0), 1); // Copy over the data(swap)
			}

			// Leaf node has another data element.
			// Shift over. No need to fix tree.
			if( lPtr->getElements() > 1 ){
				//	  (#,S)
				//   /  |  \\
				// (#) (#) [X,<-#]
				lPtr->setData(lPtr->getData(1), 0);
				lPtr->setElements(1);
			} else {
				//	  (#, S)
				//   /  |  \\
				// (#) (#) [X]
				lPtr->setElements(0);
				fixTree( lPtr );
			}
		}
		success = true;
	} else {
		success = false;
	}
	return success;
}


#endif