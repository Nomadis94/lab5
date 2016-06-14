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
	// Сгенерировать простое число размером keySize бит
	BigInteger GenPrime(int keySize, bool dhFlag=0);
	// Возвести число в степень E по модулю N
	BigInteger PowAndMod(BigInteger const & E, BigInteger const & N);
	// Возвести число в степень -1 по модулю N
	BigInteger InvMod(BigInteger const & N);
	// Необходимая длина буфера для хранения, в зависимотсти от СС
	int BytesCount(int radix=10);

	// Возвращает строку с цифрами числа
	string	ToString(int radix = 10);
	// Возвращает "сырую" строку, полученную из байтов числа
	string	ToRawString(int sizeInBytes);
	// Возвращает массив байт числа
	char* ToByteArr();
	// Возвращает массив байт числа, выравненный по правому краю
	char* ToByteArr(int sizeInBytes);

	// Инициализировать BigInteger из массива байт
	BigInteger FromByteArr(char* ByteArr, int sizeInBytes);
	// Инициализировать BigInteger из "сырой" строки
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
