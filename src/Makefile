All: kid
kid: main.cpp utils.h utils.cpp
	g++ main.cpp utils.cpp -o kid `pkg-config --libs --cflags opencv`

clean:
	rm kid
