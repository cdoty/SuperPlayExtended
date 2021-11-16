PATH_PACKAGE = $(PLATFORM)/package
PATH_MANIFEST = $(PLATFORM)/assets/manifest
PATH_ASSETS = $(PLATFORM)/assets
PACKAGE_DIR	= $(PLATFORM)/package
PACKAGE		= $(PACKAGE_DIR)/$(BINARY)

ASSETS := $(wildcard $(PATH_ASSETS)/*)
DEPENDS := $(OBJECTS:%.o=%.d)

.PHONY: env-check all clean
all: env-check $(PACKAGE)
clean:
	rm -f $(PACKAGE)
	rm -f $(OBJECTS) $(DEPENDS)
	rm -rf $(PATH_PACKAGE)
env-check:
ifndef BINARY
	$(error Must define BINARY, name of your application.)
endif

$(PACKAGE): $(OBJECTS)
	@echo "######################################################################"
	@echo "#    BUILDING $(PATH_PACKAGE)/$(BINARY)"
	@echo "######################################################################"
	@echo "SOURCES: $(CXX_SRCS)"
	rm -rf $(PATH_PACKAGE)
	mkdir -p $(PACKAGE_DIR)
	$(C) $(LDFLAGS) $(LIBRARIES) $^ -o $(PACKAGE)
	$(STRIP) $(PACKAGE_DIR)/$(BINARY)
	cp $(RESOURCE_PATH)/*.* $(PACKAGE_DIR)
	cp -R $(PLATFORM)/assets/. $(PACKAGE_DIR)

-include $(DEPENDS)
