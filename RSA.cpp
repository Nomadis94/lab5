#include "RSA.h"

RSACrypter::RSACrypter()
{
	_P = _P.GenPrime(_KeySize);
	_Q = _Q.GenPrime(_KeySize);
	_N = _P*_Q;
	_H = (_P - 1)*(_Q - 1);
	_E = _E.GenPrime(_KeySize);
	_D = _E.InvMod(_H);
	_KeySize = DEFAULT_KEY_SIZE;
	_FillChar = '\0';
}

RSACrypter::RSACrypter(int KeySize)
{
	if (KeySize < 8) KeySize = 8;
	_P = _P.GenPrime(KeySize);
	_Q = _Q.GenPrime(KeySize);
	_N = _P*_Q;
	_H = (_P - 1)*(_Q - 1);
	_E = _E.GenPrime(KeySize);
	_D = _E.InvMod(_H);
	_KeySize = KeySize;
	_FillChar = '\0';
}

RSACrypter::RSACrypter(int KeySize, char FillChar)
{
	if (KeySize < 8) KeySize = 8;
	_P = _P.GenPrime(KeySize);
	_Q = _Q.GenPrime(KeySize);
	_N = _P*_Q;
	_H = (_P - 1)*(_Q - 1);
	_E = _E.GenPrime(KeySize);
	_D = _E.InvMod(_H);
	_KeySize = KeySize;
	_FillChar = FillChar;
}

RSACrypter::~RSACrypter()
{
}

string RSACrypter::Encrypt(string M)
{
	int BytesInBlock = _KeySize / 8;
	while (M.length() % BytesInBlock != 0)
	{
		M = M + _FillChar;
	}
	string Res = "";
	string Enc = "";
	string Block;
	BigInteger EncInt;
	BigInteger MessageInt;
	int BlocksCount = M.length() / BytesInBlock;

	for (int i = 0; i < BlocksCount; i++) {
		Block = string(M, i*BytesInBlock,BytesInBlock);
		MessageInt = MessageInt.FromRawString(Block);
		EncInt = MessageInt.PowAndMod(_E, _N);
		Enc = EncInt.ToRawString(BytesInBlock*DELTA);
		Res = Res + Enc;
	}
	return Res;
}

string RSACrypter::Decryt(string C)
{
	int BytesInBlock = _KeySize / 8;
	if (C.length() % BytesInBlock != 0)
		throw exception("Bad input data: wrong size of chipertext");
	int BlocksCount = C.length() / BytesInBlock / DELTA;
	string Res = "";
	string Dec = "";
	string Block;
	BigInteger DecInt;
	BigInteger ChiperInt;

	for (int i = 0; i < BlocksCount; i++) {
		Block = string(C, i*BytesInBlock*DELTA, BytesInBlock*DELTA);
		ChiperInt = ChiperInt.FromRawString(Block);
		DecInt = ChiperInt.PowAndMod(_D, _N);

		Dec = DecInt.ToRawString(BytesInBlock);
		Res = Res + Dec;
	}
	return Res;
}

string RSACrypter::Sign(string M)
{
	int BytesInBlock = _KeySize / 8;
	while (M.length() % BytesInBlock != 0)
	{
		M = M + _FillChar;
	}
	string Res = "";
	string Signed = "";
	string Block;
	BigInteger SignedInt;
	BigInteger MessageInt;
	int BlocksCount = M.length() / BytesInBlock;

	for (int i = 0; i < BlocksCount; i++) {
		Block = string(M, i*BytesInBlock, BytesInBlock);
		MessageInt = MessageInt.FromRawString(Block);
		SignedInt = MessageInt.PowAndMod(_D, _N);
		Signed = SignedInt.ToRawString(BytesInBlock*DELTA);
		Res = Res + Signed;
	}
	return Res;
}

bool RSACrypter::Verify(string M, string S)
{
	bool IsValid = false;
	int BytesInBlock = _KeySize / 8;
	while (M.length() % BytesInBlock != 0)
	{
		M = M + _FillChar;
	}
	if (S.length() % BytesInBlock != 0)
		throw exception("Bad input data: wrong size of chipertext");

	int BlocksCount = S.length() / BytesInBlock / DELTA;
	string ResM = "";		//M'
	string ResBlock = "";	//блок M'
	string MesBlock;		//блок М
	BigInteger ResInt;
	BigInteger SignedInt;

	for (int i = 0; i < BlocksCount; i++) {
		MesBlock = string(S, i*BytesInBlock*DELTA, BytesInBlock*DELTA);
		SignedInt = SignedInt.FromRawString(MesBlock);
		
		ResInt = SignedInt.PowAndMod(_E, _N);

		ResBlock = ResInt.ToRawString(BytesInBlock);
		ResM = ResM + ResBlock;
	}
	if (ResM == M) IsValid = true;
	return IsValid;
}
