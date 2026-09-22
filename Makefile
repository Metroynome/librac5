# Native PS2DEV tools or an existing SDK shell; no SDK installation required.
REGIONS := auto ntscu pal ntscj
.PHONY: all install clean $(REGIONS)
all: $(REGIONS)
$(REGIONS):
	$(MAKE) -f Makefile.region REGION=$@
install:
	@for region in $(REGIONS); do $(MAKE) -f Makefile.region REGION=$$region install || exit $$?; done
clean:
	rm -rf obj lib
