SOURCES := $(wildcard *.cpp)
OBJECTS := $(SOURCES:%.cpp=objs/%.o)
DEPS    := $(OBJECTS:objs/%.o=%.d)
INCLUDES:= -I.
CXXFLAGS:= -std=c++20 -Wall -pedantic -pthread -O0 -g3 -Wextra
LDFLAGS := -lgps -lstdc++fs -lpthread -L/usr/lib
TARGETS := gpsd-example

.PHONY: clean all

all: $(TARGETS)

objs/$(TARGETS).o: $(TARGETS).cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -MMD -o $@

$(TARGETS): objs/$(TARGETS).o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(DEPS)
	$(RM) $(TARGETS)

-include $(DEPS)