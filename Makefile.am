ACLOCAL_AMFLAGS = -I m4
bin_PROGRAMS = pack
noinst_PROGRAMS = make_specialescapes
lib_LTLIBRARIES = libd2hs.la

BUILT_SOURCES = specialescapes.c

libd2hs_la_SOURCES = d2h_convert.c

pack_SOURCES = main.c
pack_LDADD = libd2hs.la

d2h_convert.o: specialescapes.c

specialescapes.c: make_specialescapes
	./make_specialescapes > $@.temp
	mv $@.temp $@

make_specialescapes_SOURCES = make_specialescapes.c
