#pragma once

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include<type_traits>

#ifdef DEBUG
#include <iostream>
#include <iomanip>
#endif // DEBUG

namespace MyVector {

	template <typename Data>
	class Vector {
	private:
		Data* _data = nullptr;
		size_t _size = 0;
		size_t _capacity = 0;

		Data* reloc(size_t, Data** = nullptr);

	public:
		typedef Data* iterator;
		typedef const Data* const_iterator;

		iterator begin() { return &_data[0]; }
		const_iterator begin() const { return &_data[0]; }
		iterator end() { return &_data[_size]; }
		const_iterator end() const { return &_data[_size]; }

		Vector();
		Vector(size_t);
		Vector(const Vector&);
		Vector(Vector&&);
		~Vector();

		bool push_back(Data);
		bool push_front(Data);
		bool pop_back();
		bool pop_front();

		Data& at(size_t);

		bool insert(iterator, Data);
		template <typename ... Datas>
		void insert(iterator, Data, Datas ...);

		bool erase(iterator, size_t = UINT32_MAX);
		void clear();
		bool empty();

		size_t size();
		size_t capacity();

		void shrink_to_fit();
		void resize(size_t);

		Vector& operator=(const Vector& other) {
			_size = 0;
			if (!this->reloc(other._capacity));
			else
				for (Data el : other)
					push_back(el);

			return *this;
		}
		Vector& operator=(Vector&& other) {
			if (_data)
				delete[] _data;

			_data		= other._data;
			_size		= other._size;
			_capacity	= other._capacity;

			other._data = nullptr;
			other._size = other._capacity = 0;
			
			return *this;
		}

#ifdef DEBUG
		void show_info();
#endif // DEBUG
	};


	template<typename Data>
	inline Data* Vector<Data>::reloc(size_t capacity, Data** it) {
		size_t cur = (*it) - begin();

		if (!capacity)
			return nullptr;

		Data* ptr;
		try {
			ptr = new Data[capacity];
		}
		catch (...) {
			return nullptr;
		}

		if (this->_data) {
			for (size_t i = 0; i < _size && i < capacity; i++)
				ptr[i] = _data[i];
			delete[] _data;
		}

		this->_capacity = capacity;
		_size			= _size < this->_capacity ? _size : _capacity;
		_data			= ptr;
		(*it)			= begin() + cur;

		return _data;
	}

	template<typename Data>
	inline Vector<Data>::Vector() : Vector(10) {}

	template<typename Data>
	inline Vector<Data>::Vector(size_t capacity) : _capacity(capacity), _size(0) { 
		try {
			_data = new Data[_capacity];
		}
		catch (...) {
			_data = nullptr;
		}
	}

	template<typename Data>
	inline Vector<Data>::Vector(const Vector& other) : Vector(other._capacity) {	
		if (_data) {
			int i = 0;
			for (Data t : other)
				_data[i++] = t;

			_size = other._size;
		}
	}

	template<typename Data>
	inline Vector<Data>::Vector(Vector&& other) {
		*this = std::move(other);
	}

	template<typename Data>
	inline Vector<Data>::~Vector() {
		if (_data)
			delete[] _data;
		_capacity = _size = 0;
	}

	template<typename Data>
	inline bool Vector<Data>::push_back(Data data) {
		if (_size == _capacity && !reloc(_capacity * 2))
			return false;

		_data[_size++] = data;
		return true;
	}

	template<typename Data>
	inline bool Vector<Data>::push_front(Data data) {
		if (_size == _capacity && !reloc(_capacity * 2))
			return false;

		iterator ptr = end();
		while (ptr != begin()) {
			*ptr = *(ptr - 1);
			--ptr;
		}

		*ptr = data;
		_size++;
		return true;
	}

	template<typename Data>
	inline bool Vector<Data>::pop_back() {
		if (_size < _capacity / 2 && reloc(_capacity / 2));
		return _size--;
	}

	template<typename Data>
	inline bool Vector<Data>::pop_front() {
		if (_size < _capacity / 2 && reloc(_capacity / 2));
		iterator ptr = begin();
		while (ptr != end()) {
			*ptr = *(ptr + 1);
			++ptr;
		}
		return _size--;
	}

	template<typename Data>
	inline Data& Vector<Data>::at(size_t index) {
		if (index > _size)
			throw 1; // out of range

		return _data[index];
	}

	template<typename Data>
	inline bool Vector<Data>::insert(iterator it, Data data) {
		if (!it || it < begin() || it > end() || _size == _capacity && !reloc(_capacity * 2, &it))
			return false;

		iterator ptr = end();
		while (ptr != it) {
			*ptr = *(ptr - 1);
			--ptr;
		}

		*it = data;
		_size++;
		return true;
	}

	template<typename Data>
	inline bool Vector<Data>::erase(iterator it, size_t n) {
		if (!it || !_size || it < begin() || it >= end() || n == 0)
			return false;

		iterator en = end();
		_size = it - begin();

		if (it + n < en) {
			iterator first = it + n;
			while (first != en) {
				*it = *first;
				it++;
				first++;
				_size++;
			}
		}

		return true;
	}

	template<typename Data>
	inline void Vector<Data>::clear() {
		_size = 0;
		reloc(10);
	}

	template<typename Data>
	inline bool Vector<Data>::empty() {
		return !_size;
	}

	template<typename Data>
	inline size_t Vector<Data>::size() {
		return _size;
	}

	template<typename Data>
	inline size_t Vector<Data>::capacity() {
		return _capacity;
	}

	template<typename Data>
	inline void Vector<Data>::shrink_to_fit() {
		reloc(_size);
	}

	template<typename Data>
	inline void Vector<Data>::resize(size_t n) {
		reloc(n);
	}

#ifdef DEBUG
	template<typename Data>
	inline void Vector<Data>::show_info() {
		std::cout << "MyVector info:\n" <<
			std::right << std::setw(20) << "Data addr: "	<< _data << '\n' <<
			std::right << std::setw(20) << "Size: "			<< _size << '\n' <<
			std::right << std::setw(20) << "Capacity: "		<< _capacity << "\n\n";

	}
#endif // DEBUG

	template<typename Data>
	template<typename ...Datas>
	inline void Vector<Data>::insert(iterator it, Data data, Datas ... rest) {
		if (_size == _capacity - 1)
			reloc(_capacity * 2, &it);
		insert(it++, data);
		insert(it, rest...);
	}

}

#endif // !__VECTOR_H__
