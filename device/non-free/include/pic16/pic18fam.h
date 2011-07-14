/*
 * pic18fam.h - PIC16 families
 *
 * This file is has been generated using support/scripts/pic18fam-h-gen.pl .
 */
#ifndef __SDCC_PIC18FAM_H__
#define __SDCC_PIC18FAM_H__ 1

/*
 * Define device families.
 */
#undef  __SDCC_PIC16_FAMILY

#if    defined(pic18f242) \
    || defined(pic18f252) \
    || defined(pic18f442) \
    || defined(pic18f452)
#define __SDCC_PIC16_FAMILY 1802420

#elif  defined(pic18f248) \
    || defined(pic18f258) \
    || defined(pic18f448) \
    || defined(pic18f458)
#define __SDCC_PIC16_FAMILY 1802480

#elif  defined(pic18f1220) \
    || defined(pic18f1320)
#define __SDCC_PIC16_FAMILY 1812200

#elif  defined(pic18f13k50) \
    || defined(pic18f14k50)
#define __SDCC_PIC16_FAMILY 1813502

#elif  defined(pic18f2220) \
    || defined(pic18f2320) \
    || defined(pic18f4220) \
    || defined(pic18f4320)
#define __SDCC_PIC16_FAMILY 1822200

#elif  defined(pic18f2221) \
    || defined(pic18f2321) \
    || defined(pic18f4221) \
    || defined(pic18f4321)
#define __SDCC_PIC16_FAMILY 1822210

#elif  defined(pic18f23k20) \
    || defined(pic18f24k20) \
    || defined(pic18f25k20) \
    || defined(pic18f26k20) \
    || defined(pic18f43k20) \
    || defined(pic18f44k20) \
    || defined(pic18f45k20) \
    || defined(pic18f46k20)
#define __SDCC_PIC16_FAMILY 1823202

#elif  defined(pic18f23k22) \
    || defined(pic18f24k22) \
    || defined(pic18f25k22) \
    || defined(pic18f26k22) \
    || defined(pic18f43k22) \
    || defined(pic18f44k22) \
    || defined(pic18f45k22) \
    || defined(pic18f46k22)
#define __SDCC_PIC16_FAMILY 1823222

#elif  defined(pic18f2331) \
    || defined(pic18f2431) \
    || defined(pic18f4331) \
    || defined(pic18f4431)
#define __SDCC_PIC16_FAMILY 1823310

#elif  defined(pic18f2410) \
    || defined(pic18f2510) \
    || defined(pic18f2515) \
    || defined(pic18f2610) \
    || defined(pic18f4410) \
    || defined(pic18f4510) \
    || defined(pic18f4515) \
    || defined(pic18f4610)
#define __SDCC_PIC16_FAMILY 1824100

#elif  defined(pic18f24j10) \
    || defined(pic18f25j10) \
    || defined(pic18f44j10) \
    || defined(pic18f45j10)
#define __SDCC_PIC16_FAMILY 1824101

#elif  defined(pic18f2420) \
    || defined(pic18f2520) \
    || defined(pic18f4420) \
    || defined(pic18f4520)
#define __SDCC_PIC16_FAMILY 1824200

#elif  defined(pic18f2423) \
    || defined(pic18f2523) \
    || defined(pic18f4423) \
    || defined(pic18f4523)
#define __SDCC_PIC16_FAMILY 1824230

#elif  defined(pic18f2450) \
    || defined(pic18f4450)
#define __SDCC_PIC16_FAMILY 1824500

#elif  defined(pic18f24j50) \
    || defined(pic18f25j50) \
    || defined(pic18f26j50) \
    || defined(pic18f44j50) \
    || defined(pic18f45j50) \
    || defined(pic18f46j50)
#define __SDCC_PIC16_FAMILY 1824501

#elif  defined(pic18f2455) \
    || defined(pic18f2550) \
    || defined(pic18f4455) \
    || defined(pic18f4550)
#define __SDCC_PIC16_FAMILY 1824550

#elif  defined(pic18f2480) \
    || defined(pic18f2580) \
    || defined(pic18f4480) \
    || defined(pic18f4580)
#define __SDCC_PIC16_FAMILY 1824800

#elif  defined(pic18f2525) \
    || defined(pic18f2620) \
    || defined(pic18f4525) \
    || defined(pic18f4620)
#define __SDCC_PIC16_FAMILY 1825250

#elif  defined(pic18f2585) \
    || defined(pic18f2680) \
    || defined(pic18f4585) \
    || defined(pic18f4680)
#define __SDCC_PIC16_FAMILY 1825850

#elif  defined(pic18f2682) \
    || defined(pic18f2685) \
    || defined(pic18f4682) \
    || defined(pic18f4685)
#define __SDCC_PIC16_FAMILY 1826820

#elif  defined(pic18f6520) \
    || defined(pic18f6620) \
    || defined(pic18f6720) \
    || defined(pic18f8520) \
    || defined(pic18f8620) \
    || defined(pic18f8720)
#define __SDCC_PIC16_FAMILY 1865200

#elif  defined(pic18f6527) \
    || defined(pic18f6622) \
    || defined(pic18f6627) \
    || defined(pic18f6722) \
    || defined(pic18f8527) \
    || defined(pic18f8622) \
    || defined(pic18f8627) \
    || defined(pic18f8722)
#define __SDCC_PIC16_FAMILY 1865270

#elif  defined(pic18f65j50) \
    || defined(pic18f66j50) \
    || defined(pic18f66j55) \
    || defined(pic18f67j50) \
    || defined(pic18f85j50) \
    || defined(pic18f86j50) \
    || defined(pic18f86j55) \
    || defined(pic18f87j50)
#define __SDCC_PIC16_FAMILY 1865501

#elif  defined(pic18f6585) \
    || defined(pic18f6680) \
    || defined(pic18f8585) \
    || defined(pic18f8680)
#define __SDCC_PIC16_FAMILY 1865850

#elif  defined(pic18f66j60) \
    || defined(pic18f66j65) \
    || defined(pic18f67j60) \
    || defined(pic18f86j60) \
    || defined(pic18f86j65) \
    || defined(pic18f87j60) \
    || defined(pic18f96j60) \
    || defined(pic18f96j65) \
    || defined(pic18f97j60)
#define __SDCC_PIC16_FAMILY 1866601

#else
#warning No family associated with the target device. Please update your pic16/pic18fam.h manually and/or inform the maintainer.
#endif

/*
 * Define ADC style per device family.
 */
#undef  __SDCC_ADC_STYLE

#if    (__SDCC_PIC16_FAMILY == 1823310)
#define __SDCC_ADC_STYLE    0

#elif  (__SDCC_PIC16_FAMILY == 1802420) \
    || (__SDCC_PIC16_FAMILY == 1802480)
#define __SDCC_ADC_STYLE    1802420

#elif  (__SDCC_PIC16_FAMILY == 1812200)
#define __SDCC_ADC_STYLE    1812200

#elif  (__SDCC_PIC16_FAMILY == 1813502)
#define __SDCC_ADC_STYLE    1813502

#elif  (__SDCC_PIC16_FAMILY == 1822200) \
    || (__SDCC_PIC16_FAMILY == 1822210) \
    || (__SDCC_PIC16_FAMILY == 1823202) \
    || (__SDCC_PIC16_FAMILY == 1824100) \
    || (__SDCC_PIC16_FAMILY == 1824101) \
    || (__SDCC_PIC16_FAMILY == 1824200) \
    || (__SDCC_PIC16_FAMILY == 1824230) \
    || (__SDCC_PIC16_FAMILY == 1824500) \
    || (__SDCC_PIC16_FAMILY == 1824550) \
    || (__SDCC_PIC16_FAMILY == 1824800) \
    || (__SDCC_PIC16_FAMILY == 1825250) \
    || (__SDCC_PIC16_FAMILY == 1825850) \
    || (__SDCC_PIC16_FAMILY == 1826820) \
    || (__SDCC_PIC16_FAMILY == 1865200) \
    || (__SDCC_PIC16_FAMILY == 1865270) \
    || (__SDCC_PIC16_FAMILY == 1865850) \
    || (__SDCC_PIC16_FAMILY == 1866601)
#define __SDCC_ADC_STYLE    1822200

#elif  (__SDCC_PIC16_FAMILY == 1823222)
#define __SDCC_ADC_STYLE    1823222

#elif  (__SDCC_PIC16_FAMILY == 1824501)
#define __SDCC_ADC_STYLE    1824501

#elif  (__SDCC_PIC16_FAMILY == 1865501)
#define __SDCC_ADC_STYLE    1865501

#else
#warning No ADC style associated with the target device. Please update your pic16/pic18fam.h manually and/or inform the maintainer.
#endif

/*
 * Define (E)USART style per device family.
 */
#undef  __SDCC_USART_STYLE

#if    (__SDCC_PIC16_FAMILY == 1812200)
#define __SDCC_USART_STYLE  1812200

#elif  (__SDCC_PIC16_FAMILY == 1813502)
#define __SDCC_USART_STYLE  1813502

#elif  (__SDCC_PIC16_FAMILY == 1802420) \
    || (__SDCC_PIC16_FAMILY == 1802480) \
    || (__SDCC_PIC16_FAMILY == 1822200) \
    || (__SDCC_PIC16_FAMILY == 1865850)
#define __SDCC_USART_STYLE  1822200

#elif  (__SDCC_PIC16_FAMILY == 1822210) \
    || (__SDCC_PIC16_FAMILY == 1823202) \
    || (__SDCC_PIC16_FAMILY == 1823222) \
    || (__SDCC_PIC16_FAMILY == 1823310) \
    || (__SDCC_PIC16_FAMILY == 1824100) \
    || (__SDCC_PIC16_FAMILY == 1824101) \
    || (__SDCC_PIC16_FAMILY == 1824200) \
    || (__SDCC_PIC16_FAMILY == 1824230) \
    || (__SDCC_PIC16_FAMILY == 1824550) \
    || (__SDCC_PIC16_FAMILY == 1825250)
#define __SDCC_USART_STYLE  1822210

#elif  (__SDCC_PIC16_FAMILY == 1824500) \
    || (__SDCC_PIC16_FAMILY == 1824800) \
    || (__SDCC_PIC16_FAMILY == 1825850) \
    || (__SDCC_PIC16_FAMILY == 1826820)
#define __SDCC_USART_STYLE  1824500

#elif  (__SDCC_PIC16_FAMILY == 1824501) \
    || (__SDCC_PIC16_FAMILY == 1865270) \
    || (__SDCC_PIC16_FAMILY == 1865501) \
    || (__SDCC_PIC16_FAMILY == 1866601)
#define __SDCC_USART_STYLE  1824501

#elif  (__SDCC_PIC16_FAMILY == 1865200)
#define __SDCC_USART_STYLE  1865200

#else
#warning No (E)USART style associated with the target device. Please update your pic16/pic18fam.h manually and/or inform the maintainer.
#endif

#endif /* !__SDCC_PIC18FAM_H__ */
