#
# Top makefile
#

CROSS_COMPILE	?=
DESTDIR			?=
PREFIX			?=

AS			:= $(CROSS_COMPILE)as
CC			:= $(CROSS_COMPILE)cc
CXX			:= $(CROSS_COMPILE)c++
LD			:= $(CROSS_COMPILE)ld
AR			:= $(CROSS_COMPILE)ar
OC			:= $(CROSS_COMPILE)objcopy
OD			:= $(CROSS_COMPILE)objdump
RM			:= rm -fr

ASFLAGS		:= -g -ggdb -Wall -O3
CFLAGS		:= -g -ggdb -Wall -O3
CXXFLAGS	:= -g -ggdb -Wall -O3
LDFLAGS		:=
ARFLAGS		:= -rcs
OCFLAGS		:= -v -O binary
ODFLAGS		:=
MCFLAGS		:=

LIBDIRS		:=
LIBS 		:= `pkg-config --libs libusb-1.0`

INCDIRS		:= -I . `pkg-config --cflags libusb-1.0`
SRCDIRS		:= .


SFILES		:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))
CFILES		:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))
CPPFILES	:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.cpp))

SDEPS		:= $(patsubst %, %, $(SFILES:.S=.o.d))
CDEPS		:= $(patsubst %, %, $(CFILES:.c=.o.d))
CPPDEPS		:= $(patsubst %, %, $(CPPFILES:.cpp=.o.d))
DEPS		:= $(SDEPS) $(CDEPS) $(CPPDEPS)

SOBJS		:= $(patsubst %, %, $(SFILES:.S=.o))
COBJS		:= $(patsubst %, %, $(CFILES:.c=.o))
CPPOBJS		:= $(patsubst %, %, $(CPPFILES:.cpp=.o))
OBJS		:= $(SOBJS) $(COBJS) $(CPPOBJS)

OBJDIRS		:= $(patsubst %, %, $(SRCDIRS))
NAME		:= xrock
VPATH		:= $(OBJDIRS)

.PHONY:		all install clean

all : $(NAME)

$(NAME) : $(OBJS)
	@echo [LD] Linking $@
	@$(CC) $(LDFLAGS) $(LIBDIRS) $^ -o $@ $(LIBS)

$(SOBJS) : %.o : %.S
	@echo [AS] $<
	@$(AS) $(ASFLAGS) -MD -MP -MF $@.d $(INCDIRS) -c $< -o $@

$(COBJS) : %.o : %.c
	@echo [CC] $<
	@$(CC) $(CFLAGS) -MD -MP -MF $@.d $(INCDIRS) -c $< -o $@

$(CPPOBJS) : %.o : %.cpp
	@echo [CXX] $<
	@$(CXX) $(CXXFLAGS) -MD -MP -MF $@.d $(INCDIRS) -c $< -o $@

install:
	install -Dm0755 xrock $(DESTDIR)$(PREFIX)/bin/xrock
	install -Dm0644 99-xrock.rules $(DESTDIR)$(PREFIX)/etc/udev/rules.d/99-xrock.rules
	install -Dm0644 LICENSE $(DESTDIR)$(PREFIX)/share/licenses/xrock/LICENSE
	@if [ "$(shell uname)" = "Linux" ]; then echo "Reloading udvadm rules"; udevadm control --reload; fi

clean:
	@$(RM) $(DEPS) $(OBJS) $(NAME).exe $(NAME) *~
