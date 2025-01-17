# Copyright (c) 2025 Naemon team - license: GPL-2.0
# This file is part of the naemon project: https://www.naemon.io
# See LICENSE file in the project root for details.

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

LICENSE_HEADER = "// Copyright (c) 2025 Naemon team - license: GPL-2.0\n// This file is part of the naemon project: https://www.naemon.io\n// See LICENSE file in the project root for details.\n"
licenseheader:
	@for file in src/*.c src/*.h src/*.cc api/c++/*.cc api/c++/*.h; do \
		( \
			echo -e $(LICENSE_HEADER); \
			perl -ne 'if (/^\s*$$/ || /^\/\//) { next unless $$seen; } else { $$seen++; print; next; } print if $$seen;' $$file; \
		) \
		> $$file.tmp && mv $$file.tmp $$file; \
	done
