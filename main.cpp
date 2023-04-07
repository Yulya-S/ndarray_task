#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
using namespace std;
class error {};

template<typename T> class NDArray {
	vector<T> elements;
	int Shape[2];
	int Len;
	bool checking_boundaries(const int X, const int Y) { return (X < Shape[0] && X > -1 && Y < Shape[1] && Y > -1); }
public:
	NDArray(const int X, const int Y = 1) {
		Shape[0] = X;
		Shape[1] = Y;
		Len = X * Y;
		for (int i = 0; i < Len; i++) elements.push_back(0);
	}
	int shape(int index) { return Shape[index]; }
	int len() { return Len; }

	NDArray zeroNDArray() { for (int i = 0; i < Len; i++) elements[i] = 0; return *this; }
	NDArray unitNDArray() { for (int i = 0; i < Len; i++) elements[i] = 1; return *this; }
	NDArray rndNDArray(T numbersTheRandomizer) {
		srand((unsigned)time(0));
		for (int i = 0; i < Len; i++) elements[i] = rand() % numbersTheRandomizer;
		return *this;
	}
	NDArray transpositions() {
		NDArray<int> replica(Shape[1], Shape[0]);
		int n = 0;
		for (int i = 0; i < Shape[0]; i++)
			for (int l = 0; l < Shape[1]; l++) {
				replica.elements[n] = elements[l * Shape[0] + i];
				n++;
			}
		*this = replica;
		return *this;
	}
	NDArray mulArray(NDArray other) {
		if (other.Shape[0] != Shape[0]) throw error();
		NDArray<T> replica = other.transpositions();
		NDArray<T> newArr(Shape[1], replica.Shape[0]);
		newArr.zeroNDArray();
		int n = 0;
		for (int i = 0; i < Shape[1]; i++) for (int l = 0; l < replica.Shape[0]; l++) {
			for (int u = 0; u < Shape[0]; u++) newArr.elements[n] += elements[i * Shape[0] + u] * replica.elements[u * replica.Shape[0] + l];
			n++;
		}
		*this = newArr;
		return *this;
	}

	NDArray operator = (NDArray replica) {
		elements = replica.elements;
		Len = replica.Len;
		Shape[0] = replica.Shape[0];
		Shape[1] = replica.Shape[1];
		return *this;
	}
	NDArray operator += (NDArray value) {
		if (Shape[0] != value.Shape[0] or Shape[1] != value.Shape[1]) throw error();
		for (int i = 0; i < Len; i++) { elements[i] += value.elements[i]; }
		return *this;
	}
	NDArray operator -= (NDArray value) {
		if (Shape[0] != value.Shape[0] or Shape[1] != value.Shape[1]) throw error();
		for (int i = 0; i < Len; i++) { elements[i] -= value.elements[i]; }
		return *this;
	}
	NDArray operator *= (NDArray value) {
		if (Shape[0] != value.Shape[0] or Shape[1] != value.Shape[1]) throw error();
		for (int i = 0; i < Len; i++) { elements[i] *= value.elements[i]; }
		return *this;
	}
	NDArray operator /= (NDArray value) {
		if (value.Len == 0 or Shape[0] != value.Shape[0] or Shape[1] != value.Shape[1]) throw error();
		for (int i = 0; i < Len; i++) { if (elements[i] != 0) elements[i] /= value.elements[i]; }
		return *this;
	}
	NDArray operator + (NDArray value) { NDArray Replica = *this; return Replica += value; }
	NDArray operator - (NDArray value) { NDArray Replica = *this; return Replica -= value; }
	NDArray operator * (NDArray value) { NDArray Replica = *this; return Replica *= value; }
	NDArray operator / (NDArray value) { NDArray Replica = *this; return Replica /= value; }
	T& operator() (int X, int Y = 0) {
		if (checking_boundaries(X, Y)) return elements[Y * Shape[0] + X];
		else throw error();
	}
	ostream& print(ostream& out) {
		for (int i = 0; i < Shape[1]; i++) {
			out << "[";
			for (int l = 0; l < Shape[0]; l++) {
				out << " " << elements[i * Shape[0] + l];
			}
			out << " ]" << endl;
		}
		return out;
	}

	float mean() {
		float summ = 0;
		for (int i = 0; i < Len; i++) summ += elements[i];
		return summ / Len;
	}
	float mean(int index) {
		float summ = 0;
		for (int i = 0; i < Shape[0]; i++) summ += elements[index * Shape[0] + i];
		return summ / Shape[0];
	}
	T max() {
		T max = elements[0];
		for (int i = 1; i < Len; i++) if (elements[i] > max) max = elements[i];
		return max;
	}
	T max(int index) {
		T max = elements[index * Shape[0]];
		for (int i = 0; i < Shape[0]; i++) if (elements[index * Shape[0] + i] > max) max = elements[index * Shape[0] + i];
		return max;
	}
	T min() {
		T min = elements[0];
		for (int i = 1; i < Len; i++) if (elements[i] < min) min = elements[i];
		return min;
	}
	T min(int index) {
		T min = elements[index * Shape[0]];
		for (int i = 0; i < Shape[0]; i++) if (elements[index * Shape[0] + i] < min) min = elements[index * Shape[0] + i];
		return min;
	}

};

int main() {
	NDArray<int> arr(3, 2);
	arr(0, 0) = 15;
	cout << arr(0, 0) << endl;
	arr.rndNDArray(10);
	cout << arr(0, 0) << endl;
	arr.print(cout);

	arr.transpositions();
	cout<< endl;
	arr.print(cout);
	cout << arr.max() << " " << arr.max(1) << endl;
	cout << arr.min() << " " << arr.min(1) << endl;
	cout << arr.mean() << " " << arr.mean(1) << endl << endl;

	NDArray<int> one(15);
	one.rndNDArray(44);
	one.print(cout);
	cout << one(0) << endl;

	one = arr;
	one += arr;
	one.print(cout);
	cout << endl;

	one = arr;
	one -= arr;
	one.print(cout);
	cout << endl;

	one = arr;
	one *= arr;
	one.print(cout);
	cout << endl;

	one = arr;
	one /= arr;
	one.print(cout);
	cout << endl;

	NDArray<int> line(2);
	line.rndNDArray(5);
	line.print(cout);
	cout << endl;
	arr.print(cout);
	cout << endl;
	arr.mulArray(line);
	arr.print(cout);

	return 0;
}