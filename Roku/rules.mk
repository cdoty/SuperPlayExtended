# Copyright 2021 Roku, Inc.  All rights reserved.
PATH_PACKAGE ?= $(ROKU)/package
PATH_MANIFEST ?= $(ROKU)/assets/manifest
PATH_ASSETS ?= $(ROKU)/assets
PATH_COMPONENTS ?= $(ROKU)/components

COMPONENTS := $(wildcard $(PATH_COMPONENTS)/*)
ASSETS := $(wildcard $(PATH_ASSETS)/*)
DEPENDS := $(OBJECTS:%.o=%.d)

.PHONY: env-check all clean repackage package-clean
all: env-check $(PACKAGE)
repackage: package-clean $(PACKAGE)
package-clean:
	rm -f $(PACKAGE)
	rm -rf $(PATH_PACKAGE)
clean:
	rm -f $(PACKAGE)
	rm -f $(OBJECTS) $(DEPENDS)
	rm -rf $(PATH_PACKAGE)
env-check:
ifeq (, $(wildcard $(CXX)))
	$(error CXX is not set, did you forget to include Roku4.mk?)
endif
ifndef BINARY
	$(error Must define BINARY, name of your application.)
endif
ifndef PATH_MANIFEST
	$(error Must define PATH_MANIFEST, file containing information about your application.)
endif

$(PATH_PACKAGE)/$(BINARY): $(OBJECTS)
	@echo "######################################################################"
	@echo "#    BUILDING $(PATH_PACKAGE)/$(BINARY)"
	@echo "######################################################################"
	@echo "SOURCES: $(C_SRCS)$(CC_SRCS)$(CXX_SRCS)"
	rm -rf $(PATH_PACKAGE)
	mkdir -p $(PATH_PACKAGE)
	$(LD) $(LDFLAGS) -lz $^ -o $(PATH_PACKAGE)/$(BINARY)
	$(STRIP) $(PATH_PACKAGE)/$(BINARY)

$(PACKAGE): $(PATH_PACKAGE)/$(BINARY) $(ASSETS) $(COMPONENTS)
	@echo "######################################################################"
	@echo "#    PACKAGING $(PACKAGE)"
	@echo "######################################################################"
	rm -f $(PACKAGE)
	rm -rf $(PATH_PACKAGE)/source
	cp -a $(PATH_ASSETS) $(PATH_PACKAGE)/source
	rm -f `find $(PATH_PACKAGE)/source -name manifest`
	cp -f $(PATH_MANIFEST) $(PATH_PACKAGE)
	cp -f $(RESOURCE_PATH)/icon-246x140.png $(PATH_PACKAGE)/source
	cp -f $(RESOURCE_PATH)/icon-290x218.png $(PATH_PACKAGE)/source
	cp -f $(RESOURCE_PATH)/splash-720p.png $(PATH_PACKAGE)/source
	cp -f $(RESOURCE_PATH)/splash-480p.png $(PATH_PACKAGE)/source
	chmod -R 755 $(PATH_PACKAGE)
	$(UTIL_MKSQUASHFS) $(PATH_PACKAGE) $(PACKAGE) -force-uid 499 -force-gid 499 -noappend -comp gzip
	cp $(PACKAGE) $(HOST_DIR)

-include $(DEPENDS)
