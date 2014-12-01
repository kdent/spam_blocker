
CC=cc

spam_blocker:
	cd src; make spam_blocker

tests:
	cd test; make

clean:
	cd src; make clean
	cd test; make clean
