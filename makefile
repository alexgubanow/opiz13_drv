default: all
	
.PHONY: clean all help
MAKE_FLAGS = -j4 

opiz13_drv:
	$(MAKE) $(MAKE_FLAGS) -C src
install: opiz13_drv
	$(MAKE) install -C src
test: install
	$(MAKE) $(MAKE_FLAGS) -C test

all: opiz13_drv install test

clean:
	$(MAKE) -C src clean
	$(MAKE) -C test clean

-include $(OBJS:%.o=%.d)