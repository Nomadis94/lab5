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
	// ������ ����� � ����� (�� ���������)
	const int DEFAULT_KEY_SIZE = 512;
	// ���������� ���� ��� �������� 1 ������� (����������� MPI)
	// - ��. ������� mpi_write_binary � mpi_read_binary
	const int DELTA = 2;
	int _KeySize;
	char _FillChar;
	BigInteger _P, _Q, _N, _H, _D, _E;
	BigInteger MessageInt, EncInt, DecInt;
	BigInteger SignedInt;
	

};
