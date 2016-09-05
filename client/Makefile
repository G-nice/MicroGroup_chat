WORKDIR = `pwd`

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres

CXXFLAGS = -Wall -fexceptions


CXXFLAGS_DEBUG = $(CXXFLAGS) -g
OBJDIR_DEBUG = ./obj/Debug
OUT_DEBUG = ../chat_client

CXXFLAGS_RELEASE = $(CXXFLAGS) -O2
OBJDIR_RELEASE = obj/Release
OUT_RELEASE = ../chat_client


OBJ_DEBUG = $(OBJDIR_DEBUG)/chat_client.o $(OBJDIR_DEBUG)/epoll.o $(OBJDIR_DEBUG)/g_lib.o $(OBJDIR_DEBUG)/packet.o $(OBJDIR_DEBUG)/signal_handle.o

OBJ_RELEASE = $(OBJDIR_RELEASE)/chat_client.o $(OBJDIR_RELEASE)/epoll.o $(OBJDIR_RELEASE)/g_lib.o $(OBJDIR_RELEASE)/packet.o $(OBJDIR_RELEASE)/signal_handle.o

all: release

clean: clean_debug clean_release

before_debug: 
	test -d $(OBJDIR_DEBUG) || mkdir -p $(OBJDIR_DEBUG)

after_debug: 

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG)
	$(LD) -o $(OUT_DEBUG) $(OBJ_DEBUG)

$(OBJDIR_DEBUG)/chat_client.o: chat_client.cpp
	$(CXX) $(CXXFLAGS_DEBUG) -c chat_client.cpp -o $(OBJDIR_DEBUG)/chat_client.o

$(OBJDIR_DEBUG)/epoll.o: epoll.cpp
	$(CXX) $(CXXFLAGS_DEBUG) -c epoll.cpp -o $(OBJDIR_DEBUG)/epoll.o

$(OBJDIR_DEBUG)/g_lib.o: g_lib.cpp
	$(CXX) $(CXXFLAGS_DEBUG) -c g_lib.cpp -o $(OBJDIR_DEBUG)/g_lib.o

$(OBJDIR_DEBUG)/packet.o: packet.cpp
	$(CXX) $(CXXFLAGS_DEBUG) -c packet.cpp -o $(OBJDIR_DEBUG)/packet.o

$(OBJDIR_DEBUG)/signal_handle.o: signal_handle.cpp
	$(CXX) $(CXXFLAGS_DEBUG) -c signal_handle.cpp -o $(OBJDIR_DEBUG)/signal_handle.o

clean_debug: 
	- rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	- rm -rf $(OBJDIR_DEBUG)

before_release: 
	test -d $(OBJDIR_RELEASE) || mkdir -p $(OBJDIR_RELEASE)

after_release: 

release: before_release out_release after_release

out_release: before_release $(OBJ_RELEASE)
	$(LD) -o $(OUT_RELEASE) $(OBJ_RELEASE)

$(OBJDIR_RELEASE)/chat_client.o: chat_client.cpp
	$(CXX) $(CXXFLAGS_RELEASE) -c chat_client.cpp -o $(OBJDIR_RELEASE)/chat_client.o

$(OBJDIR_RELEASE)/epoll.o: epoll.cpp
	$(CXX) $(CXXFLAGS_RELEASE) -c epoll.cpp -o $(OBJDIR_RELEASE)/epoll.o

$(OBJDIR_RELEASE)/g_lib.o: g_lib.cpp
	$(CXX) $(CXXFLAGS_RELEASE) -c g_lib.cpp -o $(OBJDIR_RELEASE)/g_lib.o

$(OBJDIR_RELEASE)/packet.o: packet.cpp
	$(CXX) $(CXXFLAGS_RELEASE) -c packet.cpp -o $(OBJDIR_RELEASE)/packet.o

$(OBJDIR_RELEASE)/signal_handle.o: signal_handle.cpp
	$(CXX) $(CXXFLAGS_RELEASE) -c signal_handle.cpp -o $(OBJDIR_RELEASE)/signal_handle.o

clean_release: 
	- rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	- rm -rf $(OBJDIR_RELEASE)

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release

