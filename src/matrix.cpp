#include "matrix.h"

Matrix::Matrix( int rows, int columns)
			:rows(rows),
			 columns(columns),
			 elements(rows, std::vector<float>(columns, 0)){}

Matrix::Matrix(std::initializer_list<std::initializer_list<float>> items)
			: elements(items.begin(),items.end())
		{
			rows = items.size();
			columns = rows ? elements[0].size() : 0;
		}


Matrix& Matrix::operator+=(const Matrix other){
	if(other.rows != rows || other.columns != columns)
		throw std::invalid_argument("Matrix dimensions must match");

	for(int i = 0; i < rows; i++)
		for(int j = 0; j < columns; j++)
			elements[i][j] = elements[i][j] + other.elements[i][j];
	return *this; 
}

Matrix& Matrix::operator-=(const Matrix other){
	if(other.rows != rows || other.columns != columns)
		throw std::invalid_argument("Matrix dimensions must match");

	for(int i = 0; i < rows; i++)
		for(int j = 0; j < columns; j++)
			elements[i][j] = elements[i][j] - other.elements[i][j];
	return *this; 
}


void Matrix::resize(int newRows, int newColumns) {
	elements.resize(newRows);                           
	for (auto& row : elements)
		row.assign(newColumns, 0);                    
	rows = newRows;
	columns = newColumns;
}

Matrix& Matrix::operator*=(const Matrix& other){
	if(other.rows != columns)
		throw std::invalid_argument("Matrix dimensions are not compatable for multiplication");

	Matrix copy = *this;
	resize(rows, other.columns);
	int index = 0;
	for(int i = 0; i < copy.rows; i++)
		for(int j = 0; j < other.columns; j++){
			for(int k = 0; k < copy.columns; k++){
				elements[i][j] += copy.elements[i][k] * other.elements[k][j];
			}
		}
	return *this;
}

Matrix Matrix::operator*(const Matrix other) const{
	Matrix result = *this;
	result*=other;
	return result;
}

Matrix Matrix::operator+(const Matrix other) const{
	Matrix result = *this;
	result += other;
	return result;
}
Matrix Matrix::operator-(const Matrix other) const{
	Matrix result = *this;
	result -= other;
	return result;
}

std::vector<float>& Matrix::operator[](int i){
	return elements[i];
}


Matrix Matrix::transpose(){
	Matrix result(columns, rows);
	for(int i = 0; i < rows; i++)
		for(int j = 0; j < columns; j++)
			result.elements[j][i] = elements[i][j]; 
	return result;
}

void Matrix::apply(std::function<float(float)> func){
	for(int i = 0; i < rows; i++)
		for(int j = 0; j < columns; j++){
			elements[i][j] = func(elements[i][j]);
		}
}
void Matrix::replace(std::function<float(void)> func){
	for(int i = 0; i < rows; i++)
		for(int j = 0; j < columns; j++){
			elements[i][j] = func();
		}
}

Matrix Matrix::modify(std::function<float(float)> func){
	Matrix copy(rows, columns);
	for(int i = 0; i < rows; i++)
		for(int j = 0; j < columns; j++){
			copy.elements[i][j] = func(elements[i][j]);
		}
	return copy;
}

