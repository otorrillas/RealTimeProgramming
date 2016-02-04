all: StreamCamera.x ReceiveStream.x

ReceiveStream.x : read.cpp
	g++ -o ReceiveStream.x read.cpp `pkg-config --libs opencv`

StreamCamera.x : write.cpp
	g++ -o StreamCamera.x write.cpp `pkg-config --libs opencv`
