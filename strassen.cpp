#include <iostream>
#include <string>
#include <fstream>
# include <cmath>
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
		void init_randadjacency(int probability){
			for (int i=0; i < array_len; i++){
				if ((i % dims) > (i - i% dims)/dims){
					//if the entry is in the upper triangle 
					int raw = rand() % 100;
					if (raw <= probability){
						this->data[i] = 1;
					}
					//Don't add anything to the matrix in the lower triangle or diagonal
				}
			}   
			//Copy from upper to lower	
			for (int i=0; i < array_len; i++){
				int column = i % dims;
				int row = ((i - (i%dims))/dims); 
				// if this isn't divisible add or subtract 1, it should be
				if (column < row){
					//if the entry is in the lower triangle or diagonal
					int mirror = column*dims + row;
					//Compute mirror image
					this->data[i] = data[mirror];
					//This might not access data[mirror] correctly
				}
			}	
		}

		void print_matrix(){
			for (int i = 0; i < array_len; i++){
				cout << this->data[i] << " ";
			}
			cout << "\n";
		}

		void read(int* array_in){
			this->data = array_in;
		}

};
int* read_in(string filename){
	fstream in_file(filename);
	if (in_file.is_open())
	{	string line;
		int rows = 0;
		//Count how many rows in input file
		while (getline(in_file,line)){	
			rows++;
		}

		int *data_a = new int[rows/2];
		int *data_b= new int[rows/2];

		for (int i =0; i < rows/2; i++){
			getline (in_file, line);
			data_a[i] = stoi(line);
		}
		for (int i = rows/2; i < rows; i++){
			getline (in_file, line);
			data_b[i] = stoi(line);
		}
		in_file.close();

		// hacky stuff here to get an array of pointers
		int* dim_ptr = nullptr;
		*dim_ptr = (int) sqrt(rows/2);
		int* arr_data[3]{dim_ptr, data_a, data_b};
		int* arr_ptr = &arr_data[0];
		return arr_ptr;
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

int triangleCount(int probability){
    //initialize a random 1024x1024 adjacency matrix
    Matrix mat(1024); //we hardcode this 
    mat.init_randadjacency(probability);
    2mat = strass(mat, mat);
    3mat = strass(2mat, mat);
    //count number of triangles
    int diagonaledges = 0;
    for (int i = 0; i < array_len; i = i + array_len + 1){
        //We only hit the diagonal entries by adding dim + 1 to i each time
        if (data[i] == 1){
            diagonaledges = diagonaledges + 1;
        }
    }
    int triangles = diagonaledges / 6;
    print(triangles)
    return (triangles);
}

int main(){
	Matrix mat_a(dimension);
	Matrix mat_b(dimension);
	mat_a.init_rand();
	mat_b.init_rand();
	mat_a.print_matrix();
	mat_b.print_matrix();
	conventional(mat_a, mat_b).print_matrix();

	int* data_ptr = read_in("ascii_file.txt");
	Matrix mat_c(*data_ptr[0]);
	mat_c.read(*data_ptr[1])
	Matrix mat_d(dimension);
	return 0;
}
// generates the matrices based on a given size, 


// run strassen until a cutoff point n after that normal matrix


// write a diagonaliser output