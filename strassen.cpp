#include <math.h>
#include <time.h> 
#include <string.h>
#include <fstream>
using namespace std;

// after this point normal matrix operation will take over
int crossover = 128;
// int dimension = 2048;
int dimension = 16;
int current_dim = dimension;

// NOTES
// Change to be able to handle 32 bit numbers - perhaps use long int

class Matrix
{
	public:
		int dims;
		int array_len = 0;
		int *data;

		Matrix(int size){
			dims = size;
			array_len = pow(dims,2);
			data = new int[array_len];
		}
		

		void init_rand(){
			for (int i=0; i < array_len; i++){
				this->data[i] = rand() %2;
			}
		}

		void print_matrix(){
			for (int i = 0; i < array_len; i++){
				cout << this->data[i] << " ";
			}
			cout << "\n";
		}

		// void read_in(file){

		// }

};
int* read_in(string filename){
	fstream in_file(filename);
	if (in_file.is_open())
	{	string line;
		int lines = 0;
		while (getline(in_file,line)){	
			lines++;
		}

		int *all_array_data = new int[lines];

		for (int i =0; i < lines; i++){
			getline (in_file, line)
			all_array_data[i] = stoi(line);
		}
		in_file.close();
		return all_array_data;
	}
	else {
		cout << "Unable to open file";
		return NULL;
	}

	
}

Matrix conventional(Matrix a, Matrix b){
	// perform conventional matrix multi
	assert(a.dims == b.dims);
	Matrix output_matrix(a.dims);

	for (int i = 0; i < output_matrix.array_len; i++){
		output_matrix.data[i] = a.data[i] + b.data[i];
	}
	return output_matrix;
}

int strass(int val){
	while (current_dim > crossover){
		//perform strassen
		strass(val);
	}
	return 0;
}


int main(){
	Matrix mat_a(dimension);
	Matrix mat_b(dimension);
	mat_a.init_rand();
	mat_b.init_rand();
	mat_a.print_matrix();
	mat_b.print_matrix();
	conventional(mat_a, mat_b).print_matrix();

	read_in("ascii_file.txt");
	// Matrix mat_a(dimension);
	// Matrix mat_b(dimension);
	return 0;
}
// generates the matrices based on a given size, 


// run strassen until a cutoff point n after that normal matrix


// write a diagonaliser output