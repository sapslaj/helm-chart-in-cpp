#define EXTISM_IMPLEMENTATION
#include "../include/extism-pdk.h"
#include "../include/shit.h"
#include "../include/base64.h"

#define BUFFER_SIZE 16384
static char input_buffer[BUFFER_SIZE];
static char output_buffer[BUFFER_SIZE];

extern "C" {
    int32_t EXTISM_EXPORTED_FUNCTION(helm_plugin_main) {
        uint64_t input_len = extism_input_length();

        if (input_len == 0) {
            extism_output_buf_from_sz("{\"manifests\":\"\"}");
            return 0;
        }

        if (input_len >= BUFFER_SIZE) {
            extism_output_buf_from_sz("Input too large");
            return 1;
        }

        if (!extism_load_input(0, input_buffer, input_len)) {
            extism_output_buf_from_sz("Failed to load input");
            return 1;
        }

        char manifest[] =
            "---\n"
            "apiVersion: apps/v1\n"
            "kind: Deployment\n"
            "metadata:\n"
            "  name: nginx\n"
            "  labels:\n"
            "    app: nginx\n"
            "spec:\n"
            "  replicas:\n"
            "  selector:\n"
            "    matchLabels:\n"
            "      app: nginx\n"
            "  template:\n"
            "    metadata:\n"
            "      labels:\n"
            "        app: nginx\n"
            "    spec:\n"
            "      containers:\n"
            "        - name: nginx\n"
            "          image: nginx:latest\n"
            "          ports:\n"
            "            - containerPort: 80\n"
        ;

        // if we're unmarshaling into `[]byte` then we need to base64 encode
        // this, but the struct is using `bytes.Buffer` so it don't work right
        // now.
        char base64_output[BUFFER_SIZE];
        size_t encoded_len = base64_encode(
            (uint8_t*)manifest,
            sizeof(manifest)-1,
            base64_output,
            sizeof(base64_output)
        );
        if (encoded_len == 0) {
            extism_output_buf_from_sz("Failed to encode base64");
            return 1;
        }

        size_t json_len = my_snprintf(
            output_buffer,
            sizeof(output_buffer),
            "{\"manifests\":\"%s\"}",
            base64_output
        );

        if (json_len > 0 && json_len < sizeof(output_buffer)) {
            extism_output_buf(output_buffer, json_len);
            return 0;
        } else {
            extism_output_buf_from_sz("Failed to create output");
            return 1;
        }
    }
}
