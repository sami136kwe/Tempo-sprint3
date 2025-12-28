tests = test_tempo test_input test_interpolate test_describe test_show \
        test_json test_memory

.PHONY: test

test: $(tests)

$(tests):
	bats-core/bin/bats $@.bats
