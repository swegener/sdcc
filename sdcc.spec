Summary: Small Device C Compiler
Name: sdcc
Version: 2.3.0
Release: 2
Copyright: GPL
Group: Applications/Engineering
Source: sdcc-2.3.0.tar.gz
URL: http://sdcc.sourceforge.net/
Packager: Stephen Williams <steve@icarus.com>

BuildRoot: /tmp/sdcc

%description
SDC is a C compiler for 8051 class and similar microcontrollers.
The packge includes the compiler, assemblers and linkers, a device
simulator and a core library. The processors supported (to a varying
degree) include the 8051, avr, ds390, PIC, and z80.

%prep
%setup -n sdcc

%build
./configure --prefix=/usr
make all

%install
make prefix=$RPM_BUILD_ROOT/usr  install
mv $RPM_BUILD_ROOT/usr/doc/ucsim $RPM_BUILD_ROOT/usr/share/doc

# These files need not be distributed.
rm -rf $RPM_BUILD_ROOT/usr/share/sdcc/lib/src/ds390/CVS
rm -rf $RPM_BUILD_ROOT/usr/share/sdcc/lib/src/gbz80/CVS
rm -rf $RPM_BUILD_ROOT/usr/share/sdcc/lib/src/z80/CVS

# These files freak out find-requires if left executable.
chmod -x $RPM_BUILD_ROOT/usr/share/sdcc/lib/src/ds390/Makefile
chmod -x $RPM_BUILD_ROOT/usr/share/sdcc/lib/src/gbz80/Makefile
chmod -x $RPM_BUILD_ROOT/usr/share/sdcc/lib/src/z80/Makefile

%files

%attr(-,root,root) /usr/bin/as-gbz80
%attr(-,root,root) /usr/bin/aslink
%attr(-,root,root) /usr/bin/asx8051
%attr(-,root,root) /usr/bin/as-z80
%attr(-,root,root) /usr/bin/link-gbz80
%attr(-,root,root) /usr/bin/link-z80
%attr(-,root,root) /usr/bin/packihx
%attr(-,root,root) /usr/bin/s51
%attr(-,root,root) /usr/bin/savr
%attr(-,root,root) /usr/bin/sdcc
%attr(-,root,root) /usr/bin/sdcdb
%attr(-,root,root) /usr/bin/sdcpp
%attr(-,root,root) /usr/bin/sdcppold
%attr(-,root,root) /usr/bin/serialview
%attr(-,root,root) /usr/bin/sz80

%attr(-,root,root) /usr/share/sdcc


%attr(-,root,root) %doc /usr/share/doc/sdcc
%attr(-,root,root) %doc /usr/share/doc/ucsim
