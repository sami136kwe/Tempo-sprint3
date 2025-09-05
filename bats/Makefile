tests = test_tempo test_input test_describe test_show

.PHONY: test

test: $(tests)

$(tests):
	bats-core/bin/bats $@.bats
