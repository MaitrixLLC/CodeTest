// interview_question1.cpp : By Eric Olsen for Facebook coding test
//

#include <string>
#include <iostream>
#include <fstream>
#include <limits>
#include <vector>

using namespace std;

typedef char BYTE;


unsigned int RangedRand(int range_min, int range_max);
void waitKey(void);
char val_to_hexdigit(int val);
BYTE hexdigits_to_val(char c1, char c2);
int write_demo_file(string filename, unsigned int num_bytes);

class ConvertToMatrix {

public:
	ConvertToMatrix(unsigned int rows, unsigned int cols, string filename);
	BYTE accessMatrix(unsigned int row, unsigned int col);
	void printMatrix(void);

protected:
	vector<vector<BYTE>> mat;

private:
	unsigned int Rows;
	unsigned int Cols;

};

BYTE ConvertToMatrix::accessMatrix(unsigned int row, unsigned int col)
{
	if ((row < Rows) && (col < Cols)) {
		return(mat[row][col]);
	}
	else {
		cerr << "Access beyond matrix dimensions" << endl;
	}

}

void ConvertToMatrix::printMatrix(void)
{
	cout << endl;
	cout << "Matrix dump: " << endl;

	for (int row = 0; row < mat.size(); row++) {
		for (int col = 0; col < mat[row].size(); col++) {
			cout << (unsigned int)(mat[row][col] & 0xff) << " ";
		}
		cout << endl;

	}

}

#define MAX_ROWS  100
#define MAX_COLS  100
ConvertToMatrix::ConvertToMatrix(unsigned int rows, unsigned int cols, string filename)
{
	Rows = 0;
	Cols = 0;

	if ((rows <= MAX_ROWS) && (cols <= MAX_COLS)) {
		mat.resize(rows);
		for (int i = 0; i < mat.size(); i++) {
			mat[i].resize(cols);
		}
		Rows = rows;
		Cols = cols;
	}
	else {
		return;
	}

	// fill a matrix with zeros first
	for (int row = 0; row < mat.size(); row++) {
		for (int col = 0; col < mat[row].size(); col++) {
			mat[row][col] = 0;
		}
		cout << endl;
	}

	ifstream infile;
	infile.open(filename, ifstream::in);

	char lsc, msc;
	unsigned int row = 0;
	unsigned int col = 0;
	do {

		do {							// any character can be a space or CR, read until real char
			msc = infile.get();
		} while (((msc == ' ') || (msc == '\n')) && (msc != NULL));

		do {							// any character can be a space or CR, read until real char
			lsc = infile.get();
		} while (((lsc == ' ') || (lsc == '\n')) && (lsc != NULL));

		if (msc != NULL) {
			mat[row][col] = hexdigits_to_val(lsc, msc);
			row += 1;
			if (row == Rows) {
				row = 0;
				col += 1;
				if (col == Cols) {
					break;
				}
			}
		}

	} while (msc != NULL);

}


int main()
{
	int i;
	char hex_digit;

/*
	cout << "writing demo file..." << endl;
	write_demo_file("test1.txt", 200);
	cout << "done ..." << endl;
*/

	ConvertToMatrix *matrix = new ConvertToMatrix(10, 14, "test1.txt");
	matrix->printMatrix();		// print the matrix in decimal format

	waitKey();


}

// study the limits syntax
void waitKey(void)
{

	std::cout << "Press ENTER to continue...";
	std::cin.ignore(std::numeric_limits <std::streamsize> ::max(), '\n');
	
}


// create a test file that has random spaces, hex digits, and varying line lengths
#define MAX_LINE_LEN  80
#define PROBABILITY_OF_SPACE 13
int write_demo_file(string filename, unsigned int num_bytes)
{
	char hex_digit;
	unsigned int line_len = 0;
	int place_a_space;

	ofstream outfile;

	num_bytes *= 2;		// two hex digits per byte, always even

	outfile.open(filename);

	while (num_bytes) {

		if (num_bytes > MAX_LINE_LEN) {
			line_len = RangedRand(0, MAX_LINE_LEN);
		}
		else {
			line_len = num_bytes;		// last line is remaining bytes
		}

		num_bytes -= line_len;

		while (line_len) {
			hex_digit = val_to_hexdigit(RangedRand(0, 16));
			outfile.write(&hex_digit, 1);

			unsigned int prob = RangedRand(0, PROBABILITY_OF_SPACE);

			if (prob == PROBABILITY_OF_SPACE - 1) {
				char c = ' ';
				outfile.write(&c, 1);
			}

			line_len -= 1;
		}

		char end = '\n';			// write an end of line character
		outfile.write(&end, 1);


	}

	outfile.close();
	return(0);
	
}

unsigned int RangedRand(int range_min, int range_max)
{
	unsigned int u;

	if (range_max > range_min) {
		u = (double)rand() / (RAND_MAX + 1) * (range_max - range_min) + range_min;
	}
	else {
		return(0);
	}

	return(u);

}


char val_to_hexdigit(int val)
{

	if (val < 10) {
		return(val + '0');
	}
	else if (val < 16) {
		return((val-10) + 'a');
	}
	else {
		return(0);
	}

}

BYTE hexdigits_to_val(char lsc, char msc)
{
	BYTE val = 0;

	if ((lsc >= '0') && (lsc <= '9')) {
		val += lsc - '0';
	}
	else if ((lsc >= 'a') && (lsc <= 'f')) {
		val += lsc - 'a';
	}
	else {	// ERROR
		cerr << "Error in hexdigits_to_val" << endl;
	}

	if ((msc >= '0') && (msc <= '9')) {
		val += (msc - '0') * 16;
	}
	else if ((msc >= 'a') && (msc <= 'f')) {
		val += (msc - 'a') * 16;
	}
	else {
		cerr << "Error in hexdigits_to_val" << endl;
	}

	return(val);

}

