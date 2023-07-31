# test.cpp tabu_search.cpp basic.h
tar = mavsd
obj = cross_ctsp.o   Individual.o  LS.o read_data.o Tstwoopt.o initialsol.o solution.o main.o
deps = cross_ctsp.h   Individual.h  LS.h  read_data.h Tstwoopt.h initialsol.h solution.h basic.h
cc = g++
RMRF :=rm -rf

$(tar): $(obj)      # transfer to exe files
	$(cc) $^ -o $@
#test: tabu_search.o test.o
#	g++ tabu_search.o test.o -o test
%.o: %.c $(deps)    # head files
	$(cc) -c %.c -o %.o   # transfer a file
#test.o: test.cpp
#	g++ -c test.cpp -o test.o
#tabu_search.o: tabu_search.cpp
#	g++ -c tabu_search.cpp -o tabu_search.o

.PHONY:
clearall:
	$(RMRF) $(obj) $(tar)
clear:
	$(RMRF) $(obj) 