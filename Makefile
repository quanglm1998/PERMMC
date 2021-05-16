SOURCEDIR = src/code/cpp
BUILDDIR = build

FLAGS   := -std=c++17 -g -ftree-vectorize -Wall -O2
OBJECTS := $(BUILDDIR)/city.o $(BUILDDIR)/tree.o $(BUILDDIR)/algorithms.o

brute_force: $(SOURCEDIR)/brute_force.cc $(OBJECTS)
	g++ $(FLAGS) $(SOURCEDIR)/brute_force.cc $(OBJECTS) -o $(BUILDDIR)/brute_force

nguyen2020: $(SOURCEDIR)/nguyen2020.cc $(OBJECTS)
	g++ $(FLAGS) $(SOURCEDIR)/nguyen2020.cc $(OBJECTS) -o $(BUILDDIR)/nguyen2020

main: $(BUILDDIR)/main.o $(OBJECTS)
	g++ $(FLAGS) $(BUILDDIR)/main.o $(OBJECTS) -o $(BUILDDIR)/main

$(BUILDDIR)/main.o: $(SOURCEDIR)/main.cc
	g++ $(FLAGS) -c $(SOURCEDIR)/main.cc -o $(BUILDDIR)/main.o
	
$(BUILDDIR)/city.o: $(SOURCEDIR)/tsp/city.cpp
	g++ $(FLAGS) -c $(SOURCEDIR)/tsp/city.cpp	-o $(BUILDDIR)/city.o
	
$(BUILDDIR)/tree.o: $(SOURCEDIR)/tsp/tree.cpp
	g++ $(FLAGS) -c $(SOURCEDIR)/tsp/tree.cpp -o $(BUILDDIR)/tree.o
	
$(BUILDDIR)/algorithms.o: $(SOURCEDIR)/tsp/algorithms.cpp
	g++ $(FLAGS) -c $(SOURCEDIR)/tsp/algorithms.cpp -o $(BUILDDIR)/algorithms.o

gen_data: $(SOURCEDIR)/gen_data.cc
	g++ $(FLAGS) $(SOURCEDIR)/gen_data.cc -o $(BUILDDIR)/gen_data

clean:
	rm -rf $(BUILDDIR)/*.o $(BUILDDIR)/main $(BUILDDIR)/gen_data
