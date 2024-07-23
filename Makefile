SRC		=	$(addprefix src/,	main.cpp\
								glad.c\
								Renderer.cpp\
								VertexBuffer.cpp\
								IndexBuffer.cpp\
								VertexArray.cpp\
								UniformBuffer.cpp\
								Shader.cpp\
								stb_image.cpp\
								Texture.cpp\
								ParsedObject.cpp\
								ObjectRenderer.cpp\
								KeyboardManager.cpp\
								BmpLoader.cpp\
								nrg_math.cpp\
								)
OBJDIR	=	obj
SRCDIR	=	src
OBJ		=	$(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(filter %.cpp, $(SRC))) \
			$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(filter %.c, $(SRC)))
CC		=	clang++
LIBFLAGS=	-L./lib -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
CFLAGS	=	--std=c++17 -Wno-deprecated -MD -MP -I include -I include/imgui
# CFLAGS	=	--std=c++17 -Wno-deprecated -Wall -Wextra -Werror -MD -MP -I include -I include/imgui
DEPENDS	=	$(OBJ:.o=.d)
NAME	=	opengl_test
.PHONY	:	all re clean fclean

all	: $(NAME)

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME)	: $(OBJ) Makefile
	@echo "Linking $@"
	@$(CC) $(CFLAGS) $(LIBFLAGS) $(OBJ) -o $(NAME)

clean	:	
	rm -f $(OBJ) $(DEPENDS)
	# rm -rf $(OBJDIR)

fclean	: clean
	rm -f $(NAME)

x : all
	./$(NAME)


leaks			:	all
					leaks --atExit -- ./$(NAME)

-include $(DEPENDS)
