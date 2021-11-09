PATH_PACKAGE = $(PLATFORM)/package
PATH_MANIFEST = $(PLATFORM)/assets/manifest
PATH_ASSETS = $(PLATFORM)/assets
PACKAGE_DIR	= $(PLATFORM)/package
PATH_PACKAGE = $(PACKAGE_DIR)/$(BINARY)

ASSETS := $(wildcard $(PATH_ASSETS)/*)
DEPENDS := $(OBJECTS:%.o=%.d)

.PHONY: env-check all clean
all: env-check $(PACKAGE)
clean:
	rm -f $(PACKAGE)
	rm -f $(OBJECTS) $(DEPENDS)
	rm -rf $(PATH_PACKAGE)
env-check:
	export LD_LIBRARY_PATH="$(PATH_TOOLCHAIN)/lib"
ifeq (, $(wildcard $(CXX)))
	$(error CXX is not set, did you forget to include platform make file?)
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
	mkdir -p $(PACKAGE_DIR)
	mkdir -p $(PATH_PACKAGE)
	$(C) $(LDFLAGS) $(LIBRARIES) $^ -o $(PATH_PACKAGE)/$(BINARY)
	$(STRIP) $(PATH_PACKAGE)/$(BINARY)
	cp $(RESOURCE_PATH)/*.* $(PATH_PACKAGE)
	cp -R $(PLATFORM)/assets/. $(PATH_PACKAGE)

$(PACKAGE): $(PATH_PACKAGE)/$(BINARY) $(ASSETS)
	@echo "######################################################################"
	@echo "#    PACKAGING $(PACKAGE)"
	@echo "######################################################################"
	tar -C $(PACKAGE_DIR) -zcvf $(PACKAGE) $(BINARY)
ifdef HOST_DIR	
	cp $(PACKAGE) $(HOST_DIR)
endif

-include $(DEPENDS)
