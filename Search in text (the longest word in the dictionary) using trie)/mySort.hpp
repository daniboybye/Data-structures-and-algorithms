/**
*
* Solution to homework task
* Data Structures Course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2016/2017
*
* @author Daniel Urumov
* @idnumber 45135
* @task 3
* @compiler VC
*
*/
#ifndef _SORT_HEADER_INCLUDED_
#define _SORT_HEADER_INCLUDED_

template <class T>
inline void selectSort(T *arr, size_t size, bool(*p)(const T&, const T&)) {
	for (size_t i = 0, minIndex, ii; i < size - 1; ++i) {
		minIndex = i;
		for (ii = i + 1; ii < size; ++ii) {
			if (p(arr[ii], arr[minIndex])) minIndex = ii;
		}
		swapif(arr[minIndex], arr[i], p);
	}
}

template<class T>
inline void insertionSort(T *arr, size_t size, bool(*p)(const T&, const T&)) {
	if (size<2) return;

	size_t i = size, ii;
	T element;

	while (--i > 0)
		swapif(arr[i], arr[i - 1], p);

	for (i = 2; i<size; ++i) {
		element = arr[i];

		ii = i - 1;
		for (; p(element, arr[ii]); --ii) {
			arr[ii + 1] = arr[ii];
		}
		arr[ii + 1] = element;
	}
}

template<class T>
inline void insertionSort(T *arr, size_t size) {
	if (size<2) return;

	size_t i = size, ii;
	T element;

	while (--i)
		swapif(arr[i], arr[i - 1]);

	for (i = 2; i<size; ++i) {
		element = arr[i];

		ii = i - 1;
		while(element< arr[ii]) {
			arr[ii + 1] = arr[ii];
			--ii;
		}
		arr[ii + 1] = element;
	}
}

template <class T>
inline void merge(T *arr1, size_t size1, T *arr2, size_t size2, T *mergeArr, bool(*p)(const T&, const T&)) {
	size_t id1 = 0, id2 = 0;
	while (id1 < size1 && id2 < size2) {
		if (p(arr2[id2], arr1[id1])) {
			mergeArr[id1 + id2] = arr2[id2];
			++id2;
		}

		else {
			mergeArr[id1 + id2] = arr1[id1];
			++id1;
		}
	}

	while (id1 < size1) mergeArr[id1 + id2] = arr1[id1], ++id1;
	while (id2 < size2) mergeArr[id1 + id2] = arr2[id2], ++id2;
}

template <class T>
inline void mergeSort(T *arr, size_t size, T *helpArr, bool(*p)(const T&, const T&)) {
	if (size >= 10) {
		mergeSort(arr, size / 2, helpArr, p);
		mergeSort(arr + size / 2, size - size / 2, helpArr + size / 2, p);
		merge(arr, size / 2, arr + size / 2, size - size / 2, helpArr, p);
		memcpy(arr, helpArr, sizeof(arr[0])*size);

		return;
	}

	insertionSort(arr, size, p);
}

#endif //_SORT_HEADER_INCLUDED_