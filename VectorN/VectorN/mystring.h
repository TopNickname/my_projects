#pragma once
#include "VectorN.h"

namespace std_plus {

	class mystring {
		VectorN<char> data_;
		using iterator = decltype(data_.begin());
	public:
		mystring(size_t n, const char& el) { 
			data_ = VectorN<char>(n, el);
			data_.push_back('\0');
		}
		mystring(size_t n) { 
			data_ = VectorN<char>(n); 
			data_.push_back('\0');
		}

		mystring() { 
			data_ = VectorN<char>();
			data_.push_back('\0');
		}
		mystring(std::initializer_list<char> list) { 
			data_ = VectorN<char>(list);
			data_.push_back('\0');
		}


		template<size_t N>
		mystring(const char(&arr)[N]) { 
			data_ = VectorN<char>(arr);
			data_.push_back('\0');
		}

		char& operator[](size_t i) { return data_[i]; }
		const char operator[](size_t i) const {
			return data_[i];
		}
		void push_back(const char& el) { 
			data_[data_.size() - 1] = el;
			data_.push_back('\0'); }
		char pop_back() { 
			data_.pop_back();
			char tmp=data_.pop_back();
			data_.push_back('\0');
			return tmp;
		}

		long long rfind(const char& el) const { return data_.rfind(el); }
		long long find(const char& el) const { return data_.find(el); }
		size_t count(const char& el) const { return data_.count(el); }
		long long capacity() const noexcept { return data_.capacity(); }
		void shrink_to_fit() noexcept { data_.shrink_to_fit(); }
		char front() const { return data_.front(); }
		char back() const { return data_[data_.size()-2]; }
		size_t size() const noexcept { return data_.size()-1; }
		void resize(size_t n) { data_.resize(n); }
		iterator begin() const noexcept { return data_.begin(); }
		iterator end() const noexcept { return data_.end(); }
		char* data() const noexcept { 
			return data_.data();
		}

		//void erase(size_t s, size_t d = 1) { data_.erase(s, d); }
		//void insert(size_t i, const char& el) { data_.insert(i, el); }
		//bool empty() const noexcept { return data_.empty(); }
		//void clear() { data_.clear(); }

		bool operator==(const mystring& v) const { return data_.operator==(v.data_); }
		bool operator<(const mystring& v) const { return data_.operator<(v.data_); }
		bool operator>(const mystring& v) const { return data_.operator>(v.data_); }
		bool operator<=(const mystring& v) const { return data_.operator<=(v.data_); }
		bool operator>=(const mystring& v) const { return data_.operator>=(v.data_); }
		bool operator!=(const mystring& v) const { return data_.operator!=(v.data_); }

		mystring& operator+=(const mystring& v) {
			for (size_t i = 0; i < v.size(); i++) {
				(*this).push_back(v[i]);
			}
			return *this;
		}

		mystring operator+(const mystring& v) const {
			mystring New_str(*this);
			for (size_t i = 0; i < v.size(); i++) {
				New_str.push_back(v[i]);
			}
			return New_str;
		}

	};

	inline std::ostream& operator<<(std::ostream& os, const mystring& v) {
		for (size_t i = 0; i < v.size(); ++i) {
			os << v[i];
		}
		os << '\0';
		return os;
	}
}