#pragma once
#include <iostream>

//#define DEBUG

namespace mx {

	template<typename items_type, int X, int Y>
	struct Array {
		items_type arr[X][Y];
	};

	template <typename items_type, int X, int Y>
	class Matrix {
	private:
		int m_x, m_y;
		items_type m_matrix[X][Y];
	public:
		Matrix() {
#ifdef DEBUG
			std::cout << '\n' << "=== === === ===" << "Constructor" << "=== === === ===" << '\n';
#endif
			m_x = X;
			m_y = Y;
			for (int i = 0; i < m_x; i++)
				for (int j = 0; j < m_y; j++)
					m_matrix[i][j] = 0;
		}

		Matrix(const Matrix<items_type, X, Y>& matrix) {
#ifdef DEBUG
			std::cout << '\n' << "=== === === ===" << "Constructor" << "=== === === ===" << '\n';
#endif
			m_x = matrix.m_x;
			m_y = matrix.m_y;
			for (int i = 0; i < m_x; i++)
				for (int j = 0; j < m_y; j++)
					m_matrix[i][j] = matrix.m_matrix[i][j];
		}

		Matrix(const Array<items_type, X, Y>& arr) {
#ifdef DEBUG
			std::cout << '\n' << "=== === === ===" << "Constructor" << "=== === === ===" << '\n';
#endif
			m_x = X;
			m_y = Y;
			for (int i = 0; i < m_x; i++)
				for (int j = 0; j < m_y; j++)
					m_matrix[i][j] = arr.arr[i][j];
		}

		template <typename items_type, int X, int Y>
		friend std::istream& operator>>(std::istream& c_input, Matrix<items_type, X, Y>& matrix);

		template <typename items_type, int X, int Y>
		friend std::ostream& operator<<(std::ostream& c_output, const Matrix<items_type, X, Y>& matrix);

		template <typename items_type, int X, int Y>
		Matrix<items_type, X, Y>& operator=(const Matrix<items_type, X, Y>& matrix) {
#ifdef DEBUG
			std::cout << '\n' << "=== === === ===" << "Operator (=)" << "=== === === ===" << '\n';
#endif
			m_x = matrix.getX();
			m_y = matrix.getY();
			for (int i = 0; i < m_x; i++)
				for (int j = 0; j < m_y; j++)
					m_matrix[i][j] = matrix.get(i, j);
			return *this;
		}

		//Getter and Setter
		int getX() const { return m_x; }
		int getY() const { return m_y; }
		items_type get(int i, int j) const { return m_matrix[i][j]; }
		void set(int i, int j, items_type item) { m_matrix[i][j] = item; }
		//Matrix<items_type, X, Y> getMatrix() const { return m_matrix; }

		template <typename items_type, int X, int Y>
		Matrix<items_type, X, Y> operator+(const Matrix<items_type, X, Y>& matrix) {
#ifdef DEBUG
			std::cout << '\n' << "=== === === ===" << "Operator (+)" << "=== === === ===" << '\n';
#endif
			Matrix<items_type, X, Y> tmp;
			if ((m_x != X) || (m_y != Y)) {
				std::cout << '\n' << "Operation cancelled!" << '\n';
				return tmp;
			}
			for (int i = 0; i < m_x; i++)
				for (int j = 0; j < m_y; j++) {
					tmp.set(i, j, m_matrix[i][j] + matrix.get(i, j));
				}
			return tmp;
		}

		template <typename items_type, int X, int Y>
		Matrix<items_type, X, Y> operator-(const Matrix<items_type, X, Y>& matrix) {
#ifdef DEBUG
			std::cout << '\n' << "=== === === ===" << "Operator (-)" << "=== === === ===" << '\n';
#endif
			Matrix<items_type, X, Y> tmp;
			if ((m_x != matrix.m_x) || (m_y != matrix.m_y)) {
				std::cout << '\n' << "Operation cancelled!" << '\n';
				return tmp;
			}
			for (int i = 0; i < m_x; i++)
				for (int j = 0; j < m_y; j++)
					tmp.set(i, j, m_matrix[i][j] - matrix.get(i, j));
			return tmp;
		}

		template <typename items_type, int X, int Y>
		Matrix<items_type, X, Y> operator*(const Matrix<items_type, X, Y>& matrix) {
#ifdef DEBUG
			std::cout << '\n' << "=== === === ===" << "Operator (*)" << "=== === === ===" << '\n';
#endif
			Matrix<items_type, X, Y> tmp;
			if (m_y != matrix.getX()) {
				std::cout << '\n' << "Operation cancelled!" << '\n';
				return tmp;
			}
			for (int i = 0; i < m_x; i++)
				for (int j = 0; j < matrix.getY(); j++) {
					items_type res = 0;
					for (int c = 0; c < m_y; c++) {
						res += m_matrix[i][c] * matrix.get(c, j);
					}
					tmp.set(i, j, res);
				}
			return tmp;
		}

		int determinant() {
#ifdef DEBUG
			std::cout << '\n' << "=== === === ===" << "Determinant" << "=== === === ===" << '\n';
#endif
			if (m_x == m_y) {
				if (m_x == 2)
					return (m_matrix[0][0] * m_matrix[1][1] - m_matrix[0][1] * m_matrix[1][0]);
				else if (m_x == 3)
					return (m_matrix[0][0] * m_matrix[1][1] * m_matrix[2][2]
						+ m_matrix[0][1] * m_matrix[1][2] * m_matrix[2][0]
						+ m_matrix[0][2] * m_matrix[1][0] * m_matrix[2][1]
						- m_matrix[0][2] * m_matrix[1][1] * m_matrix[2][0]
						- m_matrix[0][1] * m_matrix[1][0] * m_matrix[2][2]
						- m_matrix[0][0] * m_matrix[1][2] * m_matrix[2][1]);
				else {
					std::cout << '\n' << "Operation is not supported!" << '\n';
					return 0;
				}
			}
			else {
				std::cout << '\n' << "The matrix must be square!" << '\n';
				return 0;
			}
		}

		Matrix<items_type, X, Y> inverse() {
#ifdef DEBUG
			std::cout << '\n' << "=== === === ===" << "Inverse" << "=== === === ===" << '\n';
#endif
			Matrix<items_type, X, Y> tmp;
			if (m_x == m_y) {
				int det = this->determinant();
				if (det != 0) {
					if (m_x == 3) {
						double inv_det = 1.0 / det;
						tmp.set(0, 0,(m_matrix[1][1] * m_matrix[2][2] - m_matrix[2][1] * m_matrix[1][2]) * (inv_det / 1.0));
						tmp.set(0, 1, (m_matrix[0][1] * m_matrix[2][2] - m_matrix[0][2] * m_matrix[2][1]) * (inv_det / 1.0) * (-1.0));
						tmp.set(0, 2, (m_matrix[0][1] * m_matrix[1][2] - m_matrix[0][2] * m_matrix[1][1]) * (inv_det / 1.0));
						tmp.set(1, 0, (m_matrix[1][0] * m_matrix[2][2] - m_matrix[1][2] * m_matrix[2][0]) * (inv_det / 1.0) * (-1.0));
						tmp.set(1, 1, (m_matrix[0][0] * m_matrix[2][2] - m_matrix[0][2] * m_matrix[2][0]) * (inv_det / 1.0));
						tmp.set(1, 2, (m_matrix[0][0] * m_matrix[1][2] - m_matrix[1][0] * m_matrix[0][2]) * (inv_det / 1.0) * (-1.0));
						tmp.set(2, 0, (m_matrix[1][0] * m_matrix[2][1] - m_matrix[2][0] * m_matrix[1][1]) * (inv_det / 1.0));
						tmp.set(2, 1, (m_matrix[0][0] * m_matrix[2][1] - m_matrix[2][0] * m_matrix[0][1]) * (inv_det / 1.0) * (-1.0));
						tmp.set(2, 2, (m_matrix[0][0] * m_matrix[1][1] - m_matrix[1][0] * m_matrix[0][1]) * (inv_det / 1.0));
					}
					else if (m_x == 2) {
						double inv_det = 1.0 / det;
						tmp.set(0, 0, m_matrix[1][1] * (inv_det / 1.0));
						tmp.set(0, 1, m_matrix[0][1] * (inv_det / 1.0) * (-1.0));
						tmp.set(1, 0, m_matrix[1][0] * (inv_det / 1.0) * (-1.0));
						tmp.set(1, 1, m_matrix[0][0] * (inv_det / 1.0));
					}
					else {
						std::cout << '\n' << "Operation is not supported!" << '\n';
					}
				}
				else {
					std::cout << '\n' << "Determinant is 0!" << '\n';
				}
			}
			else {
				std::cout << '\n' << "The matrix must be square!" << '\n';
			}
			return tmp;
		}

		Matrix<items_type, X, Y> transpose() {
#ifdef DEBUG
			std::cout << '\n' << "=== === === ===" << "Transpose" << "=== === === ===" << '\n';
#endif
			Matrix<items_type, Y, X> tmp;
			for (int i = 0; i < m_x; i++)
				for (int j = i; j < m_y; j++) {
					tmp.m_matrix[i][j] = m_matrix[j][i];
					tmp.m_matrix[j][i] = m_matrix[i][j];
				}
			return tmp;
		}

		~Matrix() {
#ifdef DEBUG
			std::cout << '\n' << "=== === === ===" << "Destructor" << "=== === === ===" << '\n';
#endif
		}
	};

	template <typename items_type, int X, int Y>
	std::istream& operator>>(std::istream& c_input, Matrix<items_type, X, Y>& matrix) {
		for (int i = 0; i < matrix.m_x; i++)
			for (int j = 0; j < matrix.m_y; j++)
				c_input >> matrix.m_matrix[i][j];
		return c_input;
	}

	template <typename items_type, int X, int Y>
	std::ostream& operator<<(std::ostream& c_output, const Matrix<items_type, X, Y>& matrix) {
		for (int i = 0; i < matrix.m_x; i++) {
			for (int j = 0; j < matrix.m_y; j++)
				c_output << matrix.m_matrix[i][j] << ' ';
			c_output << '\n';
		}
		return c_output;
	}

	using Vec2int = Matrix<int, 2, 1>;
	using Vec2dbl = Matrix<double, 2, 1>;
	using Vec3dbl = Matrix<double, 3, 1>;
	using Mx2int = Matrix<int, 2, 2>;
	using Mx2dbl = Matrix<double, 2, 2>;
	using Mx3int = Matrix<int, 3, 3>;
	using Mx3dbl = Matrix<double, 3, 3>;

}