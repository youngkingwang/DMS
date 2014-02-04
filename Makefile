ALL=logReader.o logSender.o client.o main.o
main:$(ALL)
	g++ $(ALL) -o main
main.o:main.cpp
	g++ -c main.cpp
client.o:client.cpp
	g++ -c client.cpp
logReader.o:logReader.cpp
	g++ -c logReader.cpp
logSender.o:logSender.cpp
	g++ -c logSender.cpp


clean:
	rm -f *.gch *.o main
