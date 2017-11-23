B1;2802;0cCONFIG   =  Config.sh
PYTHON   =  /usr/bin/python
CPPFLAGS = -Wall -O4 --std=c++14 -fopenmp
CXX      = g++-5
INCL	 = 
OBJS 	 = tests/Test0.o tests/Test1.o tests/Test2.o 

make: $(OBJS)
	python make_macros.py
	$(CXX) src/CompareCalculationTimes.cpp $(CPPFLAGS) -o bin/CompareTimes
	$(CXX) src/CompareCalculationTimesOMP.cpp $(CPPFLAGS) -o bin/CompareTimesOMP
	$(CXX) src/FindOptimalBlockSize.cpp $(CPPFLAGS) -o bin/FindOptimalBlockSize
	$(CXX) tests/Test0.o $(CPPFLAGS) -o bin/Test0
	$(CXX) tests/Test1.o $(CPPFLAGS) -o bin/Test1
	$(CXX) tests/Test2.o $(CPPFLAGS) -o bin/Test2
	#$(CXX) $(OBJS) $(CPPFLAGS) -o bin/LinAlgTests

clean:
	rm -f *~ src/*.o src/*~ include/*.o include/*~ bin/* \#* src/\#* include/\#* bin/\#* \
	python/*~ python/\#* OUTPUTS/*~ OUTPUTS/\#* OUTPUTS/* tests/*~ tests/\#* tests/*.o

test:
	./tests/RunUnitTests.sh
