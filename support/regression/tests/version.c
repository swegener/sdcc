/* Ensure version macros are present
 */

#include <testfwk.h>


unsigned int major = __SDCC_VERSION_MAJOR;
unsigned int minor = __SDCC_VERSION_MINOR;
unsigned int revision = __SDCC_REVISION;

void testVersion(void)
{
}

