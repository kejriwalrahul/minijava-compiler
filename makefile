all:
	$(MAKE) -C pass_1
	$(MAKE) -C pass_2
	$(MAKE) -C pass_3
	$(MAKE) -C pass_4
	$(MAKE) -C pass_5
	$(MAKE) -C pass_6

clean:
	$(MAKE) -C pass_1 clean
	$(MAKE) -C pass_2 clean
	$(MAKE) -C pass_3 clean
	$(MAKE) -C pass_4 clean
	$(MAKE) -C pass_5 clean
	$(MAKE) -C pass_6 clean
	rm -rf tmp/*