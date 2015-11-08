CC  = g++
FLAG = -g
TARGET1  = tcpClient
TARGET2  = tcpServer

#$(TARGET1):testTcpClient.o
#	CC -O $@ $^

$(TARGET2):testTcpServer.o tcpSocket.o
	$(CC) -o $@ $^ $(FLAG)

# testTcpClient.o:testTcpClient.cpp
#	CC -c  testTcpClient.cpp $(FLAG)

testTcpServer.o:testTcpServer.cpp
	$(CC) -c  testTcpServer.cpp $(FLAG)
	
tcpSocket.o:tcpSocket.cpp
	$(CC) -c tcpSocket.cpp $(FLAG)
	
clean:
	rm -f $(TARGET1) $(TARGET2) testTcpClient.o testTcpServer.o tcpSocket.o