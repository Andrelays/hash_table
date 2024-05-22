CXX = g++
LD  = g++

CXXFLAGS = -g -O3 -msse4.1 -msse4.2 -mavx2 -mavx
LDFLAGS =
OBJDIR  = obj
SOURCES = main.o list.o hash_table.o hash_func.o lib.o

release: $(SOURCES) link

link: $(SOURCES)
		@echo [LD] [LDFLAGS] [CXXFLAGS] $(addprefix $(OBJDIR)/, $(^F)) $(LDFLAGS) -o release
		@$(LD) $(LDFLAGS) $(CXXFLAGS) $(addprefix $(OBJDIR)/, $(^F)) $(LDFLAGS) -o release

%.o: %.cpp
		@echo [CXX] [CXXFLAGS] $< -o $(OBJDIR)/$(@F)
		@$(CXX) $(CXXFLAGS) -c $< -o $(OBJDIR)/$(@F)

clean:
		rm -rf $(OBJDIR)/*.o
