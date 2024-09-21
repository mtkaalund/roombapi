SOURCES := $(wildcard *.cpp)
OBJECTS := $(SOURCES:%.cpp=%.o)
DEPS    := $(OBJECTS:.o=.d)
INCLUDES:= -I.
CXXFLAGS:= -std=c++20 -Wall -pedantic
LDFLAGS := -lgps -lstdc++fs -lpthread
TARGETS := gpsd-example

all: $(TARGETS)
	@echo $(TARGETS)

%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -MMD -o $@

%: %.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(DEPS)
	$(RM) $(TARGETS)

-include $(DEPS)