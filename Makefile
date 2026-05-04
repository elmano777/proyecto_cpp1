CXX = g++
CXXFLAGS = -std=c++17 $(shell pkg-config --cflags Qt6Widgets Qt6Core)
LIBS = $(shell pkg-config --libs Qt6Widgets Qt6Core)
MOC = /usr/lib64/qt6/libexec/moc

run: spreadsheet
	./spreadsheet

spreadsheet: src/main.cpp src/core/spreadSheet.cpp src/ui/mainWindow.cpp src/moc_mainWindow.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

src/moc_mainWindow.cpp: src/ui/mainWindow.h
	$(MOC) $< -o $@

clean:
	rm -f spreadsheet src/moc_mainWindow.cpp
