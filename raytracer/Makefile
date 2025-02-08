# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra

# Target executable
TARGET = image_renderer

# Source files
SRCS = main.cpp

# Build target
all: $(TARGET)

# Link and create the executable
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

# Clean up build files
clean:
	rm -f $(TARGET)

image: 
	cmake --build build
	./build/image_renderer > image.ppm

# Run the program
run: $(TARGET)
	./$(TARGET)
