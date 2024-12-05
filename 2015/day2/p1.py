#       p1.py
#       
#       Kyle Shores
#       12/8/2015 (around then)
#
#       Last Edited: 12/28/2015
#
#       find the total square footage the elves need to wrap the presents

def main():
	total=0
	with open("input.txt", "r") as f:
		contents = [x.strip() for x in f.readlines()]
		for i in contents:
			i =[int(x) for x in i.split("x")]
			footage = [2*i[0]*i[1], 2*i[0]*i[2], 2*i[1]*i[2]]
			total += sum(footage) + min(footage) / 2
	print "The total number of square feet of wrapping paper is %d" %(total)
main()
