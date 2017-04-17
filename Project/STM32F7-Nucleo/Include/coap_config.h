#ifndef _CONFIG_H_
#define _CONFIG_H_

//#include <lwip/opt.h>
//#include <lwip/debug.h>
//#include <lwip/def.h> /* provide ntohs, htons */

//#define WITH_LWIP 0

#define CONTIKI
#define BSD             199103

#define PACKAGE_NAME "libcoap-lwip"
#define PACKAGE_VERSION "?"
#define PACKAGE_STRING PACKAGE_NAME PACKAGE_VERSION

#define coap_assert(x) do{if(!(x)) while(1);}while(0)

/* it's just provided by libc. i hope we don't get too many of those, as
 * actually we'd need autotools again to find out what environment we're
 * building in */
#define HAVE_STRNLEN 1

#define HAVE_LIMITS_H

#define COAP_RESOURCES_NOHASH

#define USE_COAP_PRNG           1

#endif /* _CONFIG_H_ */
