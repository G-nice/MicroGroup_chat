#ROOT := $(shell pwd)
#SRC := $(ROOT)/src
#SRC := $(ROOT)
#SUBDIRS := $(SRC)/server $(SRC)/client

SUBDIRS := ./server ./client

all: release

.PHONY: clean clean_debug clean_release

clean: clean_release clean_debug


debug:
	@for subdir in $(SUBDIRS); \
	do \
	    #@echo "making $@ in $$subdir"; \
	    ( cd $$subdir && make debug ) || exit 1; \
	done

release:
	
	@for subdir in $(SUBDIRS); \
	do \
	    #@echo "making $@ in $$subdir"; \
	    ( cd $$subdir && make release ) || exit 1; \
	done


clean_debug:
	@for subdir in $(SUBDIRS); \
	do \
	    #@echo "making $@ in $$subdir"; \
	    ( cd $$subdir && make clean_debug ) || exit 1; \
		rmdir -v $$subdir/obj/; \
	done
	
	


clean_release:
	@for subdir in $(SUBDIRS); \
	do \
	    #@echo "making $@ in $$subdir"; \
	    ( cd $$subdir && make clean_release ) || exit 1; \
		rmdir -v $$subdir/obj/; \
	done

