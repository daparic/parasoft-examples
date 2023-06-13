#ifndef SOCKET_H
#define SOCKET_H

#if defined(PARASOFT_IMPOSE_OUR_RECV_DEFINITION) || defined(WIN32) || defined(_MSC_VER) || defined(__MINGW32__) || defined(__sh__) || defined(__VXWORKS__) || defined(__DCC__) || defined(__sun) || defined(__ARMCC__) || defined(__TI_COMPILER_VERSION__) || defined (__ARMCC_VERSION) || defined (_ICC) || defined (__sh__) || defined (__IAR_SYSTEMS_ICC__) || defined(__NIOS2__) || defined(_dsp) || defined(_TMS320C6X) || defined(_C196_) || defined (__TASKING__) || defined(__nios__) || defined(_SH) || defined (__ghs__) || defined (__COMPILER_FCC911__) || ( defined(__MWERKS__) && defined(__HIWARE__) && defined(__HC12__) ) || defined(__C30__) || (defined(__RENESAS__) && defined(__RX)) || defined(__BORLANDC__) || defined(_ARC)

unsigned int recv(int s, void *buf, unsigned int len, int flags)
{
	return 0;
}

#else

#include <sys/types.h>
#include <sys/socket.h>

#endif

#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 1
#endif

#endif
