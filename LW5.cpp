#include "bignum.h"
#include <stdlib.h>

// Функция для получения рандома для генерации простых чисел
static int myrand( void *rng_state, unsigned char *output, size_t len )
{
    size_t i;

    if( rng_state != NULL )
        rng_state  = NULL;

    for( i = 0; i < len; ++i )
        output[i] = rand();
    
    return( 0 );
}

int main(int argc, char* argv[])
{
	mpi E, P, Q, N, H, D, X, Y, Z;

    ((void) argc);
    ((void) argv);

    mpi_init( &E ); mpi_init( &P ); mpi_init( &Q ); mpi_init( &N );
    mpi_init( &H ); mpi_init( &D ); mpi_init( &X ); mpi_init( &Y );
    mpi_init( &Z );

	if (mpi_gen_prime(&P, 512, 0, myrand, NULL) == 0)
	{
		printf( "\n  Primary:\n\n" );
		mpi_write_file( "  P = ", &P, 16, NULL );
	}

    mpi_read_string( &P, 10, "2789" );
    mpi_read_string( &Q, 10, "3203" );
    mpi_read_string( &E, 10,  "257" );
    mpi_mul_mpi( &N, &P, &Q );

    printf( "\n  Public key:\n\n" );
    mpi_write_file( "  N = ", &N, 10, NULL );
    mpi_write_file( "  E = ", &E, 10, NULL );

    printf( "\n  Private key:\n\n" );
    mpi_write_file( "  P = ", &P, 10, NULL );
    mpi_write_file( "  Q = ", &Q, 10, NULL );

#if defined(POLARSSL_GENPRIME)
    mpi_sub_int( &P, &P, 1 );
    mpi_sub_int( &Q, &Q, 1 );
    mpi_mul_mpi( &H, &P, &Q );
    mpi_inv_mod( &D, &E, &H );

    mpi_write_file( "  D = E^-1 mod (P-1)*(Q-1) = ",
                    &D, 10, NULL );
#else
    printf("\nTest skipped (POLARSSL_GENPRIME not defined).\n\n");
#endif
    mpi_read_string( &X, 10, "55555" );
    mpi_exp_mod( &Y, &X, &E, &N, NULL );
    mpi_exp_mod( &Z, &Y, &D, &N, NULL );

    printf( "\n  RSA operation:\n\n" );
    mpi_write_file( "  X (plaintext)  = ", &X, 10, NULL );
    mpi_write_file( "  Y (ciphertext) = X^E mod N = ", &Y, 10, NULL );
    mpi_write_file( "  Z (decrypted)  = Y^D mod N = ", &Z, 10, NULL );
    printf( "\n" );

    mpi_free( &E ); mpi_free( &P ); mpi_free( &Q ); mpi_free( &N );
    mpi_free( &H ); mpi_free( &D ); mpi_free( &X ); mpi_free( &Y );
    mpi_free( &Z );

#if defined(_WIN32)
    printf( "  Press Enter to exit this program...\n" );
    fflush( stdout ); getchar();
#endif

    return( 0 );
}