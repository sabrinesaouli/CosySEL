sources := $(wildcard $(SRC)*.cc)
headers := $(wildcard include/cosy/*.h)
headers := $(filter-out %IntegralTypes.h %IntType.h %Macros.h %Bitset.h, \
	$(headers))

objects := $(patsubst %.cc, $(OBJ)%.o, $(sources))

tests := $(wildcard tests/units/*.test.cc)
tests_objects := $(patsubst %.cc, $(OBJ)%.o, $(tests))
tests_objects +=  $(patsubst %.cc, $(OBJ)tests/%.o, $(sources))

lib := libcosy.a

$(call REQUIRE-DIR, $(LIB)$(lib))
$(call REQUIRE-DIR, $(BIN)test)
$(call REQUIRE-DIR, $(objects))
$(call REQUIRE-DIR, $(tests_objects))
$(call REQUIRE-DEP, $(sources))
$(call REQUIRE-DEP, $(tests))


$(LIB)$(lib): $(objects)

CFLAGS += -Iinclude/ -fPIC -Wall -Wextra

default: CFLAGS += -O3 -DNDEBUG
default: $(LIB)$(lib)

################################################################################
# TESTS

test : CFLAGS  += -O0 -D DEBUG -g -fprofile-arcs -ftest-coverage
test : LDFLAGS += -lgtest -lgtest_main -lpthread -lbliss -lsaucy -lgcov

test: $(BIN)test
run-test: test
	$(call cmd-call, ./$(BIN)test)
run-test-valgrind: test
	$(call cmd-valgrind-mem, ./$(BIN)test)
run-test-gdb: test
	$(call  cmd-gdb, ./$(BIN)test)

# Generic rules

$(LIB)%.a:
	$(call cmd-ar, $@, $^)

$(BIN)test: $(tests_objects)
	$(call cmd-ld, $@, $^, $(LDFLAGS))


################################################################################
# STYLE

check-style: $(sources) $(headers)
	$(call cmd-call, ./tests/sanity/cpplint.py --root=$$PWD $^)
