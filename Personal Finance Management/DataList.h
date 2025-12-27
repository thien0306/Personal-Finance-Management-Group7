#pragma once
#include <iostream>
#include <stdexcept>

using namespace std;

template<class T>
class DataList {
private:
	T* items;
	int count;
	int capacity;
	void expand() {
		int newCapacity = capacity * 2;
		T* newItems = new T[newCapacity];
		for (int i = 0; i < count; i++) {
			newItems[i] = items[i];
		}
		delete[] items;
		items = newItems;
		capacity = newCapacity;
	}
public:
	DataList() {
		capacity = 10;
		count = 0;
		items = new T[capacity];
	}
	~DataList() {
		if (items != nullptr) {
			delete[] items;
		}
	}
	DataList(const DataList&) = delete;
	DataList& operator=(const DataList&) = delete;
	void add(T item) {
		if (count == capacity) {
			expand();
		}
		items[count] = item;
		count++;
	}
	void removeAt(int index) {
		if (index < 0 || index >= count) return;

		
		for (int i = index; i < count - 1; i++) {
			items[i] = items[i + 1];
		}
		count--;
	}
	T& get(int index) {
		if (index < 0 || index >= count) {
			throw out_of_range("Index out of bounds!");
		}
		return items[index];
	}
	T& operator[](int index) {
		return get(index);
	}
	int size() {
		return count;
	}
	void clear() {
		count = 0;
	}
};
