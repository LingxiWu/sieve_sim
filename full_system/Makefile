CC=g++
CFLAGS= -I -Wall -g


# linking 
pcieSim: pcieSim.o sieve.o request.o pciePacket.o bank.o rank.o
	$(CC) $(CFLAGS) -o $@ $^	

pcieSim.o: pcieSim.cpp
	$(CC) $(CFLAGS) -c $<

sieve.o: sieve.cpp sieve.h
	$(CC) $(CFLAGS) -c $<

request.o: request.cpp request.h
	$(CC) $(CFLAGS) -c $<

pciePacket.o: pciePacket.cpp pciePacket.h
	$(CC) $(CFLAGS) -c $<

bank.o: bank.cpp bank.h
	$(CC) $(CFLAGS) -c $<
 
rank.o: rank.cpp rank.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm *.o pcieSim


