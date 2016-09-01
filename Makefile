# vim: set noexpandtab:ts=4:sw=4:

build_dir = ./build
src_dir = ./src
src_build_dir = $(build_dir)/src
nbddapigen_build_dir = $(src_build_dir)/nbddapigen
vendor_build_dir = $(nbddapigen_build_dir)/vendor
grammar_parser_build_dir = $(vendor_build_dir)/grammar_parser

external_dir = ./external
grammar_parser_ext_dir = $(external_dir)/grammar_parser

dist_build_dir = ./dist
dist_build_src_dir = $(dist_build_dir)/src

DIST_TARGET ?= /usr

version = $(shell cat ./VERSION)


.phony: help clean \
	    build  build-init build-clean build-source \
	    build-vendor build-vendor-grammar-parser \
	    dist dist-clean dist-init dist-build-source


help:
	@echo "available goals"
	@echo "  clean       - cleans all build and test build artifacts"
	@echo "  build       - builds the sources ready for test or distribution"
	@echo "  build-clean - cleans the source build"
	@echo "  dist        - builds the sources ready for distribution"
	@echo "  dist-clean  - cleans the distribution build"


clean: build-clean dist-clean


build: build-init build-vendor build-source


build-init:
	@if test -d $(build_dir); then \
	   echo "run make build-clean first"; \
	   false; \
	 fi
	mkdir -p $(build_dir) \
	         $(nbddapigen_build_dir) \
	         $(vendor_build_dir) \
	         $(grammar_parser_build_dir)


build-vendor: build-vendor-grammar-parser


build-vendor-grammar-parser:
	cp $(grammar_parser_ext_dir)/engine/*.inc \
	   $(grammar_parser_ext_dir)/README.txt \
	   $(grammar_parser_ext_dir)/CHANGELOG.txt \
	   $(grammar_parser_build_dir)/
	find $(grammar_parser_build_dir) -type f -exec chmod uog-x {} +
	wget -O $(grammar_parser_build_dir)/LICENSE \
	   https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt


build-source:
	cp -a $(src_dir)/lib/* $(nbddapigen_build_dir)/
	sed -i "s/__VERSION__/$(version)/" $(nbddapigen_build_dir)/version.inc


build-clean:
	rm -Rf $(build_dir)


dist: $(src_build_dir) dist-init dist-build-source


dist-clean:
	rm -Rf $(dist_build_dir)


dist-init:
	@if test -d $(dist_build_dir); then \
	   echo "run make dist-clean first"; \
	   false; \
	 fi
	mkdir -p $(dist_build_dir) \
	         $(dist_build_src_dir)/$(DIST_TARGET)/bin \
	         $(dist_build_src_dir)/$(DIST_TARGET)/lib


dist-build-source:
	cp -a $(nbddapigen_build_dir) $(dist_build_src_dir)/$(DIST_TARGET)/lib/
	cp $(src_dir)/bin/nbddapigen $(dist_build_src_dir)/$(DIST_TARGET)/bin/


dist-targz:
	cd $(dist_build_src_dir) && \
	  tar czf ../nbddapigen_$(version).tar.gz *


dist-dpkg: dist
	cp -a $(src_dir)/debian $(dist_build_dir)/
	cp CHANGELOG $(dist_build_dir)/debian/changelog
	cp LICENSE $(dist_build_dir)/debian/copyright
	cd $(dist_build_dir) && dpkg-buildpackage -us -uc
	mv ./nbddapigen* $(dist_build_dir)/


dist-rpm: dist-dpkg
	cd $(dist_build_dir) && sudo alien -k -r nbddapigen_$(version)_all.deb

