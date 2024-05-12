CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -lcurl


SRCDIR = src
INCDIR = includes

SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(SRCS:.cpp=.o)
DEPS = $(wildcard $(addprefix $(INCDIR)/, *.h)) # Modified line

#gcc flags are order sensitive  test.c is appeard before curl's internal  
#e.g gcc -lcurl test.c will fail but gcc test.c -lcurl so add cflgs add last
main: $(OBJS)
	$(CXX)  -o $@ $^ $(CXXFLAGS)

%.o: $(SRCDIR)/%.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f main $(SRCDIR)/*.o