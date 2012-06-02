#!/usr/bin/perl -w

#
# This script generates a C header file that maps the target device (as
# indicated via the sdcc generated -Dpic16cxxx or -Dpic12fxxx or -Dpic16fxxx
# macro) to its device family and the device families to their respective
# style of ADC, USART and SSP programming for use in the SDCC PIC14 I/O library.
#
# Copyright 2010 Raphael Neider <rneider AT web.de>
# Modified to the pic14 series: Molnár Károly, 2012 <proton7@freemail.hu>
#
# This file is part of SDCC.
# 
# SDCC is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation, either version 2 of the License, or (at your
# option) any later version.
#
# SDCC is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License
# along with SDCC.  If not, see <http://www.gnu.org/licenses/>.
#

#
# Usage: perl pic16fam-h-gen.pl
#
# This will create pic16fam.h.gen in your current directory.
# Check sanity of the file and move it to .../include/pic14/pic16fam.h.
# If you assigned new I/O styles, implement them in
# .../include/pic14/{adc,i2c,usart}.h and
# .../lib/pic14/libio/*/*.c
#

use strict;

my $head = '__SDCC_PIC';

my %families = ();
my @enhanced = ();
my %adc = ();
my %usart = ();
my %ssp = ();
my $update = 'Please update your pic14/pic16fam.h manually and/or inform the maintainer.';

my $ref;

#-------------------------------------------------------------------------------

sub find_list($$)
  {
  my ($list, $name) = @_;

  foreach my $n (@{$list})
    {
    return 1 if ($n eq $name);
    }

  return 0;
  }

#-------------------------------------------------------------------------------

sub find_pic($$)
  {
  my ($hash, $name) = @_;

  for my $k (sort keys %{$hash})
    {
    if ($k != 0)
      {
      return 1 if (find_list(\@{${$hash}{$k}}, $name));
      }
    }

  return 0;
  }

#-------------------------------------------------------------------------------

sub array_filter($)
  {
  my $hash = $_[0];
  my $null = \@{${$hash}{0}};
  my @array = ();

  foreach my $n (@{$null})
    {
    if (! find_pic($hash, $n) && ! find_list(\@array, $n))
      {
      push(@array, $n);
      }
    }

  @{${$hash}{0}} = @array;
  }

#-------------------------------------------------------------------------------

while (<DATA>) {
    chomp;
    s/\s*#.*$//;                # remove comments
    s/\s*//g;                   # strip whitespace
    next if (/^\s*$/);          # ignore empty lines

    my $line = $_;

    my @fields = split(/:/, $line);

    die "Invalid record >$line<" if (scalar @fields != 6);

    my ($id, $memberlist, $enh, $adcstyle, $usartstyle, $sspstyle) = @fields;

    # extract numeric family id
    $id = 0+$id;

    # extract family members
    my @arr = split(/,/, $memberlist);
    @arr = sort(map { uc($_); } @arr);
    $families{$id} = \@arr;

    # enhanced class
    $enh = 0+$enh;
    if ($enh) {
        push @enhanced, @arr;
    }

    # ADC style per device family
    $adcstyle = 0+$adcstyle;
    push @{$adc{$adcstyle}}, @arr;

    # (E)USART style per device family
    $usartstyle = 0+$usartstyle;
    push @{$usart{$usartstyle}}, @arr;

    # SSP style per device family
    $sspstyle = 0+$sspstyle;
    push @{$ssp{$sspstyle}}, @arr;
}

array_filter(\%adc);
array_filter(\%usart);
array_filter(\%ssp);

my $fname = 'pic16fam.h.gen';
open(FH, '>', $fname) or die "Could not open >$fname<";

print FH <<EOT
/*
 * pic16fam.h - PIC14 families
 *
 * This file is has been generated using $0 .
 */
#ifndef __SDCC_PIC16FAM_H__
#define __SDCC_PIC16FAM_H__ 1

/*
 * Define device class.
 */
#undef  __SDCC_PIC14_ENHANCED

EOT
;

my $memb = "defined($head" . join(") || \\\n      defined($head", sort @enhanced) . ')';
print FH <<EOT
#if   ${memb}
#define __SDCC_PIC14_ENHANCED 1

#endif

/*
 * Define ADC style per device family.
 */
#undef  __SDCC_ADC_STYLE

EOT
;

my $pp = '#if   ';
for my $s (sort keys %adc) {
    my $fams = "defined($head" . join(") || \\\n      defined($head", sort @{$adc{$s}}) . ')';
    print FH <<EOT
${pp}${fams}
#define __SDCC_ADC_STYLE      ${s}

EOT
;
$pp = '#elif ';
} # for
print FH <<EOT
#else
#warning No ADC style associated with the target device. ${update}
#endif

/*
 * Define (E)USART style per device family.
 */
#undef  __SDCC_USART_STYLE

EOT
;

$pp = '#if   ';
for my $s (sort keys %usart) {
    my $fams = "defined($head" . join(") || \\\n      defined($head", sort @{$usart{$s}}) . ')';
    print FH <<EOT
${pp}${fams}
#define __SDCC_USART_STYLE    ${s}

EOT
;
$pp = '#elif ';
} # for
print FH <<EOT
#else
#warning No (E)USART style associated with the target device. ${update}
#endif

/*
 * Define SSP style per device family.
 */
#undef  __SDCC_SSP_STYLE

EOT
;

$pp = '#if   ';
for my $s (sort keys %ssp) {
    my $fams = "defined($head" . join(") || \\\n      defined($head", sort @{$ssp{$s}}) . ')';
    print FH <<EOT
${pp}${fams}
#define __SDCC_SSP_STYLE      ${s}

EOT
;
$pp = '#elif ';
} # for
print FH <<EOT
#else
#warning No SSP style associated with the target device. ${update}
#endif

#endif /* !__SDCC_PIC16FAM_H__ */
EOT
;
__END__
#
# <id>:<head>{,<member>}:<class>:<adc>:<usart>:<ssp>
#
# Each line provides a colon separated list of
#  * a numeric family name, derived from the first family member as follows:
#    enhanced MCU: 1<num1>(F|LF)<num2>?      -> printf("1%u%04u0", <num1>, <num2>)
#    ADC         : 1<num1>(C|F|HV|LF)<num2>? -> printf("1%u%04u1", <num1>, <num2>)
#    USART       : 1<num1>(C|F|HV|LF)<num2>? -> printf("1%u%04u2", <num1>, <num2>)
#    SSP         : 1<num1>(C|F|HV|LF)<num2>? -> printf("1%u%04u3", <num1>, <num2>)
#  * a comma-separated list of members of a device family,
#    where a family comprises all devices that share a single data sheet,
#  * the enhanced MCU (numeric family name or 0, if not applicable)
#  * the ADC style (numeric family name or 0, if not applicable)
#  * the USART style (numeric family name or 0, if not applicable)
#  * the SSP style (numeric family name or 0, if not applicable)
#
# This data has been gathered manually from data sheets published by
# Microchip Technology Inc.
#

# enhanced MCU
1218220:12f1822,12f1840:1218220:0:0:0
1615030:16f1503,16f1507,16f1508,16f1509,16f1516,16f1517,16f1518,16f1519,16f1526,16f1527:1615030:0:0:0
1617820:16f1782,16f1783:1617820:0:0:0
1618230:16f1823,16f1824,16f1825,16f1826,16f1827,16f1828,16f1829,16f1847:1618230:0:0:0
1619330:16f1933,16f1934,16f1936,16f1937,16f1938,16f1939,16f1946,16f1947:1619330:0:0:0
1619020:16lf1902,16lf1903,16lf1904,16lf1906,16lf1907:1619020:0:0:0

# ADC
1206751:12f675,12f683:0:1206751:0:0
1218221:12f1822,12f1840:0:1218221:0:0
1600711:16c71,16c710,16c711:0:1600711:0:0
1600721:16c72,16f72,16c73b,16f73,16f76:0:1600721:0:0
1600741:16c74b,16f74,16f77:0:1600741:0:0
1604331:16c433:0:1604331:0:0
1607151:16c715:0:1607151:0:0
1607171:16c717,16c770,16c771:0:1607171:0:0
1607451:16c745:0:1607451:0:0
1607651:16c765:0:1607651:0:0
1607731:16c773:0:1607731:0:0
1607741:16c774:0:1607741:0:0
1607811:16c781,16c782:0:1607811:0:0
1609251:16c925,16c926,16f870,16f872,16f873,16f873a,16f876,16f876a:0:1609251:0:0
1600881:16f88:0:1600881:0:0
1606161:16f616,16hv616:0:1606161:0:0
1606761:16f676,16f684:0:1606761:0:0
1606851:16f685,16f687,16f689,16f690:0:1606851:0:0
1607161:16f716:0:1607161:0:0
1607201:16f720,16f721:0:1607201:0:0
1607371:16f737,16f767:0:1607371:0:0
1607471:16f747,16f777:0:1607471:0:0
1607851:16f785:0:1607851:0:0
1608181:16f818,16f819:0:1608181:0:0
1608711:16f871,16f874,16f874a,16f877,16f877a:0:1608711:0:0
1608821:16f882,16f883,16f886:0:1608821:0:0
1608841:16f884,16f887:0:1608841:0:0
1609131:16f913,16f916:0:1609131:0:0
1609141:16f914,16f917,16f946:0:1609141:0:0
1615031:16f1503:0:1615031:0:0
1615071:16f1507,16f1508,16f1509:0:1615071:0:0
1615161:16f1516,16f1518:0:1615161:0:0
1615171:16f1517,16f1519:0:1615171:0:0
1615261:16f1526,16f1527:0:1615261:0:0
1617821:16f1782,16f1783:0:1617821:0:0
1618231:16f1823:0:1618231:0:0
1618241:16f1824,16f1825:0:1618241:0:0
1618261:16f1826,16f1827,16f1847:0:1618261:0:0
1618281:16f1828,16f1829:0:1618281:0:0
1619331:16f1933,16f1936,16f1938:0:1619331:0:0
1619341:16f1934,16f1937,16f1939:0:1619341:0:0
1619461:16f1946,16f1947:0:1619461:0:0
1619021:16lf1902,16lf1903,16lf1906:0:1619021:0:0
1619041:16lf1904,16lf1907:0:1619041:0:0

# (E)USART
1218222:12f1822,12f1840:0:0:1218222:0
1600632:16c63a,16c65b,16c73b,16c74b,16c745,16c765,16f73,16f74,16f76,16f77:0:0:1600632:0
1600872:16f87,16f88:0:0:1600872:0
1606272:16f627,16f627a,16f628,16f628a,16f648a:0:0:1606272:0
1606872:16f687,16f689,16f690:0:0:1606872:0
1606882:16f688:0:0:1606882:0
1607202:16f720,16f722:0:0:1607202:0
1607732:16c773,16c774,16f737,16f747,16f767,16f777,16f870,16f871,16f873,16f873a,16f874,16f874a,16f876,16f876a,16f877,16f877a,16f913,16f914,16f916,16f917,16f946:0:0:1607732:0
1608822:16f882,16f883,16f884,16f886,16f887:0:0:1608822:0
1615082:16f1508,16f1509:0:0:1615082:0
1615162:16f1516,16f1517,16f1518,16f1519,16f1933,16f1934,16f1936,16f1937,16f1938,16f1939,16lf1904,16lf1906,16lf1907:0:0:1615162:0
1615262:16f1526,16f1527:0:0:1615262:0
1617822:16f1782,16f1783:0:0:1617822:0
1618232:16f1823,16f1824,16f1825:0:0:1618232:0
1618262:16f1826,16f1827,16f1847:0:0:1618262:0
1618282:16f1828,16f1829:0:0:1618282:0
1619462:16f1946,16f1947:0:0:1619462:0

# SSP
1218223:12f1822,12f1840:0:0:0:1218223
1600623:16c62,16c72:0:0:0:1600623
1600633:16c63a,16c65b,16c73b,16c74b:0:0:0:1600633
1607173:16c717,16c770,16c771:0:0:0:1607173
1607733:16c773,16c774:0:0:0:1607733
1609253:16c925,16c926:0:0:0:1609253
1600723:16f72,16f73,16f74,16f76,16f77:0:0:0:1600723
1600873:16f87,16f88,16f818,16f819:0:0:0:1600873
1606873:16f687,16f689,16f690,16f720,16f721:0:0:0:1606873
1607373:16f737,16f747,16f767,16f777,16f872,16f873,16f873a,16f874,16f874a,16f876,16f876a,16f877,16f877a,16f882,16f883,16f884,16f886,16f887:0:0:0:1607373
1609133:16f913,16f914,16f916,16f917,16f946:0:0:0:1609133
1615033:16f1503:0:0:0:1615033
1615083:16f1508,16f1509:0:0:0:1615083
1615173:16f1516,16f1517,16f1518,16f1519:0:0:0:1615173
1615263:16f1526,16f1527:0:0:0:1615263
1617823:16f1782,16f1783:0:0:0:1617823
1618233:16f1823,16f1824,16f1825:0:0:0:1618233
1618263:16f1826:0:0:0:1618263
1618273:16f1827,16f1847:0:0:0:1618273
1618283:16f1828:0:0:0:1618283
1618293:16f1829:0:0:0:1618293
1619333:16f1933,16f1934,16f1936,16f1937,16f1938,16f1939:0:0:0:1619333
1619463:16f1946,16f1947:0:0:0:1619463
