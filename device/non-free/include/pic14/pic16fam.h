/*
 * pic16fam.h - PIC14 families
 *
 * This file is has been generated using pic16fam-h-gen.pl .
 */
#ifndef __SDCC_PIC16FAM_H__
#define __SDCC_PIC16FAM_H__ 1

/*
 * Define device class.
 */
#undef  __SDCC_PIC14_ENHANCED

#if   defined(__SDCC_PIC12F1822) || \
      defined(__SDCC_PIC12F1840) || \
      defined(__SDCC_PIC16F1503) || \
      defined(__SDCC_PIC16F1507) || \
      defined(__SDCC_PIC16F1508) || \
      defined(__SDCC_PIC16F1509) || \
      defined(__SDCC_PIC16F1516) || \
      defined(__SDCC_PIC16F1517) || \
      defined(__SDCC_PIC16F1518) || \
      defined(__SDCC_PIC16F1519) || \
      defined(__SDCC_PIC16F1526) || \
      defined(__SDCC_PIC16F1527) || \
      defined(__SDCC_PIC16F1782) || \
      defined(__SDCC_PIC16F1783) || \
      defined(__SDCC_PIC16F1823) || \
      defined(__SDCC_PIC16F1824) || \
      defined(__SDCC_PIC16F1825) || \
      defined(__SDCC_PIC16F1826) || \
      defined(__SDCC_PIC16F1827) || \
      defined(__SDCC_PIC16F1828) || \
      defined(__SDCC_PIC16F1829) || \
      defined(__SDCC_PIC16F1847) || \
      defined(__SDCC_PIC16F1933) || \
      defined(__SDCC_PIC16F1934) || \
      defined(__SDCC_PIC16F1936) || \
      defined(__SDCC_PIC16F1937) || \
      defined(__SDCC_PIC16F1938) || \
      defined(__SDCC_PIC16F1939) || \
      defined(__SDCC_PIC16F1946) || \
      defined(__SDCC_PIC16F1947) || \
      defined(__SDCC_PIC16LF1902) || \
      defined(__SDCC_PIC16LF1903) || \
      defined(__SDCC_PIC16LF1904) || \
      defined(__SDCC_PIC16LF1906) || \
      defined(__SDCC_PIC16LF1907)
#define __SDCC_PIC14_ENHANCED 1

#endif

/*
 * Define ADC style per device family.
 */
#undef  __SDCC_ADC_STYLE

#if   defined(__SDCC_PIC16C62) || \
      defined(__SDCC_PIC16C63A) || \
      defined(__SDCC_PIC16C65B) || \
      defined(__SDCC_PIC16F627) || \
      defined(__SDCC_PIC16F627A) || \
      defined(__SDCC_PIC16F628) || \
      defined(__SDCC_PIC16F628A) || \
      defined(__SDCC_PIC16F648A) || \
      defined(__SDCC_PIC16F87)
#define __SDCC_ADC_STYLE      0

#elif defined(__SDCC_PIC12F675) || \
      defined(__SDCC_PIC12F683)
#define __SDCC_ADC_STYLE      1206751

#elif defined(__SDCC_PIC12F1822) || \
      defined(__SDCC_PIC12F1840)
#define __SDCC_ADC_STYLE      1218221

#elif defined(__SDCC_PIC16C71) || \
      defined(__SDCC_PIC16C710) || \
      defined(__SDCC_PIC16C711)
#define __SDCC_ADC_STYLE      1600711

#elif defined(__SDCC_PIC16C72) || \
      defined(__SDCC_PIC16C73B) || \
      defined(__SDCC_PIC16F72) || \
      defined(__SDCC_PIC16F73) || \
      defined(__SDCC_PIC16F76)
#define __SDCC_ADC_STYLE      1600721

#elif defined(__SDCC_PIC16C74B) || \
      defined(__SDCC_PIC16F74) || \
      defined(__SDCC_PIC16F77)
#define __SDCC_ADC_STYLE      1600741

#elif defined(__SDCC_PIC16F88)
#define __SDCC_ADC_STYLE      1600881

#elif defined(__SDCC_PIC16C433)
#define __SDCC_ADC_STYLE      1604331

#elif defined(__SDCC_PIC16F616) || \
      defined(__SDCC_PIC16HV616)
#define __SDCC_ADC_STYLE      1606161

#elif defined(__SDCC_PIC16F676) || \
      defined(__SDCC_PIC16F684) || \
      defined(__SDCC_PIC16F688)
#define __SDCC_ADC_STYLE      1606761

#elif defined(__SDCC_PIC16F685) || \
      defined(__SDCC_PIC16F687) || \
      defined(__SDCC_PIC16F689) || \
      defined(__SDCC_PIC16F690)
#define __SDCC_ADC_STYLE      1606851

#elif defined(__SDCC_PIC16C715)
#define __SDCC_ADC_STYLE      1607151

#elif defined(__SDCC_PIC16F716)
#define __SDCC_ADC_STYLE      1607161

#elif defined(__SDCC_PIC16C717) || \
      defined(__SDCC_PIC16C770) || \
      defined(__SDCC_PIC16C771)
#define __SDCC_ADC_STYLE      1607171

#elif defined(__SDCC_PIC16F720) || \
      defined(__SDCC_PIC16F721)
#define __SDCC_ADC_STYLE      1607201

#elif defined(__SDCC_PIC16F737) || \
      defined(__SDCC_PIC16F767)
#define __SDCC_ADC_STYLE      1607371

#elif defined(__SDCC_PIC16C745)
#define __SDCC_ADC_STYLE      1607451

#elif defined(__SDCC_PIC16F747) || \
      defined(__SDCC_PIC16F777)
#define __SDCC_ADC_STYLE      1607471

#elif defined(__SDCC_PIC16C765)
#define __SDCC_ADC_STYLE      1607651

#elif defined(__SDCC_PIC16C773)
#define __SDCC_ADC_STYLE      1607731

#elif defined(__SDCC_PIC16C774)
#define __SDCC_ADC_STYLE      1607741

#elif defined(__SDCC_PIC16C781) || \
      defined(__SDCC_PIC16C782)
#define __SDCC_ADC_STYLE      1607811

#elif defined(__SDCC_PIC16F785)
#define __SDCC_ADC_STYLE      1607851

#elif defined(__SDCC_PIC16F818) || \
      defined(__SDCC_PIC16F819)
#define __SDCC_ADC_STYLE      1608181

#elif defined(__SDCC_PIC16F871) || \
      defined(__SDCC_PIC16F874) || \
      defined(__SDCC_PIC16F874A) || \
      defined(__SDCC_PIC16F877) || \
      defined(__SDCC_PIC16F877A)
#define __SDCC_ADC_STYLE      1608711

#elif defined(__SDCC_PIC16F882) || \
      defined(__SDCC_PIC16F883) || \
      defined(__SDCC_PIC16F886)
#define __SDCC_ADC_STYLE      1608821

#elif defined(__SDCC_PIC16F884) || \
      defined(__SDCC_PIC16F887)
#define __SDCC_ADC_STYLE      1608841

#elif defined(__SDCC_PIC16F913) || \
      defined(__SDCC_PIC16F916)
#define __SDCC_ADC_STYLE      1609131

#elif defined(__SDCC_PIC16F914) || \
      defined(__SDCC_PIC16F917) || \
      defined(__SDCC_PIC16F946)
#define __SDCC_ADC_STYLE      1609141

#elif defined(__SDCC_PIC16C925) || \
      defined(__SDCC_PIC16C926) || \
      defined(__SDCC_PIC16F870) || \
      defined(__SDCC_PIC16F872) || \
      defined(__SDCC_PIC16F873) || \
      defined(__SDCC_PIC16F873A) || \
      defined(__SDCC_PIC16F876) || \
      defined(__SDCC_PIC16F876A)
#define __SDCC_ADC_STYLE      1609251

#elif defined(__SDCC_PIC16F1503)
#define __SDCC_ADC_STYLE      1615031

#elif defined(__SDCC_PIC16F1507) || \
      defined(__SDCC_PIC16F1508) || \
      defined(__SDCC_PIC16F1509)
#define __SDCC_ADC_STYLE      1615071

#elif defined(__SDCC_PIC16F1516) || \
      defined(__SDCC_PIC16F1518)
#define __SDCC_ADC_STYLE      1615161

#elif defined(__SDCC_PIC16F1517) || \
      defined(__SDCC_PIC16F1519)
#define __SDCC_ADC_STYLE      1615171

#elif defined(__SDCC_PIC16F1526) || \
      defined(__SDCC_PIC16F1527)
#define __SDCC_ADC_STYLE      1615261

#elif defined(__SDCC_PIC16F1782) || \
      defined(__SDCC_PIC16F1783)
#define __SDCC_ADC_STYLE      1617821

#elif defined(__SDCC_PIC16F1823)
#define __SDCC_ADC_STYLE      1618231

#elif defined(__SDCC_PIC16F1824) || \
      defined(__SDCC_PIC16F1825)
#define __SDCC_ADC_STYLE      1618241

#elif defined(__SDCC_PIC16F1826) || \
      defined(__SDCC_PIC16F1827) || \
      defined(__SDCC_PIC16F1847)
#define __SDCC_ADC_STYLE      1618261

#elif defined(__SDCC_PIC16F1828) || \
      defined(__SDCC_PIC16F1829)
#define __SDCC_ADC_STYLE      1618281

#elif defined(__SDCC_PIC16LF1902) || \
      defined(__SDCC_PIC16LF1903) || \
      defined(__SDCC_PIC16LF1906)
#define __SDCC_ADC_STYLE      1619021

#elif defined(__SDCC_PIC16LF1904) || \
      defined(__SDCC_PIC16LF1907)
#define __SDCC_ADC_STYLE      1619041

#elif defined(__SDCC_PIC16F1933) || \
      defined(__SDCC_PIC16F1936) || \
      defined(__SDCC_PIC16F1938)
#define __SDCC_ADC_STYLE      1619331

#elif defined(__SDCC_PIC16F1934) || \
      defined(__SDCC_PIC16F1937) || \
      defined(__SDCC_PIC16F1939)
#define __SDCC_ADC_STYLE      1619341

#elif defined(__SDCC_PIC16F1946) || \
      defined(__SDCC_PIC16F1947)
#define __SDCC_ADC_STYLE      1619461

#else
#warning No ADC style associated with the target device. Please update your pic14/pic16fam.h manually and/or inform the maintainer.
#endif

/*
 * Define (E)USART style per device family.
 */
#undef  __SDCC_USART_STYLE

#if   defined(__SDCC_PIC12F675) || \
      defined(__SDCC_PIC12F683) || \
      defined(__SDCC_PIC16C433) || \
      defined(__SDCC_PIC16C62) || \
      defined(__SDCC_PIC16C71) || \
      defined(__SDCC_PIC16C710) || \
      defined(__SDCC_PIC16C711) || \
      defined(__SDCC_PIC16C715) || \
      defined(__SDCC_PIC16C717) || \
      defined(__SDCC_PIC16C72) || \
      defined(__SDCC_PIC16C770) || \
      defined(__SDCC_PIC16C771) || \
      defined(__SDCC_PIC16C781) || \
      defined(__SDCC_PIC16C782) || \
      defined(__SDCC_PIC16C925) || \
      defined(__SDCC_PIC16C926) || \
      defined(__SDCC_PIC16F1503) || \
      defined(__SDCC_PIC16F1507) || \
      defined(__SDCC_PIC16F616) || \
      defined(__SDCC_PIC16F676) || \
      defined(__SDCC_PIC16F684) || \
      defined(__SDCC_PIC16F685) || \
      defined(__SDCC_PIC16F716) || \
      defined(__SDCC_PIC16F72) || \
      defined(__SDCC_PIC16F785) || \
      defined(__SDCC_PIC16F818) || \
      defined(__SDCC_PIC16F819) || \
      defined(__SDCC_PIC16F872) || \
      defined(__SDCC_PIC16HV616) || \
      defined(__SDCC_PIC16LF1902) || \
      defined(__SDCC_PIC16LF1903)
#define __SDCC_USART_STYLE    0

#elif defined(__SDCC_PIC12F1822) || \
      defined(__SDCC_PIC12F1840)
#define __SDCC_USART_STYLE    1218222

#elif defined(__SDCC_PIC16C63A) || \
      defined(__SDCC_PIC16C65B) || \
      defined(__SDCC_PIC16C73B) || \
      defined(__SDCC_PIC16C745) || \
      defined(__SDCC_PIC16C74B) || \
      defined(__SDCC_PIC16C765) || \
      defined(__SDCC_PIC16F73) || \
      defined(__SDCC_PIC16F74) || \
      defined(__SDCC_PIC16F76) || \
      defined(__SDCC_PIC16F77)
#define __SDCC_USART_STYLE    1600632

#elif defined(__SDCC_PIC16F87) || \
      defined(__SDCC_PIC16F88)
#define __SDCC_USART_STYLE    1600872

#elif defined(__SDCC_PIC16F627) || \
      defined(__SDCC_PIC16F627A) || \
      defined(__SDCC_PIC16F628) || \
      defined(__SDCC_PIC16F628A) || \
      defined(__SDCC_PIC16F648A)
#define __SDCC_USART_STYLE    1606272

#elif defined(__SDCC_PIC16F687) || \
      defined(__SDCC_PIC16F689) || \
      defined(__SDCC_PIC16F690)
#define __SDCC_USART_STYLE    1606872

#elif defined(__SDCC_PIC16F688)
#define __SDCC_USART_STYLE    1606882

#elif defined(__SDCC_PIC16F720) || \
      defined(__SDCC_PIC16F721)
#define __SDCC_USART_STYLE    1607202

#elif defined(__SDCC_PIC16C773) || \
      defined(__SDCC_PIC16C774) || \
      defined(__SDCC_PIC16F737) || \
      defined(__SDCC_PIC16F747) || \
      defined(__SDCC_PIC16F767) || \
      defined(__SDCC_PIC16F777) || \
      defined(__SDCC_PIC16F870) || \
      defined(__SDCC_PIC16F871) || \
      defined(__SDCC_PIC16F873) || \
      defined(__SDCC_PIC16F873A) || \
      defined(__SDCC_PIC16F874) || \
      defined(__SDCC_PIC16F874A) || \
      defined(__SDCC_PIC16F876) || \
      defined(__SDCC_PIC16F876A) || \
      defined(__SDCC_PIC16F877) || \
      defined(__SDCC_PIC16F877A) || \
      defined(__SDCC_PIC16F913) || \
      defined(__SDCC_PIC16F914) || \
      defined(__SDCC_PIC16F916) || \
      defined(__SDCC_PIC16F917) || \
      defined(__SDCC_PIC16F946)
#define __SDCC_USART_STYLE    1607732

#elif defined(__SDCC_PIC16F882) || \
      defined(__SDCC_PIC16F883) || \
      defined(__SDCC_PIC16F884) || \
      defined(__SDCC_PIC16F886) || \
      defined(__SDCC_PIC16F887)
#define __SDCC_USART_STYLE    1608822

#elif defined(__SDCC_PIC16F1508) || \
      defined(__SDCC_PIC16F1509)
#define __SDCC_USART_STYLE    1615082

#elif defined(__SDCC_PIC16F1516) || \
      defined(__SDCC_PIC16F1517) || \
      defined(__SDCC_PIC16F1518) || \
      defined(__SDCC_PIC16F1519) || \
      defined(__SDCC_PIC16F1933) || \
      defined(__SDCC_PIC16F1934) || \
      defined(__SDCC_PIC16F1936) || \
      defined(__SDCC_PIC16F1937) || \
      defined(__SDCC_PIC16F1938) || \
      defined(__SDCC_PIC16F1939) || \
      defined(__SDCC_PIC16LF1904) || \
      defined(__SDCC_PIC16LF1906) || \
      defined(__SDCC_PIC16LF1907)
#define __SDCC_USART_STYLE    1615162

#elif defined(__SDCC_PIC16F1526) || \
      defined(__SDCC_PIC16F1527)
#define __SDCC_USART_STYLE    1615262

#elif defined(__SDCC_PIC16F1782) || \
      defined(__SDCC_PIC16F1783)
#define __SDCC_USART_STYLE    1617822

#elif defined(__SDCC_PIC16F1823) || \
      defined(__SDCC_PIC16F1824) || \
      defined(__SDCC_PIC16F1825)
#define __SDCC_USART_STYLE    1618232

#elif defined(__SDCC_PIC16F1826) || \
      defined(__SDCC_PIC16F1827) || \
      defined(__SDCC_PIC16F1847)
#define __SDCC_USART_STYLE    1618262

#elif defined(__SDCC_PIC16F1828) || \
      defined(__SDCC_PIC16F1829)
#define __SDCC_USART_STYLE    1618282

#elif defined(__SDCC_PIC16F1946) || \
      defined(__SDCC_PIC16F1947)
#define __SDCC_USART_STYLE    1619462

#else
#warning No (E)USART style associated with the target device. Please update your pic14/pic16fam.h manually and/or inform the maintainer.
#endif

/*
 * Define SSP style per device family.
 */
#undef  __SDCC_SSP_STYLE

#if   defined(__SDCC_PIC12F675) || \
      defined(__SDCC_PIC12F683) || \
      defined(__SDCC_PIC16C433) || \
      defined(__SDCC_PIC16C71) || \
      defined(__SDCC_PIC16C710) || \
      defined(__SDCC_PIC16C711) || \
      defined(__SDCC_PIC16C715) || \
      defined(__SDCC_PIC16C745) || \
      defined(__SDCC_PIC16C765) || \
      defined(__SDCC_PIC16C781) || \
      defined(__SDCC_PIC16C782) || \
      defined(__SDCC_PIC16F1507) || \
      defined(__SDCC_PIC16F616) || \
      defined(__SDCC_PIC16F627) || \
      defined(__SDCC_PIC16F627A) || \
      defined(__SDCC_PIC16F628) || \
      defined(__SDCC_PIC16F628A) || \
      defined(__SDCC_PIC16F648A) || \
      defined(__SDCC_PIC16F676) || \
      defined(__SDCC_PIC16F684) || \
      defined(__SDCC_PIC16F685) || \
      defined(__SDCC_PIC16F688) || \
      defined(__SDCC_PIC16F716) || \
      defined(__SDCC_PIC16F785) || \
      defined(__SDCC_PIC16F870) || \
      defined(__SDCC_PIC16F871) || \
      defined(__SDCC_PIC16HV616) || \
      defined(__SDCC_PIC16LF1902) || \
      defined(__SDCC_PIC16LF1903) || \
      defined(__SDCC_PIC16LF1904) || \
      defined(__SDCC_PIC16LF1906) || \
      defined(__SDCC_PIC16LF1907)
#define __SDCC_SSP_STYLE      0

#elif defined(__SDCC_PIC12F1822) || \
      defined(__SDCC_PIC12F1840)
#define __SDCC_SSP_STYLE      1218223

#elif defined(__SDCC_PIC16C62) || \
      defined(__SDCC_PIC16C72)
#define __SDCC_SSP_STYLE      1600623

#elif defined(__SDCC_PIC16C63A) || \
      defined(__SDCC_PIC16C65B) || \
      defined(__SDCC_PIC16C73B) || \
      defined(__SDCC_PIC16C74B)
#define __SDCC_SSP_STYLE      1600633

#elif defined(__SDCC_PIC16F72) || \
      defined(__SDCC_PIC16F73) || \
      defined(__SDCC_PIC16F74) || \
      defined(__SDCC_PIC16F76) || \
      defined(__SDCC_PIC16F77)
#define __SDCC_SSP_STYLE      1600723

#elif defined(__SDCC_PIC16F818) || \
      defined(__SDCC_PIC16F819) || \
      defined(__SDCC_PIC16F87) || \
      defined(__SDCC_PIC16F88)
#define __SDCC_SSP_STYLE      1600873

#elif defined(__SDCC_PIC16F687) || \
      defined(__SDCC_PIC16F689) || \
      defined(__SDCC_PIC16F690) || \
      defined(__SDCC_PIC16F720) || \
      defined(__SDCC_PIC16F721)
#define __SDCC_SSP_STYLE      1606873

#elif defined(__SDCC_PIC16C717) || \
      defined(__SDCC_PIC16C770) || \
      defined(__SDCC_PIC16C771)
#define __SDCC_SSP_STYLE      1607173

#elif defined(__SDCC_PIC16F737) || \
      defined(__SDCC_PIC16F747) || \
      defined(__SDCC_PIC16F767) || \
      defined(__SDCC_PIC16F777) || \
      defined(__SDCC_PIC16F872) || \
      defined(__SDCC_PIC16F873) || \
      defined(__SDCC_PIC16F873A) || \
      defined(__SDCC_PIC16F874) || \
      defined(__SDCC_PIC16F874A) || \
      defined(__SDCC_PIC16F876) || \
      defined(__SDCC_PIC16F876A) || \
      defined(__SDCC_PIC16F877) || \
      defined(__SDCC_PIC16F877A) || \
      defined(__SDCC_PIC16F882) || \
      defined(__SDCC_PIC16F883) || \
      defined(__SDCC_PIC16F884) || \
      defined(__SDCC_PIC16F886) || \
      defined(__SDCC_PIC16F887)
#define __SDCC_SSP_STYLE      1607373

#elif defined(__SDCC_PIC16C773) || \
      defined(__SDCC_PIC16C774)
#define __SDCC_SSP_STYLE      1607733

#elif defined(__SDCC_PIC16F913) || \
      defined(__SDCC_PIC16F914) || \
      defined(__SDCC_PIC16F916) || \
      defined(__SDCC_PIC16F917) || \
      defined(__SDCC_PIC16F946)
#define __SDCC_SSP_STYLE      1609133

#elif defined(__SDCC_PIC16C925) || \
      defined(__SDCC_PIC16C926)
#define __SDCC_SSP_STYLE      1609253

#elif defined(__SDCC_PIC16F1503)
#define __SDCC_SSP_STYLE      1615033

#elif defined(__SDCC_PIC16F1508) || \
      defined(__SDCC_PIC16F1509)
#define __SDCC_SSP_STYLE      1615083

#elif defined(__SDCC_PIC16F1516) || \
      defined(__SDCC_PIC16F1517) || \
      defined(__SDCC_PIC16F1518) || \
      defined(__SDCC_PIC16F1519)
#define __SDCC_SSP_STYLE      1615173

#elif defined(__SDCC_PIC16F1526) || \
      defined(__SDCC_PIC16F1527)
#define __SDCC_SSP_STYLE      1615263

#elif defined(__SDCC_PIC16F1782) || \
      defined(__SDCC_PIC16F1783)
#define __SDCC_SSP_STYLE      1617823

#elif defined(__SDCC_PIC16F1823) || \
      defined(__SDCC_PIC16F1824) || \
      defined(__SDCC_PIC16F1825)
#define __SDCC_SSP_STYLE      1618233

#elif defined(__SDCC_PIC16F1826)
#define __SDCC_SSP_STYLE      1618263

#elif defined(__SDCC_PIC16F1827) || \
      defined(__SDCC_PIC16F1847)
#define __SDCC_SSP_STYLE      1618273

#elif defined(__SDCC_PIC16F1828)
#define __SDCC_SSP_STYLE      1618283

#elif defined(__SDCC_PIC16F1829)
#define __SDCC_SSP_STYLE      1618293

#elif defined(__SDCC_PIC16F1933) || \
      defined(__SDCC_PIC16F1934) || \
      defined(__SDCC_PIC16F1936) || \
      defined(__SDCC_PIC16F1937) || \
      defined(__SDCC_PIC16F1938) || \
      defined(__SDCC_PIC16F1939)
#define __SDCC_SSP_STYLE      1619333

#elif defined(__SDCC_PIC16F1946) || \
      defined(__SDCC_PIC16F1947)
#define __SDCC_SSP_STYLE      1619463

#else
#warning No SSP style associated with the target device. Please update your pic14/pic16fam.h manually and/or inform the maintainer.
#endif

#endif /* !__SDCC_PIC16FAM_H__ */
