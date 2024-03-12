CXX := g++
CXXFLAGS := -std=c++17 -O3 -Iincludes -lncurses -lpthread
GTESTFLAGS := -lgtest -lgtest_main -pthread

LIB_CPP_FILES := $(wildcard src/lib/*.cpp)
LIB_OBJ_FILES := $(LIB_CPP_FILES:src/lib/%.cpp=obj/%.o)

EXEC_CPP_FILES := $(wildcard src/exec/*.cpp)
EXEC_OBJ_FILES := $(EXEC_CPP_FILES:src/exec/%.cpp=obj/%.o.run)
EXEC_BIN_FILES := $(EXEC_CPP_FILES:src/exec/%.cpp=bin/run.%)

TEST_FILES := $(wildcard testsrc/*.cpp)
TEST_OBJ_FILES := $(TEST_FILES:testsrc/%.cpp=obj/%.o.gtest)
TEST_BIN_FILES := $(TEST_FILES:testsrc/%.cpp=bin/gtest.%)

DATASET := wordleSmall
GTEST_OPTIONS := --gtest_color=yes --gtest_brief=1

compile: $(EXEC_BIN_FILES) $(TEST_BIN_FILES)
createstats: $(EXEC_BIN_FILES)
	bin/run.$@ data/$(DATASET).txt data/$(DATASET).csv 0 thread
sim: $(EXEC_BIN_FILES)
	#bin/run.createstats data/$(DATASET).txt data/$(DATASET).csv 0 thread
	bin/run.$@ data/$(DATASET).csv
game: $(EXEC_BIN_FILES)
	bin/run.$@ data/$(DATASET).csv
game_pretty: $(EXEC_BIN_FILES)
	bin/run.game_pretty data/$(DATASET).csv 1
gtest: $(TEST_BIN_FILES)
	@for test in $^; do \
		echo "Running $$test $(GTEST_OPTIONS)"; \
		$$test $(GTEST_OPTIONS); \
	done
obj/%.o: src/lib/%.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@
bin/run.%: src/exec/%.cpp $(LIB_OBJ_FILES)
	$(CXX) $(CXXFLAGS) $^ -o $@
bin/gtest.%: testsrc/%.cpp $(LIB_OBJ_FILES)
	$(CXX) $(CXXFLAGS) $^ $(GTESTFLAGS) -o $@
clean:
	mkdir -p bin
	mkdir -p obj
	rm -rf bin/*
	rm -rf obj/*
