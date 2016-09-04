WORKDIR = `pwd`

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres

CFLAGS = -Wall -fexceptions
CXXFLAGS = -Wall -fexceptions -std=c++11

CFLAGS_DEBUG = $(CFLAGS) -g
CXXFLAGS_DEBUG = $(CXXFLAGS) -g
OBJDIR_DEBUG = ./obj/Debug
OUT_DEBUG = ../chat_server


CFLAGS_RELEASE = $(CFLAGS) -O2
CXXFLAGS_RELEASE = $(CXXFLAGS) -O2
OBJDIR_RELEASE = ./obj/Release
OUT_RELEASE = ../chat_server


OBJ_DEBUG = $(OBJDIR_DEBUG)/chat_server.o $(OBJDIR_DEBUG)/g_lib.o $(OBJDIR_DEBUG)/packet.o $(OBJDIR_DEBUG)/semaphore.o $(OBJDIR_DEBUG)/sharememory.o $(OBJDIR_DEBUG)/signal_handle.o

OBJ_RELEASE = $(OBJDIR_RELEASE)/chat_server.o $(OBJDIR_RELEASE)/g_lib.o $(OBJDIR_RELEASE)/packet.o $(OBJDIR_RELEASE)/semaphore.o $(OBJDIR_RELEASE)/sharememory.o $(OBJDIR_RELEASE)/signal_handle.o

all: release

clean: clean_debug clean_release

before_debug: 
	test -d $(OBJDIR_DEBUG) || mkdir -p $(OBJDIR_DEBUG)

after_debug: 

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG)
	$(LD) -o $(OUT_DEBUG) $(OBJ_DEBUG)

$(OBJDIR_DEBUG)/chat_server.o: chat_server.cpp
	$(CXX) $(CXXFLAGS_DEBUG) -c chat_server.cpp -o $(OBJDIR_DEBUG)/chat_server.o

$(OBJDIR_DEBUG)/g_lib.o: g_lib.cpp
	$(CXX) $(CXXFLAGS_DEBUG) -c g_lib.cpp -o $(OBJDIR_DEBUG)/g_lib.o

$(OBJDIR_DEBUG)/packet.o: packet.cpp
	$(CXX) $(CXXFLAGS_DEBUG) -c packet.cpp -o $(OBJDIR_DEBUG)/packet.o

$(OBJDIR_DEBUG)/semaphore.o: semaphore.cpp
	$(CXX) $(CXXFLAGS_DEBUG) -c semaphore.cpp -o $(OBJDIR_DEBUG)/semaphore.o

$(OBJDIR_DEBUG)/sharememory.o: sharememory.cpp
	$(CXX) $(CXXFLAGS_DEBUG) -c sharememory.cpp -o $(OBJDIR_DEBUG)/sharememory.o

$(OBJDIR_DEBUG)/signal_handle.o: signal_handle.cpp
	$(CXX) $(CXXFLAGS_DEBUG) -c signal_handle.cpp -o $(OBJDIR_DEBUG)/signal_handle.o

clean_debug: 
	-rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	-rm -rf $(OBJDIR_DEBUG)

before_release: 
	test -d $(OBJDIR_RELEASE) || mkdir -p $(OBJDIR_RELEASE)

after_release: 

release: before_release out_release after_release

out_release: before_release $(OBJ_RELEASE)
	$(LD) -o $(OUT_RELEASE) $(OBJ_RELEASE)

$(OBJDIR_RELEASE)/chat_server.o: chat_server.cpp
	$(CXX) $(CXXFLAGS_RELEASE) $(INC_RELEASE) -c chat_server.cpp -o $(OBJDIR_RELEASE)/chat_server.o

$(OBJDIR_RELEASE)/g_lib.o: g_lib.cpp
	$(CXX) $(CXXFLAGS_RELEASE) $(INC_RELEASE) -c g_lib.cpp -o $(OBJDIR_RELEASE)/g_lib.o

$(OBJDIR_RELEASE)/packet.o: packet.cpp
	$(CXX) $(CXXFLAGS_RELEASE) $(INC_RELEASE) -c packet.cpp -o $(OBJDIR_RELEASE)/packet.o

$(OBJDIR_RELEASE)/semaphore.o: semaphore.cpp
	$(CXX) $(CXXFLAGS_RELEASE) $(INC_RELEASE) -c semaphore.cpp -o $(OBJDIR_RELEASE)/semaphore.o

$(OBJDIR_RELEASE)/sharememory.o: sharememory.cpp
	$(CXX) $(CXXFLAGS_RELEASE) $(INC_RELEASE) -c sharememory.cpp -o $(OBJDIR_RELEASE)/sharememory.o

$(OBJDIR_RELEASE)/signal_handle.o: signal_handle.cpp
	$(CXX) $(CXXFLAGS_RELEASE) $(INC_RELEASE) -c signal_handle.cpp -o $(OBJDIR_RELEASE)/signal_handle.o

clean_release: 
	-rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	-rm -rf $(OBJDIR_RELEASE)

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release

