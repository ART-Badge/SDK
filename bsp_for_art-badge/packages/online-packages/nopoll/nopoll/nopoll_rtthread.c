#include "nopoll_rtthread.h"
#include <rtthread.h>
#include <lwip/sockets.h>

/*
 * os mutex routine
 */
static noPollPtr   nopoll_rtt_mutex_create (void)
{
	noPollPtr ptr;
    ptr = rt_mutex_create("nopoll", RT_IPC_FLAG_FIFO);

    return ptr;
}

static void        nopoll_rtt_mutex_lock    (noPollPtr mutex)
{
    rt_mutex_t m = (rt_mutex_t) mutex;

    rt_mutex_take(m, RT_WAITING_FOREVER);
    return;
}

static void        nopoll_rtt_mutex_unlock  (noPollPtr mutex)
{
    rt_mutex_t m = (rt_mutex_t) mutex;

    rt_mutex_release(m);
    return;
}

static void        nopoll_rtt_mutex_destroy (noPollPtr mutex)
{
    rt_mutex_t m = (rt_mutex_t) mutex;

    rt_mutex_delete(m);
    return;
}

/*
 * socket send/recv, because these two routines was defined by lwIP macro.
 */
#ifndef SAL_USING_POSIX

#ifdef send
#undef send
#endif

int send(int s, const void *dataptr, size_t size, int flags)
{
    return lwip_send(s, dataptr, size, flags);
}

#ifdef recv
#undef recv
#endif

int recv(int s, void *mem, size_t len, int flags)
{
    return lwip_recv(s, mem, len, flags);
}
#endif

/*
 * random routine, just to use os_tick
 */
unsigned int nopoll_rtt_random(void)
{
	return rt_tick_get();
}

/*
 * base64 routines
 */
static const unsigned char base64_enc_map[64] =
{
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd',
    'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
    'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', '+', '/'
};

static const unsigned char base64_dec_map[128] =
{
    127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
    127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
    127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
    127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
    127, 127, 127,  62, 127, 127, 127,  63,  52,  53,
     54,  55,  56,  57,  58,  59,  60,  61, 127, 127,
    127,  64, 127, 127, 127,   0,   1,   2,   3,   4,
      5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
     15,  16,  17,  18,  19,  20,  21,  22,  23,  24,
     25, 127, 127, 127, 127, 127, 127,  26,  27,  28,
     29,  30,  31,  32,  33,  34,  35,  36,  37,  38,
     39,  40,  41,  42,  43,  44,  45,  46,  47,  48,
     49,  50,  51, 127, 127, 127, 127, 127
};

/*
 * Encode a buffer into base64 format
 */
int _base64_encode( unsigned char *dst, size_t *dlen,
                   const unsigned char *src, size_t slen )
{
    size_t i, n;
    int C1, C2, C3;
    unsigned char *p;

    if( slen == 0 )
        return( 0 );

    n = ( slen << 3 ) / 6;

    switch( ( slen << 3 ) - ( n * 6 ) )
    {
        case  2: n += 3; break;
        case  4: n += 2; break;
        default: break;
    }

    if( *dlen < n + 1 )
    {
        *dlen = n + 1;
        return( -1 );
    }

    n = ( slen / 3 ) * 3;

    for( i = 0, p = dst; i < n; i += 3 )
    {
        C1 = *src++;
        C2 = *src++;
        C3 = *src++;

        *p++ = base64_enc_map[(C1 >> 2) & 0x3F];
        *p++ = base64_enc_map[(((C1 &  3) << 4) + (C2 >> 4)) & 0x3F];
        *p++ = base64_enc_map[(((C2 & 15) << 2) + (C3 >> 6)) & 0x3F];
        *p++ = base64_enc_map[C3 & 0x3F];
    }

    if( i < slen )
    {
        C1 = *src++;
        C2 = ( ( i + 1 ) < slen ) ? *src++ : 0;

        *p++ = base64_enc_map[(C1 >> 2) & 0x3F];
        *p++ = base64_enc_map[(((C1 & 3) << 4) + (C2 >> 4)) & 0x3F];

        if( ( i + 1 ) < slen )
             *p++ = base64_enc_map[((C2 & 15) << 2) & 0x3F];
        else *p++ = '=';

        *p++ = '=';
    }

    *dlen = p - dst;
    *p = 0;

    return( 0 );
}

/*
 * Decode a base64-formatted buffer
 */
int _base64_decode( unsigned char *dst, size_t *dlen,
                   const unsigned char *src, size_t slen )
{
    size_t i, n;
    uint32_t j, x;
    unsigned char *p;

    for( i = n = j = 0; i < slen; i++ )
    {
        if( ( slen - i ) >= 2 &&
            src[i] == '\r' && src[i + 1] == '\n' )
            continue;

        if( src[i] == '\n' )
            continue;

        if( src[i] == '=' && ++j > 2 )
            return( -1 );

        if( src[i] > 127 || base64_dec_map[src[i]] == 127 )
            return( -1 );

        if( base64_dec_map[src[i]] < 64 && j != 0 )
            return( -1 );

        n++;
    }

    if( n == 0 )
        return( 0 );

    n = ( ( n * 6 ) + 7 ) >> 3;
    n -= j;

    if( dst == NULL || *dlen < n )
    {
        *dlen = n;
        return( -1 );
    }

   for( j = 3, n = x = 0, p = dst; i > 0; i--, src++ )
   {
        if( *src == '\r' || *src == '\n' )
            continue;

        j -= ( base64_dec_map[*src] == 64 );
        x  = ( x << 6 ) | ( base64_dec_map[*src] & 0x3F );

        if( ++n == 4 )
        {
            n = 0;
            if( j > 0 ) *p++ = (unsigned char)( x >> 16 );
            if( j > 1 ) *p++ = (unsigned char)( x >>  8 );
            if( j > 2 ) *p++ = (unsigned char)( x       );
        }
    }

    *dlen = p - dst;

    return( 0 );
}

nopoll_bool nopoll_rtt_base64_encode (const char  * content,
				  int           length,
				  char        * output,
				  int         * output_size)
{
	nopoll_bool result = nopoll_false;

	if (_base64_encode(output, output_size, content, length) == 0)
		result = nopoll_true;

	return result;
}

nopoll_bool nopoll_rtt_base64_decode (const char * content,
				  int          length,
				  char       * output,
				  int        * output_size)
{
	nopoll_bool result = nopoll_false;

	if (_base64_decode(output, output_size, content, length) == 0)
		result = nopoll_true;

	return result;
}

/*
 * memory related routines
 */
noPollPtr nopoll_calloc(size_t count, size_t size)
{
	noPollPtr ptr;

	ptr = rt_malloc(count * size);
	if (ptr) memset(ptr, 0x0, count * size);

	return ptr;
}

noPollPtr nopoll_realloc(noPollPtr ref, size_t size)
{
	return rt_realloc(ref, size);
}

void nopoll_free (noPollPtr ref)
{
	rt_free (ref);
	return;
}

#ifdef __IAR_SYSTEMS_ICC__
#define MICROSECOND_PER_SECOND  1000000UL
#define MICROSECOND_PER_TICK    (MICROSECOND_PER_SECOND / RT_TICK_PER_SECOND)

/* POSIX thread provides clock_gettime function */
#include <time.h>
int gettimeofday(struct timeval *__tp, void *__tzp)
{
    if (__tp)
    {
        rt_tick_t tick;

        /* get tick */
        tick = rt_tick_get();

        __tp->tv_sec = tick / RT_TICK_PER_SECOND;
        __tp->tv_usec = ((tick % RT_TICK_PER_SECOND) * MICROSECOND_PER_TICK);
    }

    return 0;
}
#endif

/*
 * Initialization routine
 */
int nopoll_rtt_init(void)
{
	nopoll_thread_handlers(nopoll_rtt_mutex_create,
			nopoll_rtt_mutex_destroy,
			nopoll_rtt_mutex_lock,
			nopoll_rtt_mutex_unlock);

	return 0;
}
INIT_COMPONENT_EXPORT(nopoll_rtt_init);
