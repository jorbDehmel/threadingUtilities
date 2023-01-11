demo.out:	demo.cpp mutex.hpp
	clang++ -o demo.out -pthread demo.cpp

pclean:
	rm -rf *.out *.o