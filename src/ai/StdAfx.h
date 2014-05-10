#ifndef __STDAFX_H__
#define __STDAFX_H__
#pragma once

#include <cstdlib>
#include <vector>
#include <cassert>
#include <ctime>
using namespace std;

#define CopyMemory         memcpy
#define INT_PTR            int
#define BYTE               unsigned char
#define ASSERT             assert
#define GAME_PLAYER        3
#define WORD               unsigned short
#define LONG               long
#define CountArray(a)      (sizeof(a)/sizeof(a[0]))

void ZeroMemory(void * data, int n);

int GetTickCount();

template<class T>
class CArrayTemplate : public vector<T>
{
	public:
		void RemoveAll()
		{
			this->clear();
		}
		int GetCount()
		{
			return this->size();
		}
		void RemoveAt(int n)
		{
			this->erase(this->begin() + n);
		}
		void InsertAt(int n, T & val)
		{
			this->insert(this->begin() + n, val);
		}
		T & GetAt(int n)
		{
			return this->at(n);
		}
		void Add(T & val)
		{
			this->push_back(val);
		}
		bool IsEmpty()
		{
			return this->empty();
		}
};
#endif
