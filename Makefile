TARGETS = main

all: $(TARGETS)

OBJS = read.o

CXXFLAGS = -O2 -g -W -Wall -Werror
CPPFLAGS = -I. -I/sw/include

main: $(OBJS)

clean: 
	$(RM) $(OBJS) $(TARGETS)

.PHONY: package
package:
	$(RM) -r pathing
	hg clone . pathing/
	tar czf pathing.tgz pathing
