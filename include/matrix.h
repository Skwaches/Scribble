#pragma once

#include <vector>
#include <functional>
#include <iostream>
#include <format>

class Matrix {
	public:
		int rows, columns;
		std::vector< std::vector<float> > elements;
	public:
		Matrix(int rows = 1, int columns = 1);
		Matrix(std::initializer_list<std::initializer_list<float>> items);
		Matrix& operator+=(const Matrix other);
		Matrix& operator-=(const Matrix other);
		void resize(int newRows, int newColumns) ;
		Matrix& operator*=(const Matrix& other);
		Matrix operator*(const Matrix other) const;
		Matrix operator+(const Matrix other) const;
		Matrix operator-(const Matrix other) const;
		std::vector<float>& operator[](int i);
		Matrix transpose();
		Matrix centerByMass();
		
		//Pass all the elements through a function and assign the element the output.
		void apply(std::function<float(float)> func);

		//Replace all the elements by the output of a function.
		void replace(std::function<float(void)> func);

		//Pass all the elements through a function and assign the output to a new matrix. Leaving the original unchanged	
		Matrix modify(std::function<float(float)> func);

		//Pass all the elements through a function and do nothing.
		template<typename Func>
			void pass(Func fn){
				for(int i = 0; i < rows; i++)
					for(int j = 0; j < columns; j++)
						fn(elements[i][j]);
			}

		friend std::ostream& operator<<(std::ostream& os, const Matrix& A) {
			for (const auto& row : A.elements) {
				for (const auto& val : row)
					os << std::format("{:.5f}",val) << "\t";
				os << "\n";
			}
			return os;
		}
};
