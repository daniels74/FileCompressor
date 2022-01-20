buildd:
#	rm -f program.exe
	g++ -g -std=c++11 -Wall main.cpp hashmap.cpp -o program.exe
	
runn:
	./program.exe
	
valgrind:
	valgrind --tool=memcheck --leak-check=yes ./program.exe	
	
build_tests:
#	rm -f tests.exe
	g++ -g -std=c++11 -Wall tests.cpp -o tests.exe -lgtest -lgtest_main -lpthread
	
run_tests:
	./tests.exe --gtest_color=yes

valgrind_tests:
	valgrind --tool=memcheck --leak-check=yes ./tests.exe

