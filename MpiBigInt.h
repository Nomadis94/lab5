#pragma once
#include "bignum.h"
#include <iostream>
#include <string>
#include <time.h>
using namespace std;

class BigInteger
{
public:
	mpi _MPI;

	BigInteger();
	BigInteger(string NumberString);
	BigInteger(BigInteger const & BI);
	BigInteger(int Num);
	BigInteger(double Num);
	~BigInteger();
	// ������������� ������� ����� �������� keySize ���
	BigInteger GenPrime(int keySize, bool dhFlag=0);
	// �������� ����� � ������� E �� ������ N
	BigInteger PowAndMod(BigInteger const & E, BigInteger const & N);
	// �������� ����� � ������� -1 �� ������ N
	BigInteger InvMod(BigInteger const & N);
	// ����������� ����� ������ ��� ��������, � ������������ �� ��
	int BytesCount(int radix=10);

	// ���������� ������ � ������� �����
	string	ToString(int radix = 10);
	// ���������� "�����" ������, ���������� �� ������ �����
	string	ToRawString(int sizeInBytes);
	// ���������� ������ ���� �����
	char* ToByteArr();
	// ���������� ������ ���� �����, ����������� �� ������� ����
	char* ToByteArr(int sizeInBytes);

	// ���������������� BigInteger �� ������� ����
	BigInteger FromByteArr(char* ByteArr, int sizeInBytes);
	// ���������������� BigInteger �� "�����" ������
	BigInteger FromRawString(string RawString);

	BigInteger operator= (BigInteger const & BI);
	BigInteger operator= (int Num);
	BigInteger operator= (string NumbString);
	BigInteger operator+ (BigInteger const & BI);
	BigInteger &operator ++();
	BigInteger operator  ++(int);
	BigInteger operator- (BigInteger const & BI);
	BigInteger operator- (int Num);
	BigInteger operator* (BigInteger const & BI);
	BigInteger operator/ (BigInteger const & BI);
	BigInteger operator% (BigInteger const & BI);
	bool operator==(BigInteger const & BI);
	bool operator!=(BigInteger & BI);
	bool operator>(BigInteger const & BI);
	bool operator<(BigInteger const & BI);
	bool operator>=(BigInteger const & BI);
	bool operator<=(BigInteger const & BI);
private:
	void _swap(BigInteger &BI);
	static int _randFunc(void *rng_state, unsigned char *output, size_t len)
	{
		size_t i;
		if (rng_state != NULL)
			rng_state = NULL;
		for (i = 0; i < len; ++i)
			output[i] = rand();
		return(0);
	}
};
