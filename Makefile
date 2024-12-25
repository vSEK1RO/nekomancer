DIRS =\
	release/components\
	release/systems\
	build/components\
	build/systems

.PHONY: build release

build:
	cmake -B $@
	${MAKE} -C $@

release: clear ${DIRS}
	install -Dm755 build/engine/nek-engine $@
	install -Dm644 engine/nek-config.json $@
	cp -rf build/components build/systems $@
	find components/ -type f ! -name '*.hpp' ! -name '*.cpp' ! -path '*/CMakeLists.txt'\
		-exec sh -c 'for file; do mkdir -p "$(dirname "release/$file")"; cp "$file" "release/$file"; done' _ {} +
	find $@ -type d \( -name "CMakeFiles" \) -exec rm -rf {} +
	find $@ -type f \( -name "*.cmake" -o -name "Makefile" \) -exec rm -f {} +

clear:
	rm -rf release/*

${DIRS}:
	mkdir -p $@