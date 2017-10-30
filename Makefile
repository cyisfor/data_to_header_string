CFLAGS+=-O2

EXE=$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

pack: main.c d2h_convert.c specialescapes.c
	$(EXE)

specialescaples.c: make_specialescapes
	./$< >$@.temp
	mv $@.temp $@

make_specialescapes: make_specialescapes.c
	$(EXE)
