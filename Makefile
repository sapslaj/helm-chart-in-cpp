.PHONY: all clean wasm test

# Configuration
PLUGIN_NAME = cpp-renderer
WASM_OUTPUT = plugin.wasm
CC = clang
CFLAGS = --target=wasm32-unknown-wasi \
         -O2 \
         -nostdlib \
         -Wl,--no-entry \
         -Wl,--export=helm_plugin_main \
         -Wl,--allow-undefined \
         -I./include

# Source files
SOURCES = src/main.cpp

# Default target
all: wasm

# Build WASM plugin
wasm: $(WASM_OUTPUT)

$(WASM_OUTPUT): $(SOURCES) include/extism-pdk.h include/simple_json.h
	@echo "Building WASM plugin..."
	$(CC) $(CFLAGS) -o $(WASM_OUTPUT) $(SOURCES)
	@echo "✓ Built $(WASM_OUTPUT)"
	@ls -lh $(WASM_OUTPUT)

# Clean build artifacts
clean:
	rm -f $(WASM_OUTPUT)
	@echo "✓ Cleaned build artifacts"

# Test the plugin (requires Go test driver)
test: wasm
	@echo "Testing plugin..."
	@if [ -d testdriver ]; then \
		cd testdriver && go test -v; \
	else \
		echo "Test driver not yet implemented"; \
	fi

# Install plugin locally (for Helm 4)
install: wasm
	@echo "Installing plugin to ~/.local/share/helm/plugins/$(PLUGIN_NAME)/"
	@mkdir -p ~/.local/share/helm/plugins/$(PLUGIN_NAME)
	@cp $(WASM_OUTPUT) ~/.local/share/helm/plugins/$(PLUGIN_NAME)/
	@cp plugin.yaml ~/.local/share/helm/plugins/$(PLUGIN_NAME)/
	@echo "✓ Plugin installed"

# Show help
help:
	@echo "Helm 4 C++ WASM Plugin Build System"
	@echo ""
	@echo "Targets:"
	@echo "  make wasm     - Build the WASM plugin"
	@echo "  make clean    - Remove build artifacts"
	@echo "  make test     - Run tests (requires Go test driver)"
	@echo "  make install  - Install plugin for local Helm 4"
	@echo "  make help     - Show this help message"
