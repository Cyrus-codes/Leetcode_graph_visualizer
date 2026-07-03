CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall -Wextra
BD = bin
PD = p
T = $(BD)/a

.PHONY: all clean dirs

all: dirs $(T)

dirs:
	@mkdir -p $(BD)

$(T): $(PD)/a.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	@rm -rf $(BD)
