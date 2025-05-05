CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I. -I../designPattern
LDFLAGS = 

# Source files
SRCS = main.cpp \
       ../designPattern/Strategy/strategy_lexer.cpp \
       ../designPattern/TokenType/token_type.cpp \
       ../designPattern/BoolHandling/BoolHandling.cpp \
       ../designPattern/CharHandling/NewlineHandling.cpp \
       ../designPattern/CharHandling/CharHandling.cpp \
       ../designPattern/CommentHandling/MultiLineComment.cpp \
       ../designPattern/CommentHandling/SingleLineComment.cpp \
       ../designPattern/InputTypeHandling/InputHandling.cpp \
       ../designPattern/KeywordHandler/KeywordHandling.cpp \
       ../designPattern/NumberHandling/NumberHandling.cpp \
       ../designPattern/OperatorHandling/OperatorHandling.cpp \
       ../designPattern/SourceHandling/strategy_sourcestream.cpp \
       ../designPattern/StringHandling/string_literal_strategy.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Main target
TARGET = lexer

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Rule to compile .cpp files to .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)

# Run rule
run: $(TARGET)
	./$(TARGET) test.txt

.PHONY: all clean run