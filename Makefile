all: main

main: data.o leaf.o tree.o gbdt.o
		g++ -g data.o leaf.o tree.o gbdt.o main.cpp -I. -o main -std=c++11

data.o: data.cpp
		g++ -c data.cpp -o data.o -std=c++11

leaf.o: leaf.cpp
		g++ -c leaf.cpp -o leaf.o -std=c++11

tree.o: tree.cpp
		g++ -c tree.cpp -o tree.o -std=c++11

gbdt.o: gbdt.cpp
		g++ -c gbdt.cpp -o gbdt.o -std=c++11

clean:
		rm data.o leaf.o tree.o gbdt.o main
