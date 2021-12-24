#ifndef NOPOLL_RTTHREAD_H__
#define NOPOLL_RTTHREAD_H__

#include "nopoll.h"
#include <tiny_sha1.h>

unsigned int nopoll_rtt_random(void);

nopoll_bool nopoll_rtt_base64_encode (const char  * content,
				  int           length,
				  char        * output,
				  int         * output_size);

nopoll_bool nopoll_rtt_base64_decode (const char * content,
				  int          length,
				  char       * output,
				  int        * output_size);

#endif
