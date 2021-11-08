PATH_PACKAGE ?= $(STEAMLINK)/package
PATH_MANIFEST ?= $(STEAMLINK)/assets/manifest
PATH_ASSETS ?= $(STEAMLINK)/assets

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
	export LD_LIBRARY_PATH="$(PATH_TOOLCHAIN)/lib"
ifeq (, $(wildcard $(CXX)))
	$(error CXX is not set, did you forget to include Stea.mk?)
endif
ifndef BINARY
	$(error Must define BINARY, name of your application.)
endif

$(PATH_PACKAGE)/$(BINARY): $(OBJECTS)
	@echo "######################################################################"
	@echo "#    BUILDING $(PATH_PACKAGE)/$(BINARY)"
	@echo "######################################################################"
	@echo "SOURCES: $(CXX_SRCS)"
	rm -rf $(PATH_PACKAGE)
	mkdir -p $(PATH_PACKAGE)
	$(LD) $(LDFLAGS) $(LIBRARIES) -lz $^ -o $(PATH_PACKAGE)/$(BINARY)
	$(STRIP) $(PATH_PACKAGE)/$(BINARY)

$(PACKAGE): $(PATH_PACKAGE)/$(BINARY) $(ASSETS) $(COMPONENTS)
	@echo "######################################################################"
	@echo "#    PACKAGING $(PACKAGE)"
	@echo "######################################################################"
ifdef HOST_DIR	
	cp $(PACKAGE) $(HOST_DIR)
endif

-include $(DEPENDS)
