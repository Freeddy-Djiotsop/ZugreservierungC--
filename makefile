#Freeddy Djiotsop

run: build zug.txt
	./reservierung zug.txt

build: reservierung.cpp
	g++ -Wall reservierung.cpp -o reservierung
