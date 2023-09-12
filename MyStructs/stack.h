#pragma once

#ifndef __MyStack_H__
#define __MyStack_H__

#include<type_traits>

namespace MyStack {
	typedef bool err_flag;
	typedef bool war_flag;

	template <typename Data>
	class Stack {
	private:
		Data* data = nullptr;
		size_t capacity = 0, count = 0;

		Data* reloc(size_t capacity) {
			Data* ptr = nullptr;

			if (capacity == UINT32_MAX)
				cap_limit = true;

			try {
				ptr = new Data[capacity];
			}
			catch (...) {
				bad_alloc = true;
				return nullptr;
			}

			if (this->data) {
				for (size_t i = 0; i < count && i < capacity; i++)
					ptr[i] = data[i];
				delete data;
			}

			this->capacity = capacity;
			count = count < this->capacity ? count : capacity;
			data = ptr;

			return data;
		}
		void free() {
			if (data)
				delete data;
			capacity = count = 0;
		}

	public:
		err_flag bad_alloc = 0; // bad allocate memory
		war_flag cap_limit = 0; // capacity near or more then limit UINT32_MAX

		Stack(size_t capacity) : capacity(capacity) {
			if (!reloc(this->capacity))
				free();
			else
				err_clear();
		}
		Stack() : Stack(10) {}
		~Stack() {
			free();
		}

		Stack(const Stack& other) {
			if (!reloc(other.capacity)) {
				free();
			}
			else {
				err_clear();

				count = 0;
				for (size_t i = 0; i < other.count && i < this->capacity; i++)
					this->push(other.data[i]);
			}
		}
		Stack(Stack&& other) {
			*this = std::move(other);
		}

		void err_clear() {
			bad_alloc = false;
		}
		bool push(Data data) {
			if (count == capacity && !reloc(capacity * 2))
				return false;

			this->data[count++] = data;
			return true;
		}
		bool pop() {
			if (capacity > 10 && count < capacity / 2)
				reloc(capacity / 2);

			return count ? count-- : count;
		}
		Data& top() {
			return data[count - 1];
		}

		size_t size() {
			return count;
		}

		Stack& operator=(const Stack& other) {
			delete data;

			if (!this->reloc(other.capacity)) {
				this->free();
				return *this;
			}
			else
				err_clear();

			count = 0;
			for (size_t i = 0; i < other.count && i < this->capacity; i++)
				this->push(other.data[i]);

			return *this;
		}
		Stack& operator=(Stack&& other) {
			if (data)
				delete data;

			data = other.data;
			count = other.count;
			capacity = other.capacity;

			other.data = nullptr;
			other.count = other.capacity = 0;

			return *this;
		}
	};

}

#endif // !__MyStack_H__