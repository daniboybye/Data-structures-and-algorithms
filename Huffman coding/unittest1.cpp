#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\SDP_PROJECT_V1\heap.hpp"
#include "..\SDP_PROJECT_V1\bitVector.hpp"
#include <random>

using namespace fmi::sdp::structures;
using namespace fmi::sdp::bitStructures;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(TestHeap)
		{
			std::default_random_engine generator;
			std::uniform_int_distribution<int> distribution(-100, 1000);

			std::vector<int*> vector;

			for (int i = 0; i < 10000; ++i)
				vector.push_back(new int(distribution(generator)));

			Heap<int*> heap(vector);
			for (int i = 0; i < 100; ++i)
				heap.push(new int(distribution(generator)));

			int *p1, *p2;

			Assert::IsTrue(heap.getSize() == 10100);

			p1 = heap.pop();

			while (heap.getSize()) {
				p2 = heap.pop();
				Assert::IsTrue(*p1 <= *p2);

				delete p1;
				p1 = p2;
			}
			delete p2;

			Assert::IsTrue(heap.isEmpty());
		}

		TEST_METHOD(TestBitVector) {
			BitVector myVector;
			std::vector<bool> vector;

			bool value;
			uint64_t count = 10000, i, pos;

			std::default_random_engine generator;
			std::uniform_int_distribution<unsigned> distribution(0, 1);
			std::uniform_int_distribution<unsigned> distributionPos(0, count - 1);


			for (i = 0; i < count; ++i) {
				value = distribution(generator);
				myVector.push_back(value);
				vector.push_back(value);
			}

			Assert::IsTrue(myVector.getSize() == count);

			for (i = 0; i < count; ++i)
				Assert::IsTrue(myVector.getElement(i) == vector[i]);


			for (i = 0; i < count; ++i) {
				pos = distributionPos(generator);
				value = distribution(generator);

				vector[pos] = value;
				myVector.modified(pos, value);

				Assert::IsTrue(myVector.getElement(pos) == vector[pos]);
			}

			for (i = 0; i < count; ++i)
				Assert::IsTrue(myVector.getElement(i) == vector[i]);

			i = myVector.getSize();
			myVector.append(vector.crbegin(), vector.crend(), vector.size());

			Assert::IsTrue(myVector.getSize() == i + vector.size());

			auto it = vector.crbegin();

			for (; i < myVector.getSize() && it != vector.crend(); ++i, ++it)
				Assert::IsTrue(myVector.getElement(i) == *it);

			Assert::IsTrue(i == myVector.getSize() && it == vector.crend());
		}
	};
}