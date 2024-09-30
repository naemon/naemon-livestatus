# +------------------------------------------------------------------+
# |             ____ _               _        __  __ _  __           |
# |            / ___| |__   ___  ___| | __   |  \/  | |/ /           |
# |           | |   | '_ \ / _ \/ __| |/ /   | |\/| | ' /            |
# |           | |___| | | |  __/ (__|   <    | |  | | . \            |
# |            \____|_| |_|\___|\___|_|\_\___|_|  |_|_|\_\           |
# |                                                                  |
# | Copyright Mathias Kettner 2012             mk@mathias-kettner.de |
# +------------------------------------------------------------------+
#
# This file is part of Check_MK.
# The official homepage is at http://mathias-kettner.de/check_mk.
#
# check_mk is free software;  you can redistribute it and/or modify it
# under the  terms of the  GNU General Public License  as published by
# the Free Software Foundation in version 2.  check_mk is  distributed
# in the hope that it will be useful, but WITHOUT ANY WARRANTY;  with-
# out even the implied warranty of  MERCHANTABILITY  or  FITNESS FOR A
# PARTICULAR PURPOSE. See the  GNU General Public License for more de-
# ails.  You should have  received  a copy of the  GNU  General Public
# License along with GNU Make; see the file  COPYING.  If  not,  write
# to the Free Software Foundation, Inc., 51 Franklin St,  Fifth Floor,
# Boston, MA 02110-1301 USA.

ACLOCAL_AMFLAGS = -I m4
SUBDIRS = src tests

EXTRA_DIST = naemon-livestatus.spec \
             naemon-livestatus.rpmlintrc \
             api \
             debian \
             version.sh \
             livestatus.cfg.in

naemonconf_DATA = livestatus.cfg
livestatus.cfg : % : %.in
	$(AM_V_GEN) sed \
		-e 's,@pkglibdir[@],$(pkglibdir),g' \
		-e 's,@brokeroptions[@],@brokeroptions@,g' \
		$< > $@

rpm: dist
	rpmbuild -tb naemon-livestatus-${VERSION}.tar.gz

deb:
	debuild -i -us -uc -b

version:
	[ -e .git ] || { echo "changing versions only works in git clones!"; exit 1; }
	[ `git status | grep -cP 'working (directory|tree) clean'` -eq 1 ] || { echo "git project is not clean, cannot tag version"; exit 1; }
	OLDVERSION=$(shell grep ^VERSION version.sh | awk -F = '{ print $$2}'); \
	NEWVERSION=$$(dialog --stdout --inputbox "New Version:" 0 0 "$$OLDVERSION"); \
		if [ -n "$$NEWVERSION" ] && [ "$$NEWVERSION" != "$$OLDVERSION" ]; then \
			sed -ri "s/$$OLDVERSION/$$NEWVERSION/" version.sh naemon-livestatus.spec; \
			sed -e 's/UNRELEASED/unstable/g' -i debian/changelog; \
			DEBFULLNAME="Naemon Development Team" DEBEMAIL="Naemon Development <naemon-dev@monitoring-lists.org>" dch --newversion "$$NEWVERSION" --package "naemon-livestatus" -D "UNRELEASED" --urgency "low" "new upstream release"; \
			sed -e 's/unstable/UNRELEASED/g' -i debian/changelog; \
		fi

