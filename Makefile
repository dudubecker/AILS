.PHONY: clean All

All:
	@echo "----------Building project:[ AILS_OOP - Release ]----------"
	@cd "AILS_OOP" && "$(MAKE)" -f  "AILS_OOP.mk"
clean:
	@echo "----------Cleaning project:[ AILS_OOP - Release ]----------"
	@cd "AILS_OOP" && "$(MAKE)" -f  "AILS_OOP.mk" clean
