run:
	g++ -std=c++17 src/main.cpp src/spreadSheet.cpp -o spreadsheet && ./spreadsheet

clean:
	rm -f spreadsheet
