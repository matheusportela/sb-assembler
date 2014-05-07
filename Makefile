SUBDIRS = asm sim
CLEANDIRS = $(SUBDIRS:%=clean-%)

.PHONY: subdirs $(SUBDIRS)
subdirs: $(SUBDIRS)
$(SUBDIRS):
	@$(MAKE) -C $@

.PHONY: print
print:
	@echo Subdirs: $(SUBDIRS)
	
.PHONY: clean $(CLEANDIRS)
clean: $(CLEANDIRS)
$(CLEANDIRS):
	@$(MAKE) -C $(@:clean-%=%) clean