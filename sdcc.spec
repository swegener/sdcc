#
# spec file for package sdcc
#

Name:          sdcc
Requires:      /bin/sh
License:       GPL, LGPL
Group:         Applications/Engineering
Provides:      sdcc
Summary:       Small Device C Compiler
Version:       2.3.7
Release:       4
Source:        %{name}-%{version}.tar.bz2
URL:           http://sdcc.sourceforge.net/
Packager:      Bernhard Held <bernhard@bernhardheld.de>
BuildRoot:     %{_tmppath}/%{name}-%{version}-build

%description
SDCC is a C compiler for 8051 class and similar microcontrollers.
The packge includes the compiler, assemblers and linkers, a device
simulator and a core library. The processors supported (to a varying
degree) include the 8051, ds390, z80, hc08, and PIC.

%prep
%setup -n sdcc

%build
./configure CFLAGS="$RPM_OPT_FLAGS" --prefix=/usr docdir=$RPM_BUILD_ROOT/%{_defaultdocdir}/%{name}
make
cd doc
make
cd ..

%install
rm -rf $RPM_BUILD_ROOT/usr
make prefix=$RPM_BUILD_ROOT/usr install
cd doc
make install
cd ..

%files
%defattr(-,root,root)
/usr/bin
/usr/share/sdcc/include
/usr/share/sdcc/lib
%doc %{_defaultdocdir}

%changelog
* Sun Jan 04 2004 - bernhard@bernhardheld.de
- updated
* Sat Apr 27 2002 - steve@icarus.com
- first version from Stephen Williams
