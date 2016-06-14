#include "MpiBigInt.h"

BigInteger::BigInteger()
{
	mpi_init(&_MPI);
}

BigInteger::BigInteger(string NumberString)
{
	mpi_init(&_MPI);
	mpi_read_string(&_MPI, 10, NumberString.c_str());
}

BigInteger::BigInteger(BigInteger const & BI)
{
	mpi_init(&_MPI);
	mpi_copy(&_MPI, &BI._MPI);
}

BigInteger::BigInteger(int Num)
{
	mpi_init(&_MPI);
	mpi_add_int(&_MPI, &_MPI, Num);
}

BigInteger::BigInteger(double Num)
{
	mpi_init(&_MPI);
	mpi_add_int(&_MPI, &_MPI, (int)Num);
}

BigInteger::~BigInteger()
{
	mpi_free(&_MPI);
}
// Сгенерировать простое число размером keySize бит
BigInteger BigInteger::GenPrime(int keySize, bool dhFlag)
{
	BigInteger Res = BigInteger();
	mpi_gen_prime(&Res._MPI, keySize, dhFlag, _randFunc, NULL);
	return Res;
}
// Возвести число в степень E по модулю N
BigInteger BigInteger::PowAndMod(BigInteger const & E, BigInteger const & N)
{
	BigInteger Res = BigInteger();
	if (mpi_exp_mod(&Res._MPI, &_MPI, &E._MPI, &N._MPI, NULL)!=0)
		throw exception("Bad input parameters to function");
	return Res;
}
// Возвести число в степень -1 по модулю N
BigInteger BigInteger::InvMod(BigInteger const & N)
{
	BigInteger Res = BigInteger();
	mpi_inv_mod(&Res._MPI, &_MPI, &N._MPI);
	return Res;
}
// Необходимая длина буфера для хранения, в зависимотсти от СС
int BigInteger::BytesCount(int radix)
{
	size_t CurSize = mpi_msb(&_MPI);
	if (radix >= 4) CurSize >>= 1;
	if (radix >= 16) CurSize >>= 1;
	CurSize += 3;
	return CurSize;
}
// Возвращает строку с цифрами числа
string BigInteger::ToString(int radix)
{
	// Расчет необходимого размера буфера, в зависимости от СС
	size_t CurSize = BytesCount(radix);
	// Выделение памяти под параметры
	char* buf = new char[CurSize];
	size_t* slen = new size_t(CurSize);
	if (mpi_write_string(&_MPI, radix, buf, slen) != 0)
		throw new exception("Some problems in ToString()");
	return string(buf);
}
// Возвращает "сырую" строку, полученную из байтов числа
string BigInteger::ToRawString(int sizeInBytes)
{
	char* ByteArr = ToByteArr(sizeInBytes);
	string res = string(ByteArr, sizeInBytes);
	return res;
}
// Возвращает массив байт числа
char* BigInteger::ToByteArr()
{
	int bufSize = mpi_size(&_MPI);
	unsigned char *buf = new unsigned char[bufSize];
	memset(buf, 0, bufSize);
	mpi_write_binary(&_MPI, buf, bufSize);
	return (char*)buf;
}
// Возвращает массив байт числа, выравненный по правому краю
char * BigInteger::ToByteArr(int sizeInBytes)
{
	unsigned char *buf = new unsigned char[sizeInBytes];
	memset(buf, 0, sizeInBytes);
	mpi_write_binary(&_MPI, buf, sizeInBytes);
	return (char*)buf;
}

BigInteger BigInteger::FromByteArr(char* ByteArr, int sizeInBytes)
{
	BigInteger Res = BigInteger();
	mpi_read_binary(&Res._MPI, (unsigned char*)ByteArr, sizeInBytes);
	return Res;
}

BigInteger BigInteger::FromRawString(string RawString)
{
	BigInteger Res = BigInteger();
	const char* ByteArr = RawString.c_str();
	int  strlen = RawString.length();
	mpi_read_binary(&Res._MPI, (unsigned char*)ByteArr, strlen);
	return Res;
}

BigInteger BigInteger::operator=(BigInteger const & BI)
{
	if (this != &BI)
	{
		BigInteger(BI)._swap(*this);
	}
	return BigInteger();
}

BigInteger BigInteger::operator=(int Num)
{
	*this = BigInteger(Num);
	return BigInteger();
}

BigInteger BigInteger::operator=(string NumbString)
{
	*this = BigInteger(NumbString);
	return BigInteger();
}

BigInteger BigInteger::operator+(BigInteger const & BI)
{
	BigInteger Res = BigInteger();
	mpi_add_mpi(&Res._MPI, &_MPI, &BI._MPI);
	return Res;
}

BigInteger & BigInteger::operator++()
{
	BigInteger Temp = BigInteger(*this);
	Temp = Temp + 1;
	mpi_copy(&_MPI,&Temp._MPI);
	return *this;
}

BigInteger BigInteger::operator++(int)
{
	BigInteger Temp = *this;
	++*this;
	return Temp;
}

BigInteger BigInteger::operator-(BigInteger const & BI)
{
	BigInteger Res = BigInteger();
	mpi_sub_mpi(&Res._MPI, &_MPI, &BI._MPI);
	return Res;
}

BigInteger BigInteger::operator-(int Num)
{
	BigInteger Res = BigInteger();
	mpi_sub_int(&Res._MPI, &_MPI,Num);
	return Res;
}

BigInteger BigInteger::operator*(BigInteger const & BI)
{
	BigInteger Res = BigInteger();
	mpi_mul_mpi(&Res._MPI, &_MPI, &BI._MPI);
	return Res;
}

BigInteger BigInteger::operator/(BigInteger const & BI)
{
	BigInteger Res = BigInteger();
	mpi_div_mpi(&Res._MPI, NULL, &_MPI, &BI._MPI);
	return Res;
}

BigInteger BigInteger::operator%(BigInteger const & BI)
{
	BigInteger Res = BigInteger();
	mpi_div_mpi(NULL, &Res._MPI, &_MPI, &BI._MPI);
	return Res;
}

bool BigInteger::operator==(BigInteger const & BI)
{
	return !mpi_cmp_mpi(&_MPI,&BI._MPI);
}

bool BigInteger::operator!=(BigInteger & BI)
{
	return !(BI==*this);
}

bool BigInteger::operator>(BigInteger const & BI)
{
	if (mpi_cmp_mpi(&_MPI, &BI._MPI) == 1)
		return true;
	return false;
}

bool BigInteger::operator<(BigInteger const & BI)
{
	if (mpi_cmp_mpi(&_MPI, &BI._MPI) == -1)
		return true;
	return false;
}

bool BigInteger::operator>=(BigInteger const & BI)
{
	return !(*this<BI);
}

bool BigInteger::operator<=(BigInteger const & BI)
{
	return !(*this>BI);
}

void BigInteger::_swap(BigInteger &BI)
{
	mpi_init(&BI._MPI);
	mpi_swap(&BI._MPI, &_MPI);
}