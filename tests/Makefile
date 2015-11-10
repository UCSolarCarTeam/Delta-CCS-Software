CXX = g++
CXXFLAGS = -g -Wall -L/$(GTEST_HOME)/lib  -lgtest -lgtest_main -lpthread
INCS = -I./ -I/$(GTEST_HOME)/include
OBJS =  

testAll: $(OBJS)
	$(CXX) -o testAll  TestMain.cpp $(CXXFLAGS)  $(INCS) $(OBJS)

.cpp.o:
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(INCS)

clean:
	rm testAll *.o