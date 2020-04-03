#include <iostream>
#include <string>
#include <fstream>
# include <cmath>
#include <chrono>
using namespace std;

// NOTES
// Change to be able to handle 32 bit numbers - perhaps use long int

//Global debug tool
bool debug = false;

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

void write_out(string filename, bool add, Matrix a){

	fstream out_file(filename);
	if (out_file.is_open()){
		if (add) {
			out_file.seekp (0, ios::end);
		}
		for (int i=0; i<a.array_len; i++){
			out_file << a.data[i] << "\n";
		}
		out_file.close();
	}
	else cout << "unable to open output file \n";
}

Matrix conv_mul(Matrix a, Matrix b){
	// perform conventional matrix multi
	//assert(a.dims == b.dims);
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
	assert(a.dims == b.dims);
	if (debug) (cout << "Doing Add, A dims: " << a.dims << " B dims: " << b.dims << "\n");
	Matrix output_matrix(a.array_len);
	for (int i = 0; i < output_matrix.array_len; i++){
		output_matrix.data[i] = a.data[i] + b.data[i];
	}
	return output_matrix;
}

Matrix m_sub(Matrix a, Matrix b){
	assert(a.dims == b.dims);
	if (debug) (cout << "Doing Subtract, A dims: " << a.dims << " B dims: " << b.dims << "\n");
	Matrix output_matrix(a.array_len);

	for (int i = 0; i < output_matrix.array_len; i++){
		output_matrix.data[i] = a.data[i] - b.data[i];
	}
	return output_matrix;
}

Matrix combine(Matrix a, Matrix b, Matrix c, Matrix d){

	Matrix output_matrix(pow(a.dims*2, 2));
	// go through output
	for (int i=0; i<output_matrix.dims; i++){
		for (int j=0; j<output_matrix.dims; j++){
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

Matrix split(Matrix a, int ot_i, int ot_j){
	Matrix output_matrix(pow(a.dims/2, 2));
	if (debug) (cout << "Doing split, input dims: " << a.dims << " Output dims: " << output_matrix.dims << "\n");
	for (int i=0; i<output_matrix.dims; i++){
		for (int j=0; j<output_matrix.dims; j++){
			output_matrix.data[(i*output_matrix.dims) +j] = a.data[((i+ot_i*output_matrix.dims)*a.dims) +j +ot_j*output_matrix.dims];
		}
	}
	
	return output_matrix;
}

Matrix strass(int crossover, Matrix a, Matrix b){
	if (a.dims > crossover){
		//perform strassen
		if (debug) (cout << "Doing Strass, dims of a: " << a.dims << " dims of b: " << b.dims << "\n");
		Matrix a11 = split(a, 0, 0);
		Matrix a12 = split(a, 0, 1);
		Matrix a21 = split(a, 1, 0);
		Matrix a22 = split(a, 1, 1);

		Matrix b11 = split(b, 0, 0);
		Matrix b12 = split(b, 0, 1);
		Matrix b21 = split(b, 1, 0);
		Matrix b22 = split(b, 1, 1);


		Matrix m1 = strass(crossover, m_add(a11, a22), m_add(b11, b22));
		Matrix m2 = strass(crossover, m_add(a21, a22), b11);
		Matrix m3 = strass(crossover, a11, m_sub(b12, b22));
		Matrix m4 = strass(crossover, a22, m_sub(b21, b11));
		Matrix m5 = strass(crossover, m_add(a11, a12), b22);
		Matrix m6 = strass(crossover, m_sub(a21, a11), m_add(b11, b12));
		Matrix m7 = strass(crossover, m_sub(a12, a22), m_add(b21, b22));

		Matrix c11 = m_add(m1, m_sub(m4, m_add(m5, m7)));
		Matrix c12 = m_add(m3, m5);
		Matrix c21 = m_add(m2, m4);
		Matrix c22 = m_sub(m1, m_add(m2, m_add(m3, m6)));

		if (debug){
			cout << "Completed strassen arthmetic, Dim Check!,";
			cout << "\nc11: " << c11.dims << "\nc12: " << c12.dims << "\nc21: " << c21.dims << "\nc22: " << c22.dims << "\n";	
		}
		
		return combine(c11,c12,c21,c22);
	}
	else{
		if (debug) (cout << "Doing Conventional dims of a: " << a.dims << " dims of b: " << b.dims << "\n");
		return conv_mul(a,b);
	}
}

void fullOptimize(int bottom, int top, Matrix mat_a, Matrix mat_b){
    //Find goal time
    auto start1 = chrono::high_resolution_clock::now(); 
    strass(top, mat_a, mat_b);
    auto stop1 = chrono::high_resolution_clock::now(); 
    auto durationtop = chrono::duration_cast<chrono::microseconds>(stop1 - start1); 

    auto start2 = chrono::high_resolution_clock::now(); 
    strass(bottom, mat_a, mat_b);
    auto stop2 = chrono::high_resolution_clock::now(); 
    auto durationbottom = chrono::duration_cast<chrono::microseconds>(stop2 - start2); 
    while (top - bottom > 10){
        if (durationtop > durationbottom){
            top = (top+bottom)/2;
            cout << top << "\t";
            cout << bottom << "\t";
            cout << "up" << "\n";
            fullOptimize(bottom, top, mat_a, mat_b);
        } 
        else {
            bottom = (top+bottom)/2;
            cout << top << "\t";
            cout << bottom <<"\t" ;
            cout << "down" << "\n";
            fullOptimize(bottom,top, mat_a, mat_b);
        }
    }
}

int simplecalc(int crossover, Matrix a, Matrix b){
    //Find time
    auto start = chrono::high_resolution_clock::now(); 
    strass(crossover, a, b);
    auto stop = chrono::high_resolution_clock::now(); 
    auto durationS = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << durationS.count();
    return 0;
}


bool matrix_equal(Matrix a, Matrix b){
	//assert(a.dims == b.dims);
	for (int i=0; i < a.array_len; i++){
		if (a.data[i] != b.data[i]){
			cout << "Matrices are not equal\n";
			return false;
		}
	}
	cout << "Matrices are equal!\n";
	return true;
}

 int triangleCount(int probability, int dimension){
    //initialize a random 1024x1024 adjacency matrix
    int array_len = pow(dimension,2);
    Matrix mat(array_len); 
    mat.init_randadjacency(probability);
    Matrix twomat = strass(654, mat, mat);
    Matrix threemat = strass(654, twomat, mat);
    
    //count number of triangles
    int diagonaledges = 0;
    for (int i = 0; i < array_len; i = i + dimension + 1){
        //We only hit the diagonal entries by adding dim + 1 to i each time
        if (mat.data[i] > 0){
            diagonaledges = diagonaledges + mat.data[i];
            cout << "triangle edge found";
        }
    }
    int triangles = diagonaledges / 6;
    printf("Triangles: %d", triangles);
    return (triangles);
}

//change

// int main(int argc, char *argv[]){
// 	if (argc != 2){
// 		cout << "Try again with 2 command line arguments";
// 		return 0;
// 	}
// 	int dimension = stoi(argv[0]);
// 	string filename = argv[1];

int main(){
	triangleCount(4, 3);
	//int dimension = 32;
	//cout << "conventional test: \n\n";
	//Matrix mat_a(pow(dimension, 2));
	// Matrix mat_b(pow(dimension, 2));
	// mat_a.init_rand();
	// mat_b.init_rand();
	// write_out("ascii_file.txt",0, mat_a);
	// write_out("ascii_file.txt",1, mat_b);

	// mat_a.print_matrix();
	// mat_b.print_matrix();
	// simplecalc(625, mat_a, mat_b);
	// fullOptimize(0, 1024, mat_a, mat_b);
  
	//debug = true;
	
	//cout << "Reading in file data \n";
	//int* data_ptr = read_in("ascii_file.txt");
    
	//Matrix mat_c(data_ptr[0]);
	//mat_c.read(data_ptr, "first");
	// cout << "Matrix C: ";
	// mat_c.print_matrix();

	//Matrix mat_d(data_ptr[0]);
	//mat_d.read(data_ptr, "second");
	// cout << "Matrix D: ";
	// mat_d.print_matrix();
	//delete data_ptr;

	// cout << "Testing combine and split \n";

	// cout << "Combined: ";
	// Matrix test1 = combine(mat_c, mat_c, mat_c, mat_c);
	// test1.print_matrix();

	// cout << "Split: ";
	// Matrix test2 = split(test1, 0, 1);
	// test2.print_matrix();


	cout << "Testing Strass crossover \n";
	Matrix strass_out = strass(8, mat_c, mat_d);

	strass_out.print_matrix();

	int* sol_ptr = read_in("solution.txt");
	Matrix mat_sol(sol_ptr[0]*2);
	mat_sol.read(sol_ptr, "all");
	cout << "\nMatrix Solution: ";
	mat_sol.print_matrix();
	delete sol_ptr;
	matrix_equal(mat_sol, strass_out);

	// cout << "\nconventional dot product: ";
	// Matrix dot = conv_mul(mat_c, mat_d);
	// dot.print_matrix();

	

	

	// check correctnes against python version

	return 0;
}

// generates the matrices based on a given size, 


// run strassen until a cutoff point n after that normal matrix


// write a diagonaliser output