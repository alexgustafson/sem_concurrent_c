# Automatically generated makefile, created by the Introjucer
# Don't edit this file! Your changes will be overwritten when you re-save the Introjucer project!

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(CONFIG),Debug)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Debug
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  CPPFLAGS := $(DEPFLAGS) -std=c++11 -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../../common
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0 -D__USE_UNIX98
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -L/usr/X11R6/lib/ 

  TARGET := server
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

ifeq ($(CONFIG),Release)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Release
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  CPPFLAGS := $(DEPFLAGS) -std=c++11 -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../../common
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -O3 -D__USE_UNIX98
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -fvisibility=hidden -L/usr/X11R6/lib/ 

  TARGET := server
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

OBJECTS := \
  $(OBJDIR)/field_manager_8182bcc8.o \
  $(OBJDIR)/tcp_server_c50d2b8b.o \
  $(OBJDIR)/Main_4c9ac042.o \

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(RESOURCES)
	@echo Linking server
	-@mkdir -p $(BINDIR)
	-@mkdir -p $(LIBDIR)
	-@mkdir -p $(OUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning server
	@$(CLEANCMD)

strip:
	@echo Stripping server
	-@strip --strip-unneeded $(OUTDIR)/$(TARGET)

$(OBJDIR)/field_manager_8182bcc8.o: ../../../common/field_manager.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling field_manager.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/tcp_server_c50d2b8b.o: ../../../common/tcp_server.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling tcp_server.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Main_4c9ac042.o: ../../Source/Main.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Main.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
