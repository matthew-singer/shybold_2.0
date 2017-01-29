CC=g++
CFLAGS=-std=c++11 -O3
DEBUG=-std=c++11 -g
OPENCV=-I/mnt/lfs2/stre3949/local/include -L/mnt/lfs2/stre3949/local/lib   -lopencv_core -lopencv_highgui -lopencv_features2d -lopencv_flann -lopencv_imgproc -lopencv_imgcodecs  -D OPEN_CV
all:
	$(CC) *.cpp $(CFLAGS)
draw:
	$(CC) *.cpp $(CFLAGS) $(OPENCV) -o draw_interaction
debug:
	$(CC) *.cpp $(DEBUG)
