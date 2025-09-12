main: ./source/main.cpp
	g++ -std=c++17 ./source/main.cpp -o main
	./main
clean:
	rm -rf main