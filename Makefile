CFLAGS+=-O2

O=$(patsubst %,o/%.o,$N) \
$(eval objects:=$$(objects) $(N))

EXE=@echo EXE $@; $(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)
COMPILE=@echo COMPILE $*; $(CC) -MT $@ -MMD $(CFLAGS) -c -o $@ $<

N=main d2h_convert
pack: $(O)
	$(EXE)

o/%.o: %.c | o
	$(COMPILE)

o/%.d: %.c | o
	@echo DEP $*; $(CC) -ftabstop=2 -MT o/$*.o -MM -MG $(CFLAGS) -c -o $@ $<

o:
	mkdir o

specialescapes.c: make_specialescapes
	./$< >$@.temp
	mv $@.temp $@

N=make_specialescapes
make_specialescapes: $(O)
	$(EXE)

clean:
	git clean -ndx
	@echo ^C to not delete
	@read
	git clean -fdx

-include $(patsubst %, o/%.d,$(objects))
