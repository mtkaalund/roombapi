SOURCES := $(wildcard *.cpp)
OBJECTS := $(SOURCES:%.cpp=objs/%.o)
DEPS    := $(OBJECTS:objs/%.o=%.d)
INCLUDES:= -I.
CXXFLAGS:= -std=c++20 -Wall -pedantic -pthread -O0 -g3 -Wextra
LDFLAGS := -lgps -lstdc++fs -lpthread -L/usr/lib
TARGETS := gpsd-example i2c-test

.PHONY: clean all

all: $(OBJECTS) $(TARGETS)

objs/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -MMD -o $@

%: objs/%.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(DEPS)
	$(RM) $(TARGETS)

-include $(DEPS)