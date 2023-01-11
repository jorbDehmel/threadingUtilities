demo.out:	demo.cpp mutex.hpp
	clang++ -o demo.out -pthread demo.cpp

demo2.out:	demo2.cpp mutex.hpp
	clang++ -o demo2.out -pthread demo2.cpp

pclean:
	rm -rf *.out *.o