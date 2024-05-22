CXX = g++
LD  = g++

CXXFLAGS =  -O3 -mavx -mavx2 -lSDL2 -lSDL2_ttf -Wno-pointer-arith -ggdb3 -std=c++17 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wstack-usage=8192 -pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
LDFLAGS = 	libraries/Stack/libfile.a libraries/Onegin/libfile.a
OBJDIR  = obj
SOURCES = main.o list.o hash_table.o hash_func.o

release: $(SOURCES) link

link: $(SOURCES)
		@echo [LD] [LDFLAGS] [CXXFLAGS] $(addprefix $(OBJDIR)/, $(^F)) $(LDFLAGS) -o release
		@$(LD) $(LDFLAGS) $(CXXFLAGS) $(addprefix $(OBJDIR)/, $(^F)) $(LDFLAGS) -o release

%.o: %.cpp
		@echo [CXX] [CXXFLAGS] $< -o $(OBJDIR)/$(@F)
		@$(CXX) $(CXXFLAGS) -c $< -o $(OBJDIR)/$(@F)

clean:
		rm -rf $(OBJDIR)/*.o
