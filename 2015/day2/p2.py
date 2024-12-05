#       p2.py
#       
#       Kyle Shores
#       12/8/2015 (around then)
#
#       Last Edited: 12/28/2015
#
#	find the total squrae feet of wrappng paper and total feet of ribbon


def main():
	total_paper = 0
	total_ribbon = 0
	with open("input.txt", "r") as f:
		contents = [x.strip() for x in f.readlines()]
		for i in contents:
			i =[int(x) for x in i.split("x")]
			footage = [2*i[0]*i[1], 2*i[0]*i[2], 2*i[1]*i[2]]
			total_paper += sum(footage) + min(footage) / 2
			total_ribbon += i[0]*i[1]*i[2] + min([2*i[0]+2*i[1], 2*i[0]+2*i[2], 2*i[1]+2*i[2]])
	print "The total number of square feet of wrapping paper is %d.\nThe total length of ribbon is %d feet." %(total_paper, total_ribbon)
main()
