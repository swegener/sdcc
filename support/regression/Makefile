# Starting at the bottom
# Set of source test suites
# Each source suite is processesd producing multiple device specific test suites.
# Each device specific test suite is compiled.
# Each device specific test suite is run, and the output recorded.
# The output from each device specific test suite derrived from a source
# test suite are collated.

.SILENT:

CASES_DIR = cases
RESULTS_DIR = results
TESTS_DIR = tests
PORTS_DIR = ports
SUBRESULTS_DIR = subresults

GENERATE_CASES = generate-cases.py

ALL_PORTS = $(notdir $(wildcard $(PORTS_DIR)/*))

test-ports:
	for i in $(ALL_PORTS); do $(MAKE) inter-port-clean test-port PORT=$$i; done

ALL_TESTS = $(shell find $(TESTS_DIR) -name "*.c")

PORT_CASES_DIR = $(CASES_DIR)/$(PORT)
PORT_RESULTS_DIR = $(RESULTS_DIR)/$(PORT)
PORT_SUBRESULTS_DIR = $(SUBRESULTS_DIR)/$(PORT)
PORT_RESULTS = $(ALL_TESTS:$(TESTS_DIR)/%.c=$(PORT_RESULTS_DIR)/%.out)

# Defaults
SDCC = ../../bin/sdcc
SDCCFLAGS = -m$(PORT)
OBJEXT = .o
EXEEXT = .bin
DIREXT = 

ifdef PORT
include $(PORTS_DIR)/$(PORT)/spec.mk
endif

.PRECIOUS: $(PORT_CASES_DIR)/% %$(OBJEXT) %$(EXEEXT) %.dir

SDCCFLAGS += -Ifwk/include

$(PORT_CASES_DIR)/%$(DIREXT): $(TESTS_DIR)/%.c $(GENERATE_CASES)
	rm -rf $(CASES_DIR)/tests
	mkdir -p $(CASES_DIR)/tests
	mkdir -p $@
	python $(GENERATE_CASES) $< > /dev/null
	cp $(CASES_DIR)/tests/*.c $@
	touch $@

$(PORT_RESULTS_DIR)/%.out: $(PORT_CASES_DIR)/%$(DIREXT)
	$(MAKE) iterations PORT=$(PORT) CASES=$<

port-results: port-dirs $(PORT_RESULTS)
	echo Summary for \'$(PORT)\': `cat $(PORT_RESULTS) | python collate-results.py`

port-dirs:
	mkdir -p $(PORT_CASES_DIR) $(PORT_RESULTS_DIR) $(PORT_SUBRESULTS_DIR)

test-port: port-results

SUB_CASES = $(wildcard $(CASES)/*.c)
SUB_RESULTS = $(SUB_CASES:$(PORT_CASES_DIR)/%.c=$(PORT_SUBRESULTS_DIR)/%.out)
RESULTS = $(CASES:$(CASES_DIR)/%$(DIREXT)=$(RESULTS_DIR)/%.out)

iterations: $(RESULTS)

$(RESULTS): $(SUB_RESULTS)
	cat $(SUB_RESULTS) > $@

#$(PORT_CASES_DIR)/%.bin: $(PORT_CASES_DIR)/%.c

#$(PORT_CASES_DIR)/%.o: $(PORT_CASES_DIR)/%.c

clean:
	rm -rf $(CASES_DIR) $(RESULTS_DIR) $(SUBRESULTS_DIR)

inter-port-clean:
	rm -f  fwk/lib/*.o


