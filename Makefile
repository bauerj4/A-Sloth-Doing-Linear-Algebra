CONFIG   =  Config.sh
PYTHON   =  /usr/bin/python
CPPFLAGS = -Wall -O3 --std=c++14
CXX      = g++-5
INCL	 = 
OBJS 	 = src/Main.o

make: $(OBJS)
	python make_macros.py
	$(CXX) $(OBJS) $(CPPFLAGS) -o bin/LinAlgTests

clean:
	rm -f *~ src/*.o src/*~ include/*.o include/*~ bin/* \#* src/\#* include/\#* bin/\#* \
	python/*~ python/\#* OUTPUTS/*~ OUTPUTS/\#* OUTPUTS/*
