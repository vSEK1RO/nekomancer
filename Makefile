DIRS =\
	release/components\
	release/systems\

.PHONY: build release

build:
	cmake -B $@
	${MAKE} -C $@

release: ${DIRS}
	rm -rf release
	install -Dm755 build/engine/nek-engine $@
	install -Dm644 engine/nek-config.json $@
	install -Dm755 build/components/*.so $@/components
	install -Dm755 build/systems/*.so $@/systems

${DIRS}:
	mkdir -p $@