#include <iostream>
#include "bignum.h"
#include "MpiBigInt.h"
#include "RSA.h"

using namespace std;
int BreakBit(unsigned char* src, int iBit = 0)
{

	int ByteIndex = iBit / 8;
	int BitIndex = iBit % 8;
	src[ByteIndex] = src[ByteIndex] ^ (1 << BitIndex);
	return 0;
}
string BreakBit(string InpStr, int iBit = 0) 
{
	int MesLen = InpStr.length();
	unsigned char* InpBytes = new unsigned char[MesLen];
	memset(InpBytes, 0, MesLen);
	memcpy(InpBytes, InpStr.c_str(), MesLen);

	int ByteIndex = iBit / 8;
	int BitIndex = iBit % 8;
	InpBytes[ByteIndex] = InpBytes[ByteIndex] ^ (1 << BitIndex);
	string sBrokenString = string((char*)InpBytes, MesLen);
	return sBrokenString;
}
int main(int argc, char* argv[])
{

	int keySize = 512;
	cout << "Input key size in bits	:\n\t";
	cin >> keySize;

	srand((unsigned int)time(0));
	RSACrypter rsaCrypter = RSACrypter(keySize);

	cout << "Input string to encrypt:\n\t";
	string sInpStr;
	getline(cin, sInpStr);
	getline(cin, sInpStr);

	string sEncStr = rsaCrypter.Encrypt(sInpStr);
	string sDecStr = rsaCrypter.Decryt(sEncStr);
	string sMsSign = rsaCrypter.Sign(sInpStr);
	string sBroken = sInpStr;
	cout << "\n\nEncrypted:\n\t" << sEncStr;
	cout << "\n\nDecrypted:\n\t" << sDecStr;
	cout << "\n\nSign     :\n\t" << sMsSign;
	cout << "\n\nVerify   :\n\t" << rsaCrypter.Verify(sInpStr, sMsSign);
	
	cout << "\n___________________________________________________________________________";
	sBroken = BreakBit(sInpStr, 0);
	cout << "\n\nBrokenMes:\n\t" << sBroken;
	string sBrSign = rsaCrypter.Sign(sBroken);
	cout << "\n\nBrokSign :\n\t" << sBrSign;
	cout << "\n\nVerifBrkn:\n\t" << rsaCrypter.Verify(sBroken, sMsSign);
	if (sBrSign == sMsSign)
		cout << "\n\nBroken sign == Message sign";
	else
		cout << "\n\nBroken sign != Message sign";
		
	system("@echo. & pause");
    return( 0 );
}
