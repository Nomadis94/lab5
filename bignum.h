/**
 * \file bignum.h
 *
 * ��������:  Multi-precision integer library
 *
 *  Copyright (C) 2006-2010, Brainspark B.V.
 *
 *  This file is part of PolarSSL (http://www.polarssl.org)
 *  Lead Maintainer: Paul Bakker <polarssl_maintainer at polarssl.org>
 *
 *  All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#ifndef POLARSSL_BIGNUM_H
#define POLARSSL_BIGNUM_H

#include <stdio.h>
#include <string.h>

#include "config.h"

#define POLARSSL_ERR_MPI_FILE_IO_ERROR                     -0x0002  /**< An error occurred while reading from or writing to a file. */
#define POLARSSL_ERR_MPI_BAD_INPUT_DATA                    -0x0004  /**< Bad input parameters to function. */
#define POLARSSL_ERR_MPI_INVALID_CHARACTER                 -0x0006  /**< There is an invalid character in the digit string. */
#define POLARSSL_ERR_MPI_BUFFER_TOO_SMALL                  -0x0008  /**< The buffer is too small to write to. */
#define POLARSSL_ERR_MPI_NEGATIVE_VALUE                    -0x000A  /**< The input arguments are negative or result in illegal output. */
#define POLARSSL_ERR_MPI_DIVISION_BY_ZERO                  -0x000C  /**< The input argument for division is zero, which is not allowed. */
#define POLARSSL_ERR_MPI_NOT_ACCEPTABLE                    -0x000E  /**< The input arguments are not acceptable. */
#define POLARSSL_ERR_MPI_MALLOC_FAILED                     -0x0010  /**< Memory allocation failed. */

#define MPI_CHK(f) if( ( ret = f ) != 0 ) goto cleanup

/*
 * Maximum size MPIs are allowed to grow to in number of limbs.
 */
#define POLARSSL_MPI_MAX_LIMBS                             10000

/*
 * Maximum window size used for modular exponentiation. Default: 6
 * Minimum value: 1. Maximum value: 6.
 *
 * Result is an array of ( 2 << POLARSSL_MPI_WINDOW_SIZE ) MPIs used
 * for the sliding window calculation. (So 64 by default)
 *
 * Reduction in size, reduces speed.
 */
#define POLARSSL_MPI_WINDOW_SIZE                           6        /**< Maximum windows size used. */

/*
 * Maximum size of MPIs allowed in bits and bytes for user-MPIs.
 * ( Default: 512 bytes => 4096 bits )
 *
 * Note: Calculations can results temporarily in larger MPIs. So the number
 * of limbs required (POLARSSL_MPI_MAX_LIMBS) is higher.
 */
#define POLARSSL_MPI_MAX_SIZE                              512      /**< Maximum number of bytes for usable MPIs. */
#define POLARSSL_MPI_MAX_BITS                              ( 8 * POLARSSL_MPI_MAX_SIZE )    /**< Maximum number of bits for usable MPIs. */

/*
 * When reading from files with mpi_read_file() the buffer should have space
 * for a (short) label, the MPI (in the provided radix), the newline
 * characters and the '\0'.
 *
 * By default we assume at least a 10 char label, a minimum radix of 10
 * (decimal) and a maximum of 4096 bit numbers (1234 decimal chars).
 */
#define POLARSSL_MPI_READ_BUFFER_SIZE                       1250   

/*
 * Define the base integer type, architecture-wise
 */
#if defined(POLARSSL_HAVE_INT8)
typedef   signed char  t_sint;
typedef unsigned char  t_uint;
typedef unsigned short t_udbl;
#else
#if defined(POLARSSL_HAVE_INT16)
typedef   signed short t_sint;
typedef unsigned short t_uint;
typedef unsigned long  t_udbl;
#else
  typedef   signed long t_sint;
  typedef unsigned long t_uint;
  #if defined(_MSC_VER) && defined(_M_IX86)
  typedef unsigned __int64 t_udbl;
  #else
    #if defined(__GNUC__) && (                          \
        defined(__amd64__) || defined(__x86_64__)    || \
        defined(__ppc64__) || defined(__powerpc64__) || \
        defined(__ia64__)  || defined(__alpha__)     || \
        (defined(__sparc__) && defined(__arch64__))  || \
        defined(__s390x__) )
    typedef unsigned int t_udbl __attribute__((mode(TI)));
    #define POLARSSL_HAVE_LONGLONG
    #else
      #if defined(POLARSSL_HAVE_LONGLONG)
      typedef unsigned long long t_udbl;
      #endif
    #endif
  #endif
#endif
#endif

/**
 * ��������:          MPI structure
 */
typedef struct
{
    int s;              /*!<  integer sign      */
    size_t n;           /*!<  total # of limbs  */
    t_uint *p;          /*!<  pointer to limbs  */
}
mpi;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * ��������:           Initialize one MPI
 *
 * �����. X         One MPI to initialize.
 */
void mpi_init( mpi *X );

/**
 * ��������:          Unallocate one MPI
 *
 * �����. X        One MPI to unallocate.
 */
void mpi_free( mpi *X );

/**
 * ��������:          Enlarge to the specified number of limbs
 *
 * �����. X        MPI to grow
 * �����. nblimbs  The target number of limbs
 *
 * �����. :         0 if successful,
 *                 POLARSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed
 */
int mpi_grow( mpi *X, size_t nblimbs );

/**
 * ��������:          Copy the contents of Y into X
 *
 * �����. X        Destination MPI
 * �����. Y        Source MPI
 *
 * �����. :         0 if successful,
 *                 POLARSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed
 */
int mpi_copy( mpi *X, const mpi *Y );

/**
 * ��������:          Swap the contents of X and Y
 *
 * �����. X        First MPI value
 * �����. Y        Second MPI value
 */
void mpi_swap( mpi *X, mpi *Y );

/**
 * ��������:          Set value from integer
 *
 * �����. X        MPI to set
 * �����. z        Value to use
 *
 * �����. :         0 if successful,
 *                 POLARSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed
 */
int mpi_lset( mpi *X, t_sint z );

/*
 * ��������:          Get a specific bit from X
 *
 * �����. X        MPI to use
 * �����. pos      Zero-based index of the bit in X
 *
 * �����. :         Either a 0 or a 1
 */
int mpi_get_bit( mpi *X, size_t pos );

/*
 * ��������:          Set a bit of X to a specific value of 0 or 1
 *
 * �����.:        Will grow X if necessary to set a bit to 1 in a not yet
 *                 existing limb. Will not grow if bit should be set to 0
 *
 * �����. X        MPI to use
 * �����. pos      Zero-based index of the bit in X
 * �����. val      The value to set the bit to (0 or 1)
 *
 * �����. :         0 if successful,
 *                 POLARSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed,
 *                 POLARSSL_ERR_MPI_BAD_INPUT_DATA if val is not 0 or 1
 */
int mpi_set_bit( mpi *X, size_t pos, unsigned char val );

/**
 * ��������:          Return the number of least significant bits
 *
 * �����. X        MPI to use
 */
size_t mpi_lsb( const mpi *X );

/**
 * ��������:          Return the number of most significant bits
 *
 * �����. X        MPI to use
 */
size_t mpi_msb( const mpi *X );

/**
 * ��������:          Return the total size in bytes
 *
 * �����. X        MPI to use
 */
size_t mpi_size( const mpi *X );

/**
 * ��������:          Import from an ASCII string
 *
 * �����. X        Destination MPI
 * �����. radix    Input numeric base
 * �����. s        Null-terminated string buffer
 *
 * �����. :         0 if successful, or a POLARSSL_ERR_MPI_XXX error code
 */
int mpi_read_string( mpi *X, int radix, const char *s );

/**
 * ��������:          Export into an ASCII string
 *
 * �����. X        Source MPI
 * �����. radix    Output numeric base
 * �����. s        String buffer
 * �����. slen     String buffer size
 *
 * �����. :         0 if successful, or a POLARSSL_ERR_MPI_XXX error code.
 *                 *slen is always updated to reflect the amount
 *                 of data that has (or would have) been written.
 *
 * �����.:        Call this function with *slen = 0 to obtain the
 *                 minimum required buffer size in *slen.
 */
int mpi_write_string( const mpi *X, int radix, char *s, size_t *slen );

/**
 * ��������:          Read X from an opened file
 *
 * �����. X        Destination MPI
 * �����. radix    Input numeric base
 * �����. fin      Input file handle
 *
 * �����. :         0 if successful, POLARSSL_ERR_MPI_BUFFER_TOO_SMALL if
 *                 the file read buffer is too small or a
 *                 POLARSSL_ERR_MPI_XXX error code
 */
int mpi_read_file( mpi *X, int radix, FILE *fin );

/**
 * ��������:          Write X into an opened file, or stdout if fout is NULL
 *
 * �����. p        Prefix, can be NULL
 * �����. X        Source MPI
 * �����. radix    Output numeric base
 * �����. fout     Output file handle (can be NULL)
 *
 * �����. :         0 if successful, or a POLARSSL_ERR_MPI_XXX error code
 *
 * �����.:        Set fout == NULL to print X on the console.
 */
int mpi_write_file( const char *p, const mpi *X, int radix, FILE *fout );

/**
 * ��������:          Import X from unsigned binary data, big endian
 *
 * �����. X        Destination MPI
 * �����. buf      Input buffer
 * �����. buflen   Input buffer size
 *
 * �����. :         0 if successful,
 *                 POLARSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed
 */
int mpi_read_binary( mpi *X, const unsigned char *buf, size_t buflen );

/**
 * ��������:          Export X into unsigned binary data, big endian
 *
 * �����. X        Source MPI
 * �����. buf      Output buffer
 * �����. buflen   Output buffer size
 *
 * �����. :         0 if successful,
 *                 POLARSSL_ERR_MPI_BUFFER_TOO_SMALL if buf isn't large enough
 */
int mpi_write_binary( const mpi *X, unsigned char *buf, size_t buflen );

/**
 * ��������:          Left-shift: X <<= count
 *
 * �����. X        MPI to shift
 * �����. count    Amount to shift
 *
 * �����. :         0 if successful,
 *                 POLARSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed
 */
int mpi_shift_l( mpi *X, size_t count );

/**
 * ��������:          Right-shift: X >>= count
 *
 * �����. X        MPI to shift
 * �����. count    Amount to shift
 *
 * �����. :         0 if successful,
 *                 POLARSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed
 */
int mpi_shift_r( mpi *X, size_t count );

/**
 * ��������:          Compare unsigned values
 *
 * �����. X        Left-hand MPI
 * �����. Y        Right-hand MPI
 *
 * �����. :         1 if |X| is greater than |Y|,
 *                -1 if |X| is lesser  than |Y| or
 *                 0 if |X| is equal to |Y|
 */
int mpi_cmp_abs( const mpi *X, const mpi *Y );

/**
 * ��������:          Compare signed values
 *
 * �����. X        Left-hand MPI
 * �����. Y        Right-hand MPI
 *
 * �����. :         1 if X is greater than Y,
 *                -1 if X is lesser  than Y or
 *                 0 if X is equal to Y
 */
int mpi_cmp_mpi( const mpi *X, const mpi *Y );

/**
 * ��������:          Compare signed values
 *
 * �����. X        Left-hand MPI
 * �����. z        The integer value to compare to
 *
 * �����. :         1 if X is greater than z,
 *                -1 if X is lesser  than z or
 *                 0 if X is equal to z
 */
int mpi_cmp_int( const mpi *X, t_sint z );

/**
 * ��������:          Unsigned addition: X = |A| + |B|
 *
 * �����. X        Destination MPI
 * �����. A        Left-hand MPI
 * �����. B        Right-hand MPI
 *
 * �����. :         0 if successful,
 *                 POLARSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed
 */
int mpi_add_abs( mpi *X, const mpi *A, const mpi *B );

/**
 * ��������:          Unsigned substraction: X = |A| - |B|
 *
 * �����. X        Destination MPI
 * �����. A        Left-hand MPI
 * �����. B        Right-hand MPI
 *
 * �����. :         0 if successful,
 *                 POLARSSL_ERR_MPI_NEGATIVE_VALUE if B is greater than A
 */
int mpi_sub_abs( mpi *X, const mpi *A, const mpi *B );

/**
 * ��������:          Signed addition: X = A + B
 *
 * �����. X        Destination MPI
 * �����. A        Left-hand MPI
 * �����. B        Right-hand MPI
 *
 * �����. :         0 if successful,
 *                 POLARSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed
 */
int mpi_add_mpi( mpi *X, const mpi *A, const mpi *B );

/**
 * ��������:          Signed substraction: X = A - B
 *
 * �����. X        Destination MPI
 * �����. A        Left-hand MPI
 * �����. B        Right-hand MPI
 *
 * �����. :         0 if successful,
 *                 POLARSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed
 */
int mpi_sub_mpi( mpi *X, const mpi *A, const mpi *B );

/**
 * ��������:          Signed addition: X = A + b
 *
 * �����. X        Destination MPI
 * �����. A        Left-hand MPI
 * �����. b        The integer value to add
 *
 * �����. :         0 if successful,
 *                 POLARSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed
 */
int mpi_add_int( mpi *X, const mpi *A, t_sint b );

/**
 * ��������:          Signed substraction: X = A - b
 *
 * �����. X        Destination MPI
 * �����. A        Left-hand MPI
 * �����. b        The integer value to subtract
 *
 * �����. :         0 if successful,
 *                 POLARSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed
 */
int mpi_sub_int( mpi *X, const mpi *A, t_sint b );

/**
 * ��������:          Baseline multiplication: X = A * B
 *
 * �����. X        Destination MPI
 * �����. A        Left-hand MPI
 * �����. B        Right-hand MPI
 *
 * �����. :         0 if successful,
 *                 POLARSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed
 */
int mpi_mul_mpi( mpi *X, const mpi *A, const mpi *B );

/**
 * ��������:          Baseline multiplication: X = A * b
 *                 Note: b is an unsigned integer type, thus
 *                 Negative values of b are ignored.
 *
 * �����. X        Destination MPI
 * �����. A        Left-hand MPI
 * �����. b        The integer value to multiply with
 *
 * �����. :         0 if successful,
 *                 POLARSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed
 */
int mpi_mul_int( mpi *X, const mpi *A, t_sint b );

/**
 * ��������:          Division by mpi: A = Q * B + R
 *
 * �����. Q        Destination MPI for the quotient
 * �����. R        Destination MPI for the rest value
 * �����. A        Left-hand MPI
 * �����. B        Right-hand MPI
 *
 * �����. :         0 if successful,
 *                 POLARSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed,
 *                 POLARSSL_ERR_MPI_DIVISION_BY_ZERO if B == 0
 *
 * �����.:        Either Q or R can be NULL.
 */
int mpi_div_mpi( mpi *Q, mpi *R, const mpi *A, const mpi *B );

/**
 * ��������:          Division by int: A = Q * b + R
 *
 * �����. Q        Destination MPI for the quotient
 * �����. R        Destination MPI for the rest value
 * �����. A        Left-hand MPI
 * �����. b        Integer to divide by
 *
 * �����. :         0 if successful,
 *                 POLARSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed,
 *                 POLARSSL_ERR_MPI_DIVISION_BY_ZERO if b == 0
 *
 * �����.:        Either Q or R can be NULL.
 */
int mpi_div_int( mpi *Q, mpi *R, const mpi *A, t_sint b );

/**
 * ��������:          Modulo: R = A mod B
 *
 * �����. R        Destination MPI for the rest value
 * �����. A        Left-hand MPI
 * �����. B        Right-hand MPI
 *
 * �����. :         0 if successful,
 *                 POLARSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed,
 *                 POLARSSL_ERR_MPI_DIVISION_BY_ZERO if B == 0,
 *                 POLARSSL_ERR_MPI_NEGATIVE_VALUE if B < 0
 */
int mpi_mod_mpi( mpi *R, const mpi *A, const mpi *B );

/**
 * ��������:          Modulo: r = A mod b
 *
 * �����. r        Destination t_uint
 * �����. A        Left-hand MPI
 * �����. b        Integer to divide by
 *
 * �����. :         0 if successful,
 *                 POLARSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed,
 *                 POLARSSL_ERR_MPI_DIVISION_BY_ZERO if b == 0,
 *                 POLARSSL_ERR_MPI_NEGATIVE_VALUE if b < 0
 */
int mpi_mod_int( t_uint *r, const mpi *A, t_sint b );

/**
 * ��������:          Sliding-window exponentiation: X = A^E mod N
 *
 * �����. X        Destination MPI 
 * �����. A        Left-hand MPI
 * �����. E        Exponent MPI
 * �����. N        Modular MPI
 * �����. _RR      Speed-up MPI used for recalculations
 *
 * �����. :         0 if successful,
 *                 POLARSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed,
 *                 POLARSSL_ERR_MPI_BAD_INPUT_DATA if N is negative or even
 *
 * �����.:        _RR is used to avoid re-computing R*R mod N across
 *                 multiple calls, which speeds up things a bit. It can
 *                 be set to NULL if the extra performance is unneeded.
 */
int mpi_exp_mod( mpi *X, const mpi *A, const mpi *E, const mpi *N, mpi *_RR );

/**
 * ��������:          Fill an MPI X with size bytes of random
 *
 * �����. X        Destination MPI
 * �����. size     Size in bytes
 * �����. f_rng    RNG function
 * �����. p_rng    RNG parameter
 *
 * �����. :         0 if successful,
 *                 POLARSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed
 */
int mpi_fill_random( mpi *X, size_t size,
                     int (*f_rng)(void *, unsigned char *, size_t),
                     void *p_rng );

/**
 * ��������:          Greatest common divisor: G = gcd(A, B)
 *
 * �����. G        Destination MPI
 * �����. A        Left-hand MPI
 * �����. B        Right-hand MPI
 *
 * �����. :         0 if successful,
 *                 POLARSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed
 */
int mpi_gcd( mpi *G, const mpi *A, const mpi *B );

/**
 * ��������:          Modular inverse: X = A^-1 mod N
 *
 * �����. X        Destination MPI
 * �����. A        Left-hand MPI
 * �����. N        Right-hand MPI
 *
 * �����. :         0 if successful,
 *                 POLARSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed,
 *                 POLARSSL_ERR_MPI_BAD_INPUT_DATA if N is negative or nil
                   POLARSSL_ERR_MPI_NOT_ACCEPTABLE if A has no inverse mod N
 */
int mpi_inv_mod( mpi *X, const mpi *A, const mpi *N );

/**
 * ��������:          Miller-Rabin primality test
 *
 * �����. X        MPI to check
 * �����. f_rng    RNG function
 * �����. p_rng    RNG parameter
 *
 * �����. :         0 if successful (probably prime),
 *                 POLARSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed,
 *                 POLARSSL_ERR_MPI_NOT_ACCEPTABLE if X is not prime
 */
int mpi_is_prime( mpi *X,
                  int (*f_rng)(void *, unsigned char *, size_t),
                  void *p_rng );

/**
 * ��������:          Prime number generation
 *
 * �����. X        Destination MPI
 * �����. nbits    Required size of X in bits ( 3 <= nbits <= POLARSSL_MPI_MAX_BITS )
 * �����. dh_flag  If 1, then (X-1)/2 will be prime too
 * �����. f_rng    RNG function
 * �����. p_rng    RNG parameter
 *
 * �����. :         0 if successful (probably prime),
 *                 POLARSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed,
 *                 POLARSSL_ERR_MPI_BAD_INPUT_DATA if nbits is < 3
 */
int mpi_gen_prime( mpi *X, size_t nbits, int dh_flag,
                   int (*f_rng)(void *, unsigned char *, size_t),
                   void *p_rng );

/**
 * ��������:          Checkup routine
 *
 * �����. :         0 if successful, or 1 if the test failed
 */
int mpi_self_test( int verbose );

#ifdef __cplusplus
}
#endif

#endif /* bignum.h */
