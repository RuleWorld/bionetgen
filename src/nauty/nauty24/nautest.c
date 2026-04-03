/* Test for basic nauty functions (but not nauty itself) */

#include "nauty.h"

int
main(int argc, char *argv[])
{
	int i,j,bad;
	setword w;

	printf("NAUTYVERSION=%s NAUTYVERSIONID=%d\n",
		NAUTYVERSION,NAUTYVERSIONID);
	printf("MAXN=%d MAXM=%d WORDSIZE=%d NAUTY_INFINITY=%d\n",
		MAXN,MAXM,WORDSIZE,NAUTY_INFINITY);
	printf("sizes: short=%d int=%d long=%d double=%d\n",
		(int)sizeof(short),(int)sizeof(int),(int)sizeof(long),
		(int)sizeof(double));
	printf("sizes: boolean=%d setword=%d shortish=%d\n",
		(int)sizeof(boolean),(int)sizeof(setword),
		(int)sizeof(shortish));
#ifdef SETWORD_LONGLONG
	printf("sizeof(long long)=%d\n",sizeof(long long));
#endif

	printf("defined:");
#ifdef __STDC__
	printf(" __STDC__");
#endif
#ifdef BIGNAUTY
	printf(" BIGNAUTY");
#endif
#ifdef SYS_UNIX
	printf(" SYS_UNIX");
#endif
#ifdef SYS_CRAY
        printf(" SYS_CRAY");
#endif
#ifdef SETWORD_SHORT
	printf(" SETWORD_SHORT");
#endif
#ifdef SETWORD_INT
	printf(" SETWORD_INT");
#endif
#ifdef SETWORD_LONG
	printf(" SETWORD_LONG");
#endif
#ifdef SETWORD_LONGLONG
	printf(" SETWORD_LONGLONG");
#endif
	printf("\n");

	bad = 0;

	if (8*sizeof(setword) != WORDSIZE)
	{
	    printf("\n ***** NOTE:  WORDSIZE mismatch *****\n\n");
	    ++bad;
	}

	for (i = 0; i < WORDSIZE; ++i)
	{
	    w = ALLMASK(i);
	    if (POPCOUNT(w) != i)
	    {
		printf("\n ***** POPCOUNT(ALLMASK) error %d *****\n\n",i);
		++bad;
	    }
	}

	for (i = 0; i < WORDSIZE; ++i)
        {
            w = BITMASK(i);
            if (POPCOUNT(w) != WORDSIZE-i-1)
            {
                printf("\n ***** POPCOUNT(BITMASK) error %d *****\n\n",i);
                ++bad;
            }
        }

	for (i = 0; i < WORDSIZE; ++i)
	    if (POPCOUNT(ALLMASK(i)) != i)
	    {
		printf("\n ***** POPCOUNT(ALLMASK) error %d *****\n\n",i);
		++bad;
	    }

	for (i = 0; i < WORDSIZE; ++i)
            if (FIRSTBIT(BITT[i]) != i)
	    {
		printf("\n ***** FIRSTBIT(BITT) error %d *****\n\n",i);
		++bad;
	    }
	
	for (i = 0; i < WORDSIZE; ++i)
            if (POPCOUNT(BITT[i]) != 1)
	    {
                printf("\n ***** POPCOUNT(BITT) error %d *****\n\n",i);
		++bad;
	    }

	for (i = 0; i < WORDSIZE; ++i)
	{
	    w = 0;
	    for (j = 1; j <= WORDSIZE; ++j)
	    {
		w |= BITT[(j*97+i)%WORDSIZE];
		if (POPCOUNT(w) != j)
		{
		    printf("\n ***** POPCOUNT(w) error %d %d *****\n\n",i,j);
		    ++bad;
		}
	    }
	}

	if (!bad) printf("\nNo errors found\n");
	else      printf("\nXXXXXXX %d errors found XXXXXXX\n",bad);

	exit(0);
} 
