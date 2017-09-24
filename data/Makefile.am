desktopdir = $(datadir)/applications
dist_desktop_DATA = corgan.desktop

EXTRA_DIST = corgan.desktop.in
CLEANFILES = corgan.desktop

corgan.desktop: corgan.desktop.in
	$(AM_V_GEN) $(MSGFMT) --desktop --template $< -d $(top_srcdir)/po -o $@
