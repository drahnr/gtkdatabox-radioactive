Summary:    GTK+ widget for fast data display
Name:       gtkdatabox
Version:    0.9.2.0
Release:    1
License:    LGPL
Group:      System Environment/Libraries
URL:        http://www.eudoxos.net/gtk/gtkdatabox/
Source:     %{name}-%{version}.tar.gz
BuildRoot:  %{_tmppath}/%{name}-%{version}-root

Requires:      gtk3
BuildRequires: gtk3-devel

%description
GtkDatabox is a widget for the GTK+ library designed to display
large amounts of numerical data fast and easy.

%package devel
Summary:    Development headers and libraries for gtkdatabox
Group:      Development/Libraries
Requires:   gtkdatabox = %{version}, gtk3-devel, pkgconfig

%description devel
The development headers and libraries needed to develop
applications which use the gtkdatabox widget.

%prep
%setup -q

%build
%configure
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
make DESTDIR=$RPM_BUILD_ROOT install
rm -f $RPM_BUILD_ROOT%{_libdir}/*.a
rm -f $RPM_BUILD_ROOT%{_libdir}/*.la

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc AUTHORS ChangeLog COPYING INSTALL NEWS README TODO
%{_libdir}/*.so*

%files devel
%defattr(-,root,root)
%{_includedir}/*.h
%{_libdir}/pkgconfig/*.pc

%changelog
* Sat Jan 28 2006 Eric Work <work.eric@gmail.com>
- Initial release
* Wed Mar 08 2006 Roland Bock
- Minor adjustments
