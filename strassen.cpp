#include <iostream>
#include <string>
#include <fstream>
# include <cmath>
using namespace std;

// after this point normal matrix operation will take over
int crossover = 128;
// int dimension = 2048;
int dimension = 4;
int current_dim = dimension;

// NOTES
// Change to be able to handle 32 bit numbers - perhaps use long int

class Matrix
{
	public:
		int dims;
		int array_len = 0;
		int *data;

		Matrix(int array_size){
			array_len = array_size;
			dims = (int) sqrt(array_size);
			data = new int[array_len];
		}
		

		void init_rand(){
			for (int i=0; i < array_len; i++){
				this->data[i] = rand() %2;
			}
		}

		void init_randadjacency(int probability){
			for (int i=0; i < array_len; i++){
				int column = i % dims;
				int row = ((i - (i%dims))/dims); 
				printf("( %d,  %d ):", column, row);
				if ((i % dims) > (i - i% dims)/dims){
					//if the entry is in the upper triangle 
					int raw = rand() % 100;
					printf("random number: %d", raw);
					if (raw <= probability){
						this->data[i] = 1;
					}
					else {
						this->data[i] = 0;
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

		void print_diags(){
			for (int i = 0; i < dims; i++){
				cout << this->data[i*dims + i] << "\n";
			}
		}

		void read(int* array_in, string option){
			if (option == "first"){
				for (int i=0; i < array_len; i++){
					this->data[i] = array_in[i+1];
				}
				
			}
			else if (option == "second"){
				for (int i = 0; i < array_len; i++){
					this->data[i] = array_in[i+array_len+1];
				}
			}
			else if (option == "all"){
				for (int i=0; i < array_len; i++){
					this->data[i] = array_in[i+1];
				}
			}
			else{
				cout << "incorrect read parameters given \n";
			}
		}

};
int* read_in(string filename){
	fstream in_file(filename);
	if (in_file.is_open())
	{	string line;

		//Count how many rows in input file
		int rows = 0;
		while (getline(in_file,line)){
			rows++;
		}
		// set the first value of the array to be the array length
		// assumes that all arrays are square
		int *data_all = new int[rows+1];
		data_all[0] = (int) (rows/2);

		// move seeker to start
		in_file.clear();
		in_file.seekg(0);

		for (int i =1; i < rows+1; i++){
			getline (in_file, line);
			data_all[i] = stoi(line);
		}


		// Close file because we have already stored the array
		in_file.close();
		return data_all;
	}
	else {
		cout << "Unable to open file";
		return NULL;
	}
}

Matrix conv_mul(Matrix a, Matrix b){
	// perform conventional matrix multi
	assert(a.dims == b.dims);
	Matrix output_matrix(a.array_len);

	// iterate through the output matrix
	for (int i = 0; i < output_matrix.dims; i++){
		for (int j = 0; j < output_matrix.dims; j++){
			int sum =0;
			for (int k = 0; k < output_matrix.dims; k++){
				int a_val = a.data[(i*output_matrix.dims + k)];
				int b_val = b.data[(k*output_matrix.dims + j)];
				sum += a_val*b_val;
			}
			output_matrix.data[(i*output_matrix.dims) +j] = sum;
		}
	}
	return output_matrix;
}

Matrix m_add(Matrix a, Matrix b){
	// perform conventional matrix multi
	assert(a.dims == b.dims);
	Matrix output_matrix(a.dims);

	for (int i = 0; i < output_matrix.array_len; i++){
		output_matrix.data[i] = a.data[i] + b.data[i];
	}
	return output_matrix;
}

Matrix m_sub(Matrix a, Matrix b){
	// perform conventional matrix multi
	assert(a.dims == b.dims);
	Matrix output_matrix(a.dims);

	for (int i = 0; i < output_matrix.array_len; i++){
		output_matrix.data[i] = a.data[i] - b.data[i];
	}
	return output_matrix;
}

Matrix combine(Matrix a, Matrix b, Matrix c, Matrix d){

	Matrix output_matrix(pow(a.dims*2, 2))
	// go through output
	for (int i=0; i<output_matrix.dims; i++){
		for (int j=0; j<output_matrix; j++){
			if (i<a.dims){
				if (j<a.dims){
					// top left
					output_matrix.data[(i*output_matrix.dims) +j] = a.data[(i*a.dims) +j];
				}
				else{
					// top right
					output_matrix.data[(i*output_matrix.dims) +j] = b.data[(i*c.dims) + (j - b.dims)];
				}
			}
			else {
				if (j<a.dims){
					// bottom left
					output_matrix.data[(i*output_matrix.dims) +j] = c.data[((i - c.dims)*c.dims) +j];
				}
				else{
					// bottom right
					output_matrix.data[(i*output_matrix.dims) +j] = d.data[((i - d.dims)*d.dims) + (j - d.dims)];
				}
			}
		}
	}

	return output_matrix;
}

Matrix strass(int crossover, Matrix a, Matrix b){
	if (a.dims > crossover){
		//perform strassen
		Matrix a11 = split(a, 1);
		Matrix a12 = split(a, 2);
		Matrix a21 = split(a, 3);
		Matrix a22 = split(a, 4);

		Matrix b11 = split(b, 1);
		Matrix b12 = split(b, 2);
		Matrix b21 = split(b, 3);
		Matrix b22 = split(b, 4);


		Matrix m1 = conv_mul(m_add(a11, a22), m_add(b11, b22));
		Matrix m2 = conv_mul(m_add(a21, a22), b11);
		Matrix m3 = conv_mul(a11, m_sub(b12, b22));
		Matrix m4 = conv_mul(a22, m_sub(b21, b11));
		Matrix m5 = conv_mul(m_add(a11, a12), b22);
		Matrix m6 = conv_mul(m_sub(a21, a11), m_add(b11, b12));
		Matrix m7 = conv_mul(m_sub(a12, a22), m_add(b21, b22));

		Matrix c11 = m_add(m1, m_sub(m4, m_add(m5, m7)));
		Matrix c12 = m_add(m3, m5);
		Matrix c21 = m_add(m2, m4);
		Matrix c22 = m_sub(m1, m_add(m2, m_add(m3, m6)));
		return combine(c11,c12,c21,c22);
	}
	else{
		return conv_mul(a,b)
	}
}

bool matrix_equal(Matrix a, Matrix b){
	assert(a.dims == b.dims);
	for (int i=0; i < a.array_len; i++){
		if (a.data[i] != b.data[i]){
			cout << "Matrices are not equal\n";
			return false;
		}
	}
	cout << "Matrices are equal!\n";
	return true;
}

// int triangleCount(int probability){
//     //initialize a random 1024x1024 adjacency matrix
//     Matrix mat(1024); //we hardcode this 
//     int array_len = pow(1024,2);
//     mat.init_randadjacency(probability);
//     Matrix twomat = strass(mat, mat);
//     Matrix threemat = strass(twomat, mat);
//     //count number of triangles
//     int diagonaledges = 0;
//     for (int i = 0; i < array_len; i = i + array_len + 1){
//         //We only hit the diagonal entries by adding dim + 1 to i each time
//         if (mat.data[i] == 1){
//             diagonaledges = diagonaledges + 1;
//         }
//     }
//     int triangles = diagonaledges / 6;
//     printf("Triangles: %d", triangles);
//     return (triangles);
// }

int main(){
	cout << "conventional test: \n\n";
	// Matrix mat_a(dimension^2);
	// Matrix mat_b();
	// mat_a.init_rand();
	// mat_b.init_rand();
	// mat_a.print_matrix();
	// mat_b.print_matrix();
	

	int* data_ptr = read_in("ascii_file.txt");

	Matrix mat_c(data_ptr[0]);
	mat_c.read(data_ptr, "first");
	cout << "Matrix C: ";
	mat_c.print_matrix();

	Matrix mat_d(data_ptr[0]);
	mat_d.read(data_ptr, "second");
	cout << "Matrix D: ";
	mat_d.print_matrix();
	delete data_ptr;

	cout << "\nconventional dot product: ";
	Matrix dot = conv_mul(mat_c, mat_d);
	dot.print_matrix();

	int* sol_ptr = read_in("solution.txt");
	Matrix mat_sol(sol_ptr[0]*2);
	mat_sol.read(sol_ptr, "all");
	cout << "\nMatrix Solution: ";
	mat_sol.print_matrix();
	delete sol_ptr;

	matrix_equal(mat_sol, dot);
	// check correctnes against python version

	return 0;
}
// generates the matrices based on a given size, 


// run strassen until a cutoff point n after that normal matrix


// write a diagonaliser output