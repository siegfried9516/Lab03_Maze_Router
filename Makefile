CXX = g++
CXXFLAGS = -I ./src -O3
SRCS = ./src/main.cpp ./src/MR.cpp
OUTFILE = Lab3

all: $(OUTFILE)

$(OUTFILE): $(SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(OUTFILE)
