#
# spec file for package sdcc
#

Name:          sdcc
Requires:      /bin/sh
License:       GPL
Group:         Applications/Engineering
Summary:       Small Device C Compiler
Requires:      sdcc-common
Version:       2.6.4
Release:       2
Source:        %{name}-src-%{version}.tar.gz
URL:           http://sdcc.sourceforge.net/
Packager:      Bernhard Held <bernhard AT bernhardheld.de>
BuildRoot:     %{_tmppath}/%{name}-%{version}-build

%description
SDCC is a C compiler for 8051 class and similar microcontrollers.
The package includes the compiler, assemblers and linkers, a device
simulator and a core library. The processors supported (to a varying
degree) include the 8051, ds390, z80, hc08, and PIC.

%package common
License:       GPL, LGPL
Group:         Application/Engineering
Summary:       Libraries and Header Files for the SDCC C compiler
Requires:      sdcc

%description common
SDCC is a C compiler for 8051 class and similar microcontrollers.
The package includes the compiler, assemblers and linkers, a device
simulator and a core library. The processors supported (to a varying
degree) include the 8051, ds390, z80, hc08, and PIC.

%package doc
License:       GPL
Group:         Applications/Engineering
Summary:       Documentation for the SDCC C compiler

%description doc
SDCC is a C compiler for 8051 class and similar microcontrollers.
The package includes the compiler, assemblers and linkers, a device
simulator and a core library. The processors supported (to a varying
degree) include the 8051, ds390, z80, hc08, and PIC.

%prep
%setup -n sdcc

%build
./configure CFLAGS="$RPM_OPT_FLAGS" --prefix=/usr --enable-doc docdir=$RPM_BUILD_ROOT/%{_defaultdocdir}/%{name}
make
cd doc
make
cd ..

%install
rm -rf $RPM_BUILD_ROOT
make prefix=$RPM_BUILD_ROOT/usr install
cd doc
make install
cd ..

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
/usr/bin

%files common
%defattr(-,root,root)
/usr/share/sdcc/include
/usr/share/sdcc/lib

%files doc
%defattr(-,root,root)
%doc %{_defaultdocdir}

%changelog
* Sun Dec 26 2006 - bernhard AT bernhardheld.de
- version updated
* Sun Sep 03 2006 - Christer Weinigel
- fixed build of doc
* Tue Mar 09 2004 - bernhard AT bernhardheld.de
- split into two packages
* Wed Feb 26 2004 - bernhard AT bernhardheld.de
- version updated
* Sun Jan 04 2004 - bernhard AT bernhardheld.de
- updated
* Sat Apr 27 2002 - steve AT icarus.com
- first version from Stephen Williams
