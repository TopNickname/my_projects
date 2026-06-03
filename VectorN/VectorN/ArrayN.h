#pragma once
#include <string>
#include <initializer_list>
#include <cmath>
#include <ostream>
#include <algorithm> // :(





namespace std_plus {

	template<typename T, const std::size_t N>
	class ArrayN {
		T data[N];
	public:
		T& operator[](size_t i);
		T operator[](size_t i) const;
		ArrayN(const T& el = T());
		ArrayN(std::initializer_list<T> list);
		ArrayN(const T(&arr)[N]);

		T* begin()  noexcept;
		T* end()  noexcept;
		T front() const;
		T back() const;
		long long rfind(const T& el) const;
		long long find(const T& el) const;
		size_t count(const T& el) const;
		size_t size() const noexcept;
		void swap(ArrayN<T, N>& v) noexcept;
		bool operator==(const ArrayN<T, N>& v) const;
		bool operator<(const ArrayN<T, N>& v) const;
		bool operator>(const ArrayN<T, N>& v) const;
		bool operator<=(const ArrayN<T, N>& v) const;
		bool operator>=(const ArrayN<T, N>& v) const;
		bool operator!=(const ArrayN<T, N>& v) const;

#pragma region math
		ArrayN operator+(const ArrayN<T, N>& v) const;
		ArrayN operator-(const ArrayN<T, N>& v) const;
		ArrayN& operator+=(const ArrayN<T, N>& v);
		ArrayN& operator-=(const ArrayN<T, N>& v);
		T operator*(const ArrayN<T, N>& v) const;
		ArrayN operator*(const T c) const;
		double length() const;
		ArrayN normalize() const;
		void abs();
#pragma endregion
	};

#pragma region init
	template<typename T, const size_t N>
	inline ArrayN<T, N>::ArrayN(const T& el) {
		for (int i = 0; i < N; i++) {
			data[i] = el;
		}
	}

	template<typename T, const size_t N>
	inline ArrayN<T, N>::ArrayN(const T(&arr)[N]) {
		for (int i = 0; i < N; i++) {
			data[i] = arr[i];
		}
	}

	template<typename T, const size_t N>
	inline ArrayN<T, N>::ArrayN(std::initializer_list<T> list) {
		if (N != list.size()) {
			throw "Error: ID 10T";
		}
		std::copy(list.begin(), list.end(), data);
	}
#pragma endregion

#pragma region operator[]
	template<typename T, const size_t N>
	inline T& ArrayN<T, N>::operator[](size_t i) {
		if (i >= N) {
			throw "out of range";
		}
		return data[i];
	}

	template<typename T, const size_t N>
	inline T ArrayN<T, N>::operator[](size_t i) const {
		if (i >= N) {
			throw "out of range";
		}
		return data[i];
	}
#pragma endregion

#pragma region front-back-size

	template<typename T, const size_t N>
	inline size_t ArrayN<T, N>::size() const noexcept {
		return N;
	}

	template<typename T, const size_t N>
	inline T ArrayN<T, N>::front() const {
		if (N == 0) {
			throw "out of range";
		}
		return data[0];
	}

	template<typename T, const size_t N>
	inline T ArrayN<T, N>::back() const {
		if (N == 0) {
			throw "out of range";
		}
		return data[N - 1];
	}

	template<typename T, const size_t N>
	inline T* ArrayN<T, N>::begin()  noexcept {
		return data;
	}

	template<typename T, const size_t N>
	inline T* ArrayN<T, N>::end()  noexcept {
		return (data + N);
	}

#pragma endregion

#pragma region algorithm

	template<typename T, const size_t N>
	inline void ArrayN<T, N>::swap(ArrayN<T, N>& arr) noexcept {
		T* data_tmp = arr.data;
		arr.data = data;
		data = data_tmp;
	}


	template<typename T, const size_t N>
	inline long long ArrayN<T, N>::find(const T& obj) const {
		long long ret = -1;
		for (size_t i = 0; i < N; i++) {
			if (data[i] == obj) {
				ret = i;
				break;
			}
		}
		return ret;
	}

	template<typename T, const size_t N>
	inline long long ArrayN<T, N>::rfind(const T& obj) const {
		long long ret = -1;
		for (size_t i = N; i != 0; i--) {
			if (data[i - 1] == obj) {
				ret = i - 1;
				break;
			}
		}
		return ret;
	}

	template<typename T, const size_t N>
	inline size_t ArrayN<T, N>::count(const T& el) const {
		size_t ret = 0;
		for (size_t i = 0; i < N; i++) {
			ret += (int)(data[i] == el);
		}
		return ret;
	}


#pragma endregion

#pragma region bool

	template<typename T, const size_t N>
	inline bool ArrayN<T, N>::operator==(const ArrayN<T, N>& v) const {
		bool ret = true;
		for (size_t i = 0; i < N; i++) {
			if (data[i] != v[i]) {
				ret = false;
				break;
			}
		}
		return ret;

	}

	template<typename T, const size_t N>
	inline bool ArrayN<T, N>::operator!=(const ArrayN<T, N>& v) const {
		return !(*this == v);
	}

	template<typename T, const size_t N>
	inline bool ArrayN<T, N>::operator<(const ArrayN<T, N>& v) const {
		int flag = 0;
		for (size_t i = 0; i < N; i++) {
			if (data[i] == v[i]) {
				continue;
			}
			if (data[i] > v[i]) {
				flag = -1;
				break;
			}
			if (data[i] < v[i]) {
				flag = 1;
				break;
			}
		}
		return (flag == 1);
	}

	template<typename T, const size_t N>
	inline bool ArrayN<T, N>::operator>(const ArrayN<T, N>& v) const {
		int flag = 0;
		for (size_t i = 0; i < N; i++) {
			if (data[i] == v[i]) {
				continue;
			}
			if (data[i] < v[i]) {
				flag = -1;
				break;
			}
			if (data[i] > v[i]) {
				flag = 1;
				break;
			}
		}
		return (flag == 1);

	}

	template<typename T, const size_t N>
	inline bool ArrayN<T, N>::operator>=(const ArrayN<T, N>& v) const {
		return !(*this < v);
	}

	template<typename T, const size_t N>
	inline bool ArrayN<T, N>::operator<=(const ArrayN<T, N>& v) const {
		return !(*this > v);
	}

#pragma endregion

#pragma region math

	template<typename T, const size_t N>
	inline ArrayN<T, N> ArrayN<T, N>::operator+(const ArrayN<T, N>& v) const {
		ArrayN<T, N> New_vec;
		for (size_t i = 0; i < N; i++) {
			New_vec[i] = (*this)[i] + v[i];
		}
		return New_vec;
	}

	template<typename T, const size_t N>
	inline ArrayN<T, N> ArrayN<T, N>::operator-(const ArrayN<T, N>& v) const {
		ArrayN<T, N> New_vec;
		for (size_t i = 0; i < N; i++) {
			New_vec[i] = (*this)[i] - v[i];
		}
		return New_vec;
	}

	template<typename T, const size_t N>
	inline T ArrayN<T, N>::operator*(const ArrayN<T, N>& v) const {
		T res = 0;
		size_t n = N;
		for (size_t i = 0; i < n; i++) {
			res += data[i] * v[i];
		}
		return res;
	}

	template<typename T, const size_t N>
	inline ArrayN<T, N> ArrayN<T, N>::operator*(const T c) const {
		ArrayN<T, N> ret;
		size_t n = N;
		for (size_t i = 0; i < n; i++) {
			ret[i] = data[i] * c;
		}
		return ret;
	}

	template<typename T, const size_t N>
	inline void ArrayN<T, N>::abs() {
		for (int i = 0; i < N; i++) {
			data[i] = std::abs(data[i]);
		}
	}

	template<typename T, const size_t N>
	inline ArrayN<T, N>& ArrayN<T, N>::operator+=(const ArrayN<T, N>& v) {
		for (size_t i = 0; i < N; i++) {
			(*this)[i] += v[i];
		}
		ArrayN& ret = (*this);
		return *this;
	}

	template<typename T, const size_t N>
	inline ArrayN<T, N>& ArrayN<T, N>::operator-=(const ArrayN<T, N>& v) {
		for (size_t i = 0; i < N; i++) {
			(*this)[i] -= v[i];
		}
		ArrayN& ret = (*this);
		return *this;
	}

	template<typename T, const size_t N>
	inline double ArrayN<T, N>::length() const {
		double m = (*this) * (*this);
		return std::sqrt(m);
	}

	template<typename T, const size_t N>
	inline ArrayN<T, N> ArrayN<T, N>::normalize() const {
		return (*this) * (1.0 / length());
	}

#pragma endregion

#pragma region cout

	template<typename T, const size_t N>
	inline std::ostream& operator<<(std::ostream& os, const ArrayN<T, N>& v) {
		os << "[";
		for (size_t i = 0; i < N; ++i) {
			if (i != 0) os << ", ";
			os << v[i];
		}
		os << "]";
		return os;
	}

#pragma endregion

}