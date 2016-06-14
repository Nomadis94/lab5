#pragma once
#include "MpiBigInt.h"

class RSACrypter
{
public:
	RSACrypter();
	RSACrypter(int KeySize);
	RSACrypter(int KeySize, char FillChar);
	~RSACrypter();
	string Encrypt(string M);
	string Decryt(string C);
	string Sign(string M);
	bool Verify(string M, string S);

private:
	// Размер ключа в битах (по умолчанию)
	const int DEFAULT_KEY_SIZE = 512;
	// Количество байт для хранения 1 символа (особенности MPI)
	// - см. функции mpi_write_binary и mpi_read_binary
	const int DELTA = 2;
	int _KeySize;
	char _FillChar;
	BigInteger _P, _Q, _N, _H, _D, _E;
	BigInteger MessageInt, EncInt, DecInt;
	BigInteger SignedInt;
	

};
