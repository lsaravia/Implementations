/******************************************************************************
*  Module:  r250.c   
*  Description: implements R250 random number generator, from S. 
*  Kirkpatrick and E.  Stoll, Journal of Computational Physics, 40, p. 
*  517 (1981).
*  Written by:    W. L. Maier
*
*	METHOD...
*		16 parallel copies of a linear shift register with
*		period 2^250 - 1.  FAR longer period than the usual
*		linear congruent generator, and commonly faster as
*		well.  (For details see the above paper, and the
*		article in DDJ referenced below.)
*
*	HISTORY...
*		Sep 92: Number returned by dr250() is in range [0,1) instead 
*			of [0,1], so for example a random angle in the
*			interval [0, 2*PI) can be calculated
*			conveniently.  (J. R. Van Zandt <jrv@mbunix.mitre.org>)
*		Aug 92: Initialization is optional.  Default condition is 
*			equivalent to initializing with the seed 12345,
*			so that the sequence of random numbers begins:
*			1173, 53403, 52352, 35341...  (9996 numbers
*			skipped) ...57769, 14511, 46930, 11942, 7978,
*			56163, 46506, 45768, 21162, 43113...  Using ^=
*			operator rather than two separate statements. 
*			Initializing with own linear congruent
*			pseudorandom number generator for portability. 
*			Function prototypes moved to a header file. 
*			Implemented r250n() to generate numbers
*			uniformly distributed in a specific range
*			[0,n), because the common expedient rand()%n is
*			incorrect.  (J. R. Van Zandt <jrv@mbunix.mitre.org>)
*		May 91: Published by W. L. Maier, "A Fast Pseudo Random Number 
*			Generator", Dr. Dobb's Journal #176.
******************************************************************************/

#include <stdlib.h>
#include "r250.h"

/**** Static variables ****/
static int r250_index = 0;
static unsigned int r250_buffer[250] = {
	15301U,57764U,10921U,56345U,19316U,43154U,54727U,49252U,32360U,49582U,
	26124U,25833U,34404U,11030U,26232U,13965U,16051U,63635U,55860U,5184U,
	15931U,39782U,16845U,11371U,38624U,10328U,9139U,1684U,48668U,59388U,
	13297U,1364U,56028U,15687U,63279U,27771U,5277U,44628U,31973U,46977U,
	16327U,23408U,36065U,52272U,33610U,61549U,58364U,3472U,21367U,56357U,
	56345U,54035U,7712U,55884U,39774U,10241U,50164U,47995U,1718U,46887U,
	47892U,6010U,29575U,54972U,30458U,21966U,54449U,10387U,4492U,644U,
	57031U,41607U,61820U,54588U,40849U,54052U,59875U,43128U,50370U,44691U,
	286U,12071U,3574U,61384U,15592U,45677U,9711U,23022U,35256U,45493U,
	48913U,146U,9053U,5881U,36635U,43280U,53464U,8529U,34344U,64955U,
	38266U,12730U,101U,16208U,12607U,58921U,22036U,8221U,31337U,11984U,
	20290U,26734U,19552U,48U,31940U,43448U,34762U,53344U,60664U,12809U,
	57318U,17436U,44730U,19375U,30U,17425U,14117U,5416U,23853U,55783U,
	57995U,32074U,26526U,2192U,11447U,11U,53446U,35152U,64610U,64883U,
	26899U,25357U,7667U,3577U,39414U,51161U,4U,58427U,57342U,58557U,
	53233U,1066U,29237U,36808U,19370U,17493U,37568U,3U,61468U,38876U,
	17586U,64937U,21716U,56472U,58160U,44955U,55221U,63880U,1U,32200U,
	62066U,22911U,24090U,10438U,40783U,36364U,14999U,2489U,43284U,9898U,
	39612U,9245U,593U,34857U,41054U,30162U,65497U,53340U,27209U,45417U,
	37497U,4612U,58397U,52910U,56313U,62716U,22377U,40310U,15190U,34471U,
	64005U,18090U,11326U,50839U,62901U,59284U,5580U,15231U,9467U,13161U,
	58500U,7259U,317U,50968U,2962U,23006U,32280U,6994U,18751U,5148U,
	52739U,49370U,51892U,18552U,52264U,54031U,2804U,17360U,1919U,19639U,
	2323U,9448U,43821U,11022U,45500U,31509U,49180U,35598U,38883U,19754U,
	987U,11521U,55494U,38056U,20664U,2629U,50986U,31009U,54043U,59743
	};

static unsigned myrand();
static void mysrand(unsigned newseed);

/**** Function: r250_init  
	Description: initializes r250 random number generator. ****/
void r250_init(int seed)
{
/*---------------------------------------------------------------------------*/
	int        j, k;
    unsigned int mask;
    unsigned int msb;
/*---------------------------------------------------------------------------*/
    mysrand(seed);
    r250_index = 0;
    for (j = 0; j < 250; j++)     /* Fill the r250 buffer with 15-bit values */
        r250_buffer[j] = myrand();
    for (j = 0; j < 250; j++)     /* Set some of the MS bits to 1 */
        if (myrand() > 16384)
            r250_buffer[j] |= 0x8000;
    msb = 0x8000;       /* To turn on the diagonal bit   */
    mask = 0xffff;      /* To turn off the leftmost bits */
    for (j = 0; j < 16; j++)
        {
        k = 11 * j + 3;             /* Select a word to operate on        */
        r250_buffer[k] &= mask;     /* Turn off bits left of the diagonal */
        r250_buffer[k] |= msb;      /* Turn on the diagonal bit           */
        mask >>= 1;
        msb >>= 1;
        }
}

/**** Function: r250 Description: returns a random unsigned integer k
				uniformly distributed in the interval 0 <= k < 65536.  ****/
unsigned int r250()
{
/*---------------------------------------------------------------------------*/
    register int    j;
    register unsigned int new_rand;
/*---------------------------------------------------------------------------*/
    if (r250_index >= 147)
        j = r250_index - 147;      /* Wrap pointer around */
    else
        j = r250_index + 103;

    new_rand = r250_buffer[r250_index] ^= r250_buffer[j];

    if (r250_index >= 249)      /* Increment pointer for next time */
        r250_index = 0;
    else
        r250_index++;

    return new_rand;
}

/**** Function: r250n Description: returns a random unsigned integer k
					uniformly distributed in the interval 0 <= k < n ****/
unsigned int r250n(unsigned n)
{
/*---------------------------------------------------------------------------*/
    register int    j;
    register unsigned int new_rand, limit;
/*---------------------------------------------------------------------------*/
	limit = (65535U/n)*n;
	do 
        {new_rand = r250();
        if (r250_index >= 147)
            j = r250_index - 147;      /* Wrap pointer around */
        else
            j = r250_index + 103;

        new_rand = r250_buffer[r250_index] ^= r250_buffer[j];

        if (r250_index >= 249)      /* Increment pointer for next time */
            r250_index = 0;
        else
            r250_index++;
        } while(new_rand >= limit);
    return new_rand%n;
}

/**** Function: dr250 
		Description: returns a random double z in range 0 <= z < 1.  ****/
double dr250()
{
/*---------------------------------------------------------------------------*/
    register int    j;
    register unsigned int new_rand;
/*---------------------------------------------------------------------------*/
    if (r250_index >= 147)
        j = r250_index - 147;     /* Wrap pointer around */
    else
        j = r250_index + 103;

    new_rand = r250_buffer[r250_index] ^= r250_buffer[j];

    if (r250_index >= 249)      /* Increment pointer for next time */
        r250_index = 0;
    else
        r250_index++;
    return new_rand / 65536.;   /* Return a number in [0.0 to 1.0) */
}


/***  linear congruent pseudorandom number generator for initialization ***/

static unsigned long seed=1;

	/*	Return a pseudorandom number in the interval 0 <= n < 32768.
		This produces the following sequence of pseudorandom 
		numbers:
		346, 130, 10982, 1090...  (9996 numbers skipped) ...23369,
		2020, 5703, 12762, 10828, 16252, 28648, 27041, 23444, 6604... 
	*/ 
static unsigned myrand()
{
	seed = seed*0x015a4e35L + 1;
	return (seed>>16)&0x7fff;
}

	/*	Initialize above linear congruent pseudorandom number generator */
static void mysrand(unsigned newseed)
{	seed = newseed;
}

