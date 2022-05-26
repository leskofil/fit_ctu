CXX=g++
LD=g++
CXXFLAGS=-Wall -pedantic -g -std=c++17 
TARGET=leskofil
MKDIR = mkdir -p
BUILD_DIR = build

all: compile

compile: $(TARGET)

run: $(TARGET)
	./$(TARGET)

$(TARGET): $(BUILD_DIR)/CApplication.o $(BUILD_DIR)/CCompletedFilter.o $(BUILD_DIR)/CFileExporter.o $(BUILD_DIR)/CFileHandler.o $(BUILD_DIR)/CNote.o $(BUILD_DIR)/CSearcher.o $(BUILD_DIR)/CShoppingNote.o $(BUILD_DIR)/CFilter.o $(BUILD_DIR)/CTagFilter.o $(BUILD_DIR)/CTextFilter.o $(BUILD_DIR)/CTextNote.o $(BUILD_DIR)/CToDoNote.o $(BUILD_DIR)/main.o 
	$(CXX) $(CXXFLAGS) -o $@ $^ -lstdc++fs 


$(BUILD_DIR)/%.o: src/%.cpp
	$(MKDIR) $(BUILD_DIR)
	$(CXX) $(CXX_FLAGS) -c -o $@ $< -std=c++1z

doc:
	doxygen Doxyfile

clean:
	rm -rf $(TARGET) $(BUILD_DIR)/ 2>/dev/null

#dependencies
CApplication.o: src/CApplication.cpp src/CApplication.h src/CNote.h \
 src/CTextNote.h src/CToDoNote.h src/CShoppingNote.h src/CFileHandler.h \
 src/CFileExporter.h src/CSearcher.h src/CFilter.h src/CCompletedFilter.h \
 src/CTagFilter.h src/CTextFilter.h
CCompletedFilter.o: src/CCompletedFilter.cpp src/CCompletedFilter.h \
 src/CFilter.h src/CNote.h
CFileExporter.o: src/CFileExporter.cpp src/CFileExporter.h src/CNote.h \
 src/CSearcher.h src/CFilter.h src/CCompletedFilter.h src/CTagFilter.h \
 src/CTextFilter.h
CFileHandler.o: src/CFileHandler.cpp src/CFileHandler.h src/CNote.h \
 src/CFileExporter.h src/CSearcher.h src/CFilter.h src/CCompletedFilter.h \
 src/CTagFilter.h src/CTextFilter.h src/CTextNote.h src/CToDoNote.h \
 src/CShoppingNote.h
CFilter.o: src/CFilter.cpp src/CFilter.h src/CNote.h
CNote.o: src/CNote.cpp src/CNote.h
CSearcher.o: src/CSearcher.cpp src/CSearcher.h src/CNote.h src/CFilter.h \
 src/CCompletedFilter.h src/CTagFilter.h src/CTextFilter.h
CShoppingNote.o: src/CShoppingNote.cpp src/CShoppingNote.h src/CNote.h
CTagFilter.o: src/CTagFilter.cpp src/CTagFilter.h src/CNote.h \
 src/CFilter.h
CTextFilter.o: src/CTextFilter.cpp src/CTextFilter.h src/CNote.h \
 src/CFilter.h
CTextNote.o: src/CTextNote.cpp src/CTextNote.h src/CNote.h
CToDoNote.o: src/CToDoNote.cpp src/CToDoNote.h src/CNote.h
main.o: src/main.cpp src/CApplication.h src/CNote.h src/CTextNote.h \
 src/CToDoNote.h src/CShoppingNote.h src/CFileHandler.h \
 src/CFileExporter.h src/CSearcher.h src/CFilter.h src/CCompletedFilter.h \
 src/CTagFilter.h src/CTextFilter.h


