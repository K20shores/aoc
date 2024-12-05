#       p2.py
#       
#       Kyle Shores
#       12/8/2015 (around then)
#
#       Last Edited: 12/28/2015
#
#       "(" says santa should go up a floor
#       ")" says santa should go down a floor
#       When is the first time santa will get to the basement? (when he reaches floor -1)

def main():
	with open("input.txt", "r") as f:
		l =''.join([i.strip() for i in f.readlines()])
		c = 0
		for i in range(0,len(l)):
			if l[i] == "(":
				c += 1
			else:
				c -= 1
			if c == -1:
				print "Santa can get to the basement in %d moves" %(i+1)
				break
main()
