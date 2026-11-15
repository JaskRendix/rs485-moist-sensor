BUILD_DIR := build

FORMAT_FILES := \
	src/*.c \
	src/*.h \
	tests/*.cpp

.PHONY: test configure clean rebuild format

configure:
	cmake -S . -B $(BUILD_DIR)

test: configure
	cmake --build $(BUILD_DIR)
	ctest --test-dir $(BUILD_DIR) --output-on-failure

format:
	clang-format -i $(FORMAT_FILES)

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean test