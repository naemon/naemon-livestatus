%define mod_path %{_libdir}/naemon-livestatus
%define nagios_cfg /opt/monitor/etc/nagios.cfg

%if 0%{?suse_version}
%define daemon_group www
%else
%define daemon_group apache
%endif

%if ! ( 0%{?rhel} > 5 )
%{!?python_sitelib: %global python_sitelib %(/usr/bin/python26 -c "from distutils.sysconfig import get_python_lib; print(get_python_lib())")}
%{!?python_sitearch: %global python_sitearch %(/usr/bin/python26 -c "from distutils.sysconfig import get_python_lib; print(get_python_lib(1))")}
%else
%{!?python_sitelib: %global python_sitelib %(%{__python} -c "from distutils.sysconfig import get_python_lib; print(get_python_lib())")}
%{!?python_sitearch: %global python_sitearch %(%{__python} -c "from distutils.sysconfig import get_python_lib; print(get_python_lib(1))")}
%endif

Summary: MK-Livestatus is a module for fetching status data from Nagios
Name: monitor-livestatus
Version: %{op5version}
Release: %{op5release}%{?dist}
License: GPLv2
Group: op5/Monitor
URL: http://www.op5.se
Source0: %name-%version.tar.gz
Prefix: /usr
Requires: monitor
%if ! 0%{?suse_version} && ! ( 0%{?rhel} > 5)
Requires: python26
Requires: libicu > 4.2
BuildRequires: python26
%else
Requires: python
BuildRequires: python
%endif
BuildRequires: libstdc++-devel
BuildRequires: gcc-c++
BuildRequires: pkgconfig
BuildRequires: libtool
BuildRequires: op5-naemon-devel
BuildRequires: autoconf, automake
BuildRequires: libicu-devel > 4.2
BuildRequires: cppunit-devel
BuildRequires: cppunit
BuildRoot: %{_tmppath}/%{name}-%{version}


%description
%name is an eventbroker module for op5 Monitor which allows
external programs to use the running Nagios daemon as a specialized
database. %name is a shallow fork of mk-livestatus by
Mathias Kettner.


%package -n unixcat
Summary: unixcat is a utility used to send data to a unix domain socket
Group: op5/system-addons

%description -n unixcat

%prep
%setup -q


%build
rm -rf %buildroot
autoreconf -is
%configure --with-naemon-config-dir=/opt/monitor/etc/mconf
%__make
LC_ALL=en_US.utf-8 %__make check


%install
%__make install DESTDIR=%buildroot

install -d %buildroot%{python_sitelib}/livestatus
install -pm 0644 api/python/livestatus.py %buildroot%{python_sitelib}/livestatus/
install -pm 0644 api/python/__init__.py %buildroot%{python_sitelib}/livestatus/

%post
# delete livestatus from main config, it's in a mconf dir
if grep -q 'broker_module.*livestatus\.' %nagios_cfg; then
	sed --follow-symlinks -i "/broker_module=.*livestatus\./d" %nagios_cfg
fi
# we must return/exit with true since yum complains otherwise:
test -f /etc/init.d/monitor && /etc/init.d/monitor restart || :


%postun
# we must return/exit with true since yum complains otherwise:
test -f /etc/init.d/monitor && /etc/init.d/monitor restart || :


%files
%defattr(-,root,root)
%mod_path
%mod_path/livestatus.so
%mod_path/livestatus.la
%{python_sitelib}/livestatus
%attr(755,monitor,%daemon_group) %config(noreplace) /opt/monitor/etc/mconf/livestatus.cfg


%files -n unixcat
%defattr(755,root,root)
%_bindir/unixcat


%clean
rm -rf %buildroot

%changelog
* Tue Jun 14 2011 Andreas Ericsson <ae@op5.se>
- Initial specfile creation.
