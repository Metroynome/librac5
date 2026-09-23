REGIONS = ntscu pal ntscj
.PHONY: all install clean $(REGIONS)

all: $(REGIONS)

$(REGIONS):
	$(MAKE) -f $(CURDIR)/Makefile.$@

install clean:
	$(MAKE) -f $(CURDIR)/Makefile.ntscu $@
	$(MAKE) -f $(CURDIR)/Makefile.pal $@
	$(MAKE) -f $(CURDIR)/Makefile.ntscj $@
