#	p1.py
#	
#	Kyle Shores
#	12/8/2015 (around then)
#
#	Last Edited: 12/28/2015
#
#	"(" says santa should go up a floor
#	")" says santa should go down a floor
#	Which floor does santa end up on?

def main():
	with open("input.txt", "r") as f:
		l = [i.strip() for i in f.readlines()]
		s = ''.join(l)
		o = s.count("(")
		c = s.count(")")
		print "Santa should go to floor %d" %(o-c)
main()
