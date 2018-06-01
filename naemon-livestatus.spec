%if ! ( 0%{?rhel} > 5 )
%{!?python_sitelib: %global python_sitelib %(/usr/bin/python26 -c "from distutils.sysconfig import get_python_lib; print(get_python_lib())")}
%{!?python_sitearch: %global python_sitearch %(/usr/bin/python26 -c "from distutils.sysconfig import get_python_lib; print(get_python_lib(1))")}
%else
%{!?python_sitelib: %global python_sitelib %(%{__python} -c "from distutils.sysconfig import get_python_lib; print(get_python_lib())")}
%{!?python_sitearch: %global python_sitearch %(%{__python} -c "from distutils.sysconfig import get_python_lib; print(get_python_lib(1))")}
%endif

Summary: Naemon Livestatus Eventbroker Module
Name: naemon-livestatus
Version: 1.0.6
Release: 0
License: GPLv2
Group: Applications/System
URL: http://www.naemon.org/
Packager: Naemon Core Development Team <naemon-dev@monitoring-lists.org>
Vendor: Naemon Core Development Team
Source0: http://labs.consol.de/naemon/download/naemon-livestatus-%{version}.tar.gz
BuildRoot: %{_tmppath}/naemon-%{version}-%{release}
BuildRequires: naemon-devel
BuildRequires: autoconf
BuildRequires: automake
BuildRequires: libtool
BuildRequires: gcc-c++
BuildRequires: python-devel


%description
Naemon-livestatus is an eventbroker module for naemon which allows
external programs to use the running Nagios daemon as a specialized
database.


%prep
%setup -q


%build
test -f configure || ./autogen.sh
%configure \
    --libdir="%{_libdir}/naemon" \
    --with-naemon-config-dir="/etc/naemon/module-conf.d"
%{__make}


%install
%{__rm} -rf %{buildroot}
%{__make} install DESTDIR="%{buildroot}"

# We don't really want to distribute this
rm -f %{buildroot}%{_libdir}/naemon/naemon-livestatus/livestatus.la

# Livestatus Python API
install -d %buildroot%{python_sitelib}/livestatus
install -pm 0755 api/python/livestatus.py %buildroot%{python_sitelib}/livestatus/
install -pm 0644 api/python/__init__.py %buildroot%{python_sitelib}/livestatus/

install -d %{buildroot}%{_localstatedir}/log/naemon

%clean
%{__rm} -rf %{buildroot}


%post
case "$*" in
  2)
    # Upgrading so try and restart if already running
    if [ -e /etc/naemon/naemon.cfg ]; then
      # livestatus configuration has been moved to single drop dir file
      sed -i /etc/naemon/naemon.cfg -e 's~^\s*\(broker_module=/usr/lib[0-9]*/naemon/naemon-livestatus/livestatus.so.*\)~#\1~'
    fi
  ;;
  1)
    # First installation, no acton required
    :
  ;;
  *) echo case "$*" not handled in postun
esac
%if 0%{?use_systemd}
  systemctl condrestart naemon.service
%else
  /etc/init.d/naemon condrestart &>/dev/null || :
%endif
exit 0

%preun
case "$*" in
  0)
    # POSTUN
    rm -f /var/log/naemon/livestatus.log
    ;;
  1)
    # POSTUPDATE
    ;;
  *) echo case "$*" not handled in postun
esac
exit 0

%postun
case "$*" in
  0)
    # POSTUN
    if [ -e /etc/naemon/naemon.cfg ]; then
      sed -i /etc/naemon/naemon.cfg -e 's~^\s*\(broker_module=/usr/lib[0-9]*/naemon/naemon-livestatus/livestatus.so.*\)~#\1~'
    fi
    rm -f /var/cache/naemon/live
    ;;
  1)
    # POSTUPDATE
    ;;
  *) echo case "$*" not handled in postun
esac
exit 0


%files
%attr(0755,root,root) %{_bindir}/unixcat
%attr(0755,naemon,naemon) %dir %{_libdir}/naemon/naemon-livestatus
%attr(0644,root,root) %{_libdir}/naemon/naemon-livestatus/livestatus.so
%attr(0755,naemon,naemon) %dir %{_localstatedir}/log/naemon
%attr(-,root,root) %{python_sitelib}/livestatus/
%if 0%{?suse_version} >= 1315
%attr(0755,naemon,naemon) %dir %{_sysconfdir}/naemon/
%attr(0755,naemon,naemon) %dir %{_sysconfdir}/naemon/module-conf.d/
%endif
%attr(0644,naemon,naemon) %config(noreplace) %{_sysconfdir}/naemon/module-conf.d/livestatus.cfg

%changelog
* Tue Sep 19 2017 Sven Nierlein <sven.nierlein@consol.de> 1.0.7-1
- Decouple core and livestatus
