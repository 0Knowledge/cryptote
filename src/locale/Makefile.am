################################################################################
# src/locale/Makefile.am
#
# Based on wxWidgets-2.8.7/locale/Makefile
#
# Part of CryptoTE v0.5.999, see http://panthema.net/2007/cryptote
#
# Copyright (C) 2008-2014 Timo Bingmann <tb@panthema.net>
#
# This program is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation; either version 2 of the License, or (at your option) any later
# version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
# details.
#
# You should have received a copy of the GNU General Public License along with
# this program; if not, write to the Free Software Foundation, Inc., 59 Temple
# Place, Suite 330, Boston, MA 02111-1307 USA
################################################################################

SUBDIRS = wxstd

LINGUAS = de

# the gettext programs are detected by configure
if GOT_GETTEXT

# common xgettext args: C++ syntax, joining, use the specified macro names as markers
XGETTEXT_ARGS=-CF -k_ -kwxPLURAL:1,2 -kwxTRANSLATE

# implicit rule: generate binary message catalogs
if !CROSS_COMPILATION
%.mo: %.po
	$(MSGFMT) --verbose -c -o $@ $<

%.h: %.mo
	../art/file2h -c -p "locale-" $< > $@
endif

# PO files must be updated from cryptote.pot to include new translations
%.po: cryptote.pot
	if [ -f $@ ]; then $(MSGMERGE) --verbose $@ cryptote.pot > $@.new && mv $@.new $@; else cp cryptote.pot $@; fi

cryptote.pot:
	$(XGETTEXT) $(XGETTEXT_ARGS) -o cryptote.pot \
		../pwgen/*.h ../pwgen/*.cpp \
		../cryptote/*.h ../cryptote/*.cpp

force-update:
	$(RM) cryptote.pot

allpo: force-update
	@for t in $(LINGUAS); do $(MAKE) $$t.po; done

allmo:
	@for t in $(LINGUAS); do $(MAKE) $$t.mo; done

allh:
	@for t in $(LINGUAS); do $(MAKE) $$t.h; done

all-local: allmo allh

# print out the percentage of the translated strings
stats:
	@for i in $(LINGUAS); do \
		x=`$(MSGFMT) --verbose -o /dev/null "$$i.po" 2>&1 | sed -e 's/[,\.]//g' \
			-e 's/\([0-9]\+\) translated messages\?/TR=\1/' \
			-e 's/\([0-9]\+\) fuzzy translations\?/FZ=\1/' \
			-e 's/\([0-9]\+\) untranslated messages\?/UT=\1/'`; \
		TR=0 FZ=0 UT=0; \
		eval $$x; \
		TOTAL=`expr $$TR + $$FZ + $$UT`; \
		echo "$$i.po `expr 100 "*" $$TR / $$TOTAL`% of $$TOTAL strings"; \
	done

endif

.PHONY: allpo allmo allh force-update stats

EXTRA_DIST = cryptote.pot \
	de.po de.mo de.h
