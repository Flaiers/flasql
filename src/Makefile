CC=gcc
CFLAGS=-Wall -Wextra -Werror
ROOT_DIR=$(shell dirname $(abspath $(MAKEFILE_LIST)))

.SILENT:

.PHONY: help
help: ## Display this help screen
	awk 'BEGIN {FS = ":.*##"; printf "Usage:\n  make \033[36m<target>\033[0m\n"} /^[a-zA-Z_-]+:.*?##/ { printf "  \033[36m%-14s\033[0m %s\n", $$1, $$2 } /^##@/ { printf "\n\033[1m%s\033[0m\n", substr($$0, 5) }' $(MAKEFILE_LIST)

.PHONY: run
run: ## Run DBMS
	$(ROOT_DIR)/../build/dbms

.PHONY: run-out
run-out: ## Run DBMS with write out
	$(ROOT_DIR)/../build/dbms > $(ROOT_DIR)/dbms.out

.PHONY: run-leaks
run-leaks: ## Run DBMS with leaks
	leaks -atExit -- $(ROOT_DIR)/../build/dbms

.PHONY: dbms
dbms: database.o master_levels.o master_modules.o master_status_events.o shared.o modules_db.o ## Build DBMS
	$(CC) $(CFLAGS) $(ROOT_DIR)/database.o $(ROOT_DIR)/master_levels.o $(ROOT_DIR)/master_modules.o $(ROOT_DIR)/master_status_events.o $(ROOT_DIR)/shared.o $(ROOT_DIR)/modules_db.o -o $(ROOT_DIR)/../build/dbms

database.o: $(ROOT_DIR)/database.c
	$(CC) $(CFLAGS) -c $(ROOT_DIR)/database.c -o $(ROOT_DIR)/database.o

master_levels.o: $(ROOT_DIR)/master_levels.c
	$(CC) $(CFLAGS) -c $(ROOT_DIR)/master_levels.c -o $(ROOT_DIR)/master_levels.o

master_modules.o: $(ROOT_DIR)/master_modules.c
	$(CC) $(CFLAGS) -c $(ROOT_DIR)/master_modules.c -o $(ROOT_DIR)/master_modules.o

master_status_events.o: $(ROOT_DIR)/master_status_events.c
	$(CC) $(CFLAGS) -c $(ROOT_DIR)/master_status_events.c -o $(ROOT_DIR)/master_status_events.o

modules_db.o: $(ROOT_DIR)/modules_db.c
	$(CC) $(CFLAGS) -c $(ROOT_DIR)/modules_db.c -o $(ROOT_DIR)/modules_db.o

shared.o: $(ROOT_DIR)/shared.c
	$(CC) $(CFLAGS) -c $(ROOT_DIR)/shared.c -o $(ROOT_DIR)/shared.o

.PHONY: clean
clean: ## Clean DBMS
	rm -rf $(ROOT_DIR)/*.o $(ROOT_DIR)/*.out $(ROOT_DIR)/../build/dbms
