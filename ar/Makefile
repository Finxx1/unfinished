# Makefile for Arcane Rip-off
# Requires VCPKG with GLFW3, GLAD, and STB installed.

APP = arcane

VCPKG_DIR = $(VCPKG_ROOT)/installed/x64-*/
VCPKG_INC = $(VCPKG_DIR)/include/
VCPKG_LIB = $(VCPKG_DIR)/lib/

LIBS = -lm -lglfw3 -lglad

CFLAGS = -c -Wall -Wextra -I $(VCPKG_INC)
LDFLAGS = -L $(VCPKG_LIB) $(LIBS)
CC = cc

all: clean arcane

arcane: objs
	$(CC) obj/*.o $(LDFLAGS) -o $(APP)

clean:
	rm -f obj/*.o

include src/build.mk

