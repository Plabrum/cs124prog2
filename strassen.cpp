#include <iostream>
#include <time.h> 
using namespace std;

// after this point normal matrix operation will take over
int crossover = 128;
int dimension = 2048;
int current_dim = dimension;

// NOTES
// Change to be able to handle 32 bit numbers - perhaps use long int

class Matrix
{
	public:
		int dims = 0;
		// int output_matrix;

		Matrix(int dimension){
			int dims = dimension;
			int output_matrix[dims][dims];
		}
		

		void initialise(){
			for (int i=0; i < dims; i++){
				for (int j =0; j < dims; j++){
					output_matrix[i][j] = rand() %1;
				}
			}
		}

		void print_matrix(){
			for (int i = 0; i < dims; i++){
				for (int j = 0; j < dims; j++){
					cout << output_matrix[i][j] << " ";
				}
				cout << "\n";
			}
		}

};

int create_matrix (){
	int output_matrix[dimension][dimension];
	for (int i=0; i < dimension; i++){
		for (int j =0; j < dimension; j++){
			output_matrix[i][j] = rand() %1;
		}
	}
	// create matrix
	return 0;
}

int conventional (){
	// perform conventional matrix multi
	return 0;
}

int strass(int val){
	while (current_dim > crossover){
		//perform strassen
		strass(val);
	}
	conventional();
	return 0;
}


int main(){
	cout << "hello, world! \n";
	return 0;
}
// generates the matrices based on a given size, 


// run strassen until a cutoff point n after that normal matrix


// write a diagonaliser output