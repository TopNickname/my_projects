#pragma once
#include <string>
#include <initializer_list>
#include <cmath>
#include <ostream>
#include <algorithm> // :(
#include <iostream>
//#include "ArrayN.h";

namespace std_plus {

	template<typename T>
	class VectorN {
	private:
		size_t vsize = 0, memory_size = 0;
		T* mem;
		bool is_string = false;
		void init(size_t n, const T& el=T()) ;
		void mem_resize(size_t new_size);
		
		class iterator {
			T* data;
			size_t i;
		public:
			iterator(T* _data, size_t _i = 0) : data(_data), i(_i) {};
			iterator& operator++();
			iterator& operator--();
			bool operator==(iterator x);
			bool operator!=(iterator x);
			const T& operator*() const;
			T& operator*();
		};

	public:

		VectorN(size_t n, const T& el);
		VectorN(size_t n);

		VectorN();
		VectorN(const VectorN& v);
		VectorN(VectorN&& v);
		VectorN(std::initializer_list<T> list);

		template<size_t N>
		VectorN(const T (&arr)[N]);
		//template<size_t N>
		//VectorN(const std_plus::ArrayN<T,N> &arr);
		~VectorN();

		//std::ostream& operator<<(std::ostream& os, const VectorN<T>& v);

		VectorN<T>& operator=(VectorN<T> v);
		T& operator[](size_t i);
		const T& operator[](size_t i) const;
		void swap(VectorN<T>& v) noexcept;
		void push_back(const T& el);
		T pop_back();

		long long rfind(const T& el) const;
		long long find(const T& el) const;
		size_t count(const T& el) const;
		long long capacity() const noexcept;
		void shrink_to_fit() noexcept;
		T front() const;
		T back() const;
		size_t size() const noexcept;
		void resize(size_t n);
		iterator begin() const noexcept;
		iterator end() const noexcept;
		T* data() const noexcept;

		void erase(size_t s, size_t d = 1);
		void insert(size_t i, const T& el);
		bool empty() const noexcept;
		void clear();

		bool operator==(const VectorN<T>& v) const;
		bool operator<(const VectorN<T>& v) const;
		bool operator>(const VectorN<T>& v) const;
		bool operator<=(const VectorN<T>& v) const;
		bool operator>=(const VectorN<T>& v) const;
		bool operator!=(const VectorN<T>& v) const;
		

#pragma region math
		VectorN operator+(const VectorN<T>& v) const;
		VectorN operator-(const VectorN<T>& v) const;
		VectorN& operator+=(const VectorN<T>& v);
		VectorN& operator-=(const VectorN<T>& v);
		T operator*(const VectorN<T>& v) const;
		double norm() const;
		void abs();
#pragma endregion
	};

#pragma region memory

	template<typename T>
	inline T* VectorN<T>::data() const noexcept {
		return mem;
	}

	template<typename T>
	inline void VectorN<T>::clear() {
		vsize = 0;
	}

	template<typename T>
	inline void VectorN<T>::init(size_t n, const T& el) {
		vsize = n;
		memory_size = n;
		mem = new T[n];
		for (size_t i = 0; i < vsize; i++) {
			mem[i] = el;
		}
	}

	template<typename T>
	inline void VectorN<T>::resize(size_t n) {
		if (n > memory_size) {
			this->mem_resize(n);
			vsize = n;
		}
		else {
			vsize = n;
		}
	}

	template<typename T>
	inline void VectorN<T>::mem_resize(size_t new_size) {
		T* new_mem = new T[new_size];
		if (mem != nullptr) {
			for (size_t i = 0; i < memory_size; ++i) {
				new_mem[i] = mem[i];
			}
		}
		delete[] mem;
		mem = new_mem;
		memory_size = new_size;
	}

	template<typename T>
	inline void VectorN<T>::shrink_to_fit() noexcept {
		mem_resize(vsize);
	}

	template<typename T>
	inline long long VectorN<T>::capacity() const noexcept {
		return memory_size;
	}

#pragma endregion

#pragma region constructors_and_assignment

	template<typename T>
	inline VectorN<T>& VectorN<T>::operator=(VectorN<T> v)  {
		swap(v);
		return *this;
	}

	template<typename T>
	inline VectorN<T>::VectorN(size_t n, const T& el)  {
		init(n, el);
	}

	template<typename T>
	inline VectorN<T>::VectorN(size_t n)  {
		init(n);
	}

	template<typename T>
	inline VectorN<T>::VectorN()  {
		init(0);
	}

	template<typename T>
	template<size_t N>
	inline VectorN<T>::VectorN(const T(&arr)[N])  {
		init(N);
		for (size_t i = 0; i < N; i++) {
			mem[i] = arr[i];
		}
	}
	/*
	template<typename T>
	template<size_t N>
	inline VectorN<T>::VectorN(const std_plus::ArrayN<T,N> &arr) {
		init(N);
		for (size_t i = 0; i < N; i++) {
			mem[i] = arr[i];
		}
	}
	*/

	template<typename T>
	inline VectorN<T>::VectorN(std::initializer_list<T> list)  {
		size_t n = list.size();
		init(n);
		std::copy(list.begin(), list.end(), mem);
	}

	template<typename T>
	inline VectorN<T>::VectorN(const VectorN<T>& v)  {
		init(v.size());
		for (size_t i = 0; i < vsize; i++) {
			(*this)[i] = v[i];
		}
	}

	template<typename T>
	inline VectorN<T>::VectorN(VectorN<T>&& v)  
		: vsize(v.vsize), memory_size(v.memory_size), mem(v.mem) {
		v.mem = nullptr;
		v.vsize = 0;
		v.memory_size = 0;
	}

	template<typename T>
	inline VectorN<T>::~VectorN()  {
		delete[] mem;
	}

#pragma endregion

#pragma region front-back

	template<typename T>
	inline T VectorN<T>::front() const {
		if (vsize <= 0) {
			throw "out of range";
		}
		return mem[0];
	}

	template<typename T>
	inline T VectorN<T>::back() const {
		if (vsize <= 0) {
			throw "out of range";
		}
		return mem[vsize - 1];
	}

	template<typename T>
	inline typename VectorN<T>::iterator VectorN<T>::begin() const noexcept {
		return iterator(mem,0);
	}

	template<typename T>
	inline typename VectorN<T>::iterator VectorN<T>::end() const noexcept {
		return iterator(mem, vsize);
	}

#pragma endregion

#pragma region push-pop-back

	template<typename T>
	inline void VectorN<T>::push_back(const T& el) {
		if (vsize < memory_size) {
			vsize++;
			mem[(vsize)-1] = el;
		}
		else {
			if (memory_size == 0) {
				mem_resize(1);
			}
			else {
				mem_resize(memory_size * 2);
			}
			vsize++;
			mem[vsize - 1] = el;
		}
	}

	template<typename T>
	inline T VectorN<T>::pop_back() {
		if (vsize == 0) {
			throw "out of range";
		}
		vsize--;
		T tmp = mem[vsize];
		return tmp;
	}

	template<typename T>
	inline size_t VectorN<T>::size() const noexcept {
		return vsize;
	}

#pragma endregion

#pragma region operator[]
	template<typename T>
	inline T& VectorN<T>::operator[](size_t i) {
		if (i >= vsize) {
			throw "out of range";
		}
		return mem[i];
	}

	template<typename T>
	inline const T& VectorN<T>::operator[](size_t i) const {
		if (i >= vsize) {
			throw "out of range";
		}
		return mem[i];
	}
#pragma endregion

#pragma region algorithm
	
	template<typename T>
	inline void VectorN<T>::swap(VectorN<T>& v) noexcept {
		T* mem_tmp = v.mem;
		size_t memory_size_tmp = v.memory_size;
		size_t vsize_tmp = v.vsize;
		v.mem = mem;
		v.vsize = vsize;
		v.memory_size = memory_size;
		mem = mem_tmp;
		vsize = vsize_tmp;
		memory_size = memory_size_tmp;
	}
	
	template<typename T>
	inline void VectorN<T>::erase(size_t s, size_t d) {
		for (size_t i = s; i < vsize - d; i++) {
			mem[i] = mem[i + d];
		}
		resize(vsize - d);
	}

	template<typename T>
	inline long long VectorN<T>::find(const T& obj) const {
		size_t n = vsize;
		long long ret = -1;
		for (size_t i = 0; i < n; i++) {
			if (mem[i] == obj) {
				ret = i;
				break;
			}
		}
		return ret;
	}

	template<typename T>
	inline long long VectorN<T>::rfind(const T& obj) const {
		size_t n = vsize;
		long long ret = -1;
		for (size_t i = n; i != 0; i--) {
			if (mem[i-1] == obj) {
				ret = i-1;
				break;
			}
		}
		return ret;
	}

	template<typename T>
	inline size_t VectorN<T>::count(const T& el) const {
		size_t ret = 0;
		for (size_t i = 0; i < vsize; i++) {
			ret += (int)(mem[i] == el);
		}
		return ret;
	}

	template<typename T>
	inline void VectorN<T>::insert(size_t i, const T& el) {
		if (i>vsize) {
			throw "out of range";
		}
		else if (i==vsize){
			push_back(el);
			return;
		}
		push_back(this->back());
		for (size_t j = vsize - 2; j > i; j--) {
			mem[j] = mem[j - 1];
		}
		mem[i] = el;
	}

	template<typename T>
	inline bool VectorN<T>::empty() const noexcept {
		return vsize == 0;
	}

#pragma endregion

#pragma region bool

	template<typename T>
	inline bool VectorN<T>::operator==(const VectorN<T>& v) const {
		if (vsize != v.vsize) {
			return false;
		}
		else {
			bool ret = true;
			for (size_t i = 0; i < vsize; i++) {
				if (mem[i] != v[i]) {
					ret = false;
					break;
				}
			}
			return ret;
		}
	}

	template<typename T>
	inline bool VectorN<T>::operator!=(const VectorN<T>& v) const {
		return !(*this == v);
	}

	template<typename T>
	inline bool VectorN<T>::operator<(const VectorN<T>& v) const {
		size_t m = std::min(v.vsize, vsize);
		int flag = 0;
		for (size_t i = 0; i < m; i++) {
			if (mem[i] == v[i]) {
				continue;
			}
			if (mem[i] > v[i]) {
				flag = -1;
				break;
			}
			if (mem[i] < v[i]) {
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			return vsize < v.vsize;
		}
		else {
			return (flag == 1);
		}
	}

	template<typename T>
	inline bool VectorN<T>::operator>(const VectorN<T>& v) const {
		size_t m = std::min(v.vsize, vsize);
		int flag = 0;
		for (size_t i = 0; i < m; i++) {
			if (mem[i] == v[i]) {
				continue;
			}
			if (mem[i] < v[i]) {
				flag = -1;
				break;
			}
			if (mem[i] > v[i]) {
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			return vsize > v.vsize;
		}
		else {
			return (flag == 1);
		}
	}

	template<typename T>
	inline bool VectorN<T>::operator>=(const VectorN<T>& v) const {
		return !(*this < v);
	}

	template<typename T>
	inline bool VectorN<T>::operator<=(const VectorN<T>& v) const {
		return !(*this > v);
	}

#pragma endregion

#pragma region math

	template<typename T>
	inline VectorN<T> VectorN<T>::operator+(const VectorN<T> &v) const {
		size_t n = vsize;
		if (n != v.size()) {
			throw "math error";
		}
		VectorN<T> New_vec(n);
		for (size_t i = 0; i < n; i++) {
			New_vec[i] = (*this)[i] + v[i];
		}
		return New_vec;
	}

	template<typename T>
	inline VectorN<T> VectorN<T>::operator-(const VectorN<T> &v) const {
		size_t n = vsize;
		if (n != v.size()) {
			throw "math error";
		}
		VectorN<T> New_vec(n);
		for (size_t i = 0; i < n; i++) {
			New_vec[i] = (*this)[i] - v[i];
		}
		return New_vec;
	}

	template<typename T>
	inline T VectorN<T>::operator*(const VectorN<T> &v) const {
		if (vsize != v.vsize) {
			throw "math error";
		}
		T res = 0;
		size_t n = vsize;
		for (size_t i = 0; i < n; i++) {
			res += mem[i] * v[i];
		}
		return res;
	}

	template<typename T>
	inline void VectorN<T>::abs() {
		for (int i = 0; i < vsize; i++) {
			mem[i] = std::abs(mem[i]);
		}
	}

	template<typename T>
	inline VectorN<T>& VectorN<T>::operator+=(const VectorN<T>& v) {
		if (vsize != v.vsize) {
			throw "math error";
		}
		for (size_t i = 0; i < vsize; i++) {
			(*this)[i] += v[i];
		}
		VectorN& ret = (*this);
		return *this;
	}

	template<typename T>
	inline VectorN<T>& VectorN<T>::operator-=(const VectorN<T>& v) {
		if (vsize != v.vsize) {
			throw "math error";
		}
		for (size_t i = 0; i < vsize; i++) {
			(*this)[i] -= v[i];
		}
		VectorN& ret = (*this);
		return *this;
	}

	template<typename T>
	inline double VectorN<T>::norm() const {
		double m = (*this) * (*this);
		return std::sqrt(m);
	}

#pragma endregion

#pragma region cout

	template<typename T>
	inline std::ostream& operator<<(std::ostream& os, const VectorN<T>& v) {
		os << "[";
		for (size_t i = 0; i < v.size(); ++i) {
			if (i != 0) os << ", ";
			os << v[i];
		}
		os << "]";
		return os;
	}


#pragma endregion

#pragma region iterator

	
	template<typename T>
	inline typename VectorN<T>::iterator& VectorN<T>::iterator::operator++() {
		this->i++;
		return *this;
	}

	template<typename T>
	inline typename VectorN<T>::iterator& VectorN<T>::iterator::operator--() {
		this->i--;
		return *this;
	}

	template<typename T>
	inline T& VectorN<T>::iterator::operator*() {
		return *(this->data + this->i);
	}

	template<typename T>
	inline const T& VectorN<T>::iterator::operator*() const {
		return *(this->data + this->i);
	}

	template<typename T>
	inline bool VectorN<T>::iterator::operator==(iterator x) {
		return (x.data == this->data && this->i == x.i);
	}

	template<typename T>
	inline bool VectorN<T>::iterator::operator!=(iterator x) {
		return !(x == *(this));
	}
	
#pragma endregion
}