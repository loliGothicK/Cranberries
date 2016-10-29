.PHONY: Debug
Debug:
	cd test; make Debug
.PHONY: Release
Release:
	cd test; make Release
.PHONY: clean
clean:
	cd test; make clean