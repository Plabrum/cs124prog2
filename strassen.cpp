#include <iostream>
using namespace std;

// after this point normal matrix operation will take over
int crossover = 128;
int dimension = 2048;
int current_dim = dimension;

int create_matrix (){
	// create matrix
	return 0;
}

int conventional (int val){
	// perform conventional matrix multi
	if (val > 1){
		return 1;
	}
	return 0;
}

int strass(int val){
	while (current_dim > crossover){
		//perform strassen
		strass(val);
	}
	conventional(1);
	return 0;
}



int main(){
	cout << "hello, world!";
	return 0;
}
// generates the matrices based on a given size, 


// run strassen until a cutoff point n after that normal matrix


// write a diagonaliser output