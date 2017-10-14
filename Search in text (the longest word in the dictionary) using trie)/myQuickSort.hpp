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
#ifndef __QUICK_SORT_HEADER_INCLUDED_
#define __QUICK_SORT_HEADER_INCLUDED_

#define underSizeInsertionSort 12

template <class T>
inline T middleElement(const T & first, const T & second, const T & third) {
	if (third>first){
		if (second > third)
			return third;

		if (second > first)
			return second;
		
		return first;
	}

	if (first<second)
		return first;

	if (second>third)
		return second;

	return third;
}

template <class T>
inline T middleElement(const T & first, const T & second, const T & third, bool(*p)(const T&, const T&)) {
	if (p(first,third)){
		if (p(third,second))
			return third;
		if (p(first,second))
			return second;
		else
			return first;
	}

	if (p(first,second))
		return first;

	if (p(third,second))
		return second;

	return third;
}

template<class T>
inline void quickSort(T * arr, size_t size) {
	if (size < underSizeInsertionSort){
		insertionSort(arr, size);
	}
	else {
		T pivot(middleElement(arr[0], arr[size / 2], arr[size - 1]));
		T *iArr = arr, *end = arr + size;
		size_t left = 0, right = size;

		do {
			if (*iArr< pivot) {
				std::swap(*iArr, arr[left++]);
			}
		} while (++iArr !=end);

		end = arr+left;
		while (iArr-- != end) {
			if (*iArr> pivot) {
				std::swap(*iArr, arr[--right]);
			}
		}

		if (left < size - right) {
			quickSort(arr, left);
			quickSort(arr + right, size - right);
		}
		else {
			quickSort(arr + right, size - right);
			quickSort(arr, left);
		}
	}
}

template<class T>
inline void quickSort(T * arr, size_t size, bool(*p)(const T&, const T&)) {
	if (size < underSizeInsertionSort){
		insertionSort(arr, size,p);
	}
	else {
		T pivot(middleElement(arr[0], arr[size / 2], arr[size - 1],p));
		T *iArr = arr, *end = arr + size;
		size_t left = 0, right = size;

		do {
			if (p(*iArr, pivot)) {
				std::swap(*iArr, arr[left++]);
			}
		} while (++iArr != end);

		end = arr+left;
		while (iArr-- !=end ) {
			if (p(pivot, *iArr)) {
				std::swap(*iArr, arr[--right]);
			}
		}

		if (left < size - right) {
			quickSort(arr, left);
			quickSort(arr + right, size - right);
		}
		else {
			quickSort(arr + right, size - right);
			quickSort(arr, left);
		}
	}
}

template<class T>
inline void quickSortIter(T * arr, size_t size) {
		size_t left, right;
		T pivot,*iArr,*end;


		while (true) {
			if (size < underSizeInsertionSort) {
				insertionSort(arr, size);
				return;
			}

			pivot = middleElement(arr[0], arr[size / 2], arr[size - 1]);
			left = 0;
			iArr = arr;
			end = arr + size;
			do  {
				if (*iArr< pivot) {
					std::swap(*iArr, arr[left++]);
				}
			} while (++iArr != end);

			right = size;
			end = arr + left;
			while (iArr-- != end) {
				if (*iArr> pivot) {
					std::swap(*iArr, arr[--right]);
				}
			}

			if (left < size - right) {
				quickSortIter(arr, left);
				arr += right;
				size -= right;
			}
			else {
				quickSortIter(arr + right, size - right);
				size = left;
			}
		}
}

template<class T>
inline void quickSortIterP(T * beg,T * end) {
	size_t size = end - beg;
	T pivot, *iArr,*left,*right;


	while (true) {
		if (size < underSizeInsertionSort) {
			insertionSort(beg, size);
			return;
		}

		pivot = middleElement(*beg, beg[size / 2], *(end-1));

		iArr=left = beg;
		do  {
			if (*iArr< pivot) {
				std::swap(*iArr, *left);
				++left;
			}
		} while (++iArr != end);

		right = end;
		while (iArr-- != left) {
			if (*iArr> pivot) {
				--right;
				std::swap(*iArr, *right);
			}
		}

		if (left+size > right) {
			quickSortIterP(beg, left);
			beg = right;
			size = end-right;
		}
		else {
			quickSortIterP(right, end);
			size = right-beg;
		}
	}
}

#endif //__QUICK _SORT_HEADER_INCLUDED_