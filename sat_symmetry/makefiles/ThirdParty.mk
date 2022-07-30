
third_party: bliss saucy

bliss:
	$(MAKE) -C third_party/automorphism/bliss/ lib

saucy:
	$(MAKE) -C third_party/automorphism/saucy/


clean-third_party:
	$(MAKE) -C third_party/automorphism/bliss/ clean
	$(MAKE) -C third_party/automorphism/saucy/ clean

.PHONY: third_party
