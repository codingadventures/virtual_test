// virtual_test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <chrono>

using namespace std;

#define USE_POLYMORPHISM 0

#if USE_POLYMORPHISM
class TestBase
{
public:
	TestBase() = default;
	TestBase(const TestBase& other) = default;
	
	virtual ~TestBase() {}
	

	virtual int Func(int x) = 0;
	virtual int Func1(int x) = 0;
	virtual int Func2(int x) = 0;
};

class TestImpl : public TestBase
{
public:
	int Func(int x) override
	{
		return x * x;
	}

	int Func1(int x) override
	{
		return x * x * x;
	}

	int Func2(int x) override
	{
		return x * x - x * 2;
	}
};

#else

class TestBase
{
public:
	int Func(int x);
	int Func1(int x);
	int Func2(int x);
};

class TestImpl : public TestBase
{
public:
	__forceinline int FuncImpl(int x)
	{
		return x * x;
	}

	__forceinline int FuncImpl1(int x)
	{
		return x * x * x;
	}

	__forceinline int FuncImpl2(int x)
	{
		return x * x - x * 2;
	}
};

__forceinline int TestBase::Func(int x)
{
	return ((TestImpl*)this)->FuncImpl(x);
}

__forceinline int TestBase::Func1(int x)
{
	return ((TestImpl*)this)->FuncImpl1(x);
}

__forceinline int TestBase::Func2(int x)
{
	return ((TestImpl*)this)->FuncImpl2(x);
}

#endif // USE_POLYMORPHISM

#define ITER_COUNT 100

int main()
{
#if USE_POLYMORPHISM
	TestBase* test = new TestImpl();
#else
	TestImpl* test = new TestImpl();
#endif 

	uint64_t accum = 0ull;

	float elapsedAvg = 0.0f;
	for (int iterIdx = 0; iterIdx < ITER_COUNT; ++iterIdx)
	{
		auto start = chrono::high_resolution_clock::now();

		accum = 0ull;
		for (uint64_t i = 0; i < 1000000; i++)
		{
			accum += test->Func(i);
			accum += test->Func1(i);
			accum += test->Func2(i);
		}

		auto end = chrono::high_resolution_clock::now();

		using dura = std::chrono::duration<float, std::chrono::milliseconds::period>;
		auto elapsed = chrono::duration_cast<dura>(end - start).count();

		elapsedAvg += elapsed;
	}

	elapsedAvg = elapsedAvg / (float)ITER_COUNT;
	std::cout << elapsedAvg << std::endl;

	delete test;

	return (int)accum;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
