SUBMISSION = sworms
BUILDDIR   = build
EXECUTABLE = $(SUBMISSION)
SOURCES    = $(wildcard *.cpp)
ZIP_SOURCES = $(filter-out Random.cpp Random.h, $(wildcard *.cpp *.h))
OBJECTS    = $(patsubst %,$(BUILDDIR)/%,${SOURCES:.cpp=.o})
CXX        = g++
CCFLAGS    = -Wall -g -c -std=c++14 -o
LDFLAGS    =
LDLIBS     =
DEFINE     = #-DDEBUG

ifeq ($(OS),Windows_NT)
else
	LDLIBS += -ldl
endif

#-------------------------------------------------------------------------------
#make executable
all: $(BUILDDIR) $(EXECUTABLE)

#create build directory
$(BUILDDIR):
ifeq ($(OS),Windows_NT)
	if not exist $(BUILDDIR) mkdir $(BUILDDIR)
else
	mkdir -p $(BUILDDIR)
endif

#compile objects
$(BUILDDIR)/%.o: %.cpp
	$(CXX) $(CCFLAGS) $@ $< -MMD -MF ./$@.d $(DEFINE)

#link Objects
$(EXECUTABLE) : $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

#make clean
clean:
ifeq ($(OS),Windows_NT)
	del /Q .\$(BUILDDIR)\*.o
	del /Q .\$(BUILDDIR)\*.o.d
	del /Q $(EXECUTABLE).exe
	del /Q $(EXECUTABLE).zip
else
	rm -rf ./$(BUILDDIR)
	rm -f $(EXECUTABLE)
	rm -f $(EXECUTABLE).zip
endif

#make valgrind
valgrind:
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origin=yes ./$(EXECUTABLE) test.sworms

#make webinterface
webinterface: all
ifeq ($(OS),Windows_NT)
	cmd /c start sworms.xyz
else
	-xdg-open http://sworms.xyz
endif
	python3 launcher.py ./sworms test.sworms

#make submission zip archive for PALME
submission:
ifeq ($(OS),Windows_NT)
	del /Q $(SUBMISSION).zip
else
	rm -f $(SUBMISSION).zip
endif
	zip $(SUBMISSION).zip $(ZIP_SOURCES)

.PHONY: clean submission

#The dependencies:
-include $(wildcard $(BUILDDIR)/*.d)
