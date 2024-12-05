#	p1.py
#
#	Kyle Shores
#	1/3/2016
#	
#	Last Edited: 1/3/2016
#
#
#	Look and say number generator
#	What is the length of the number after the 40'th iteration?


def main():
	start = "1321131112"	#provided by the adventofcode.com to me

	run = 0
	end_after = 50		#how many runs?
	while run < end_after:
		new = ""
		index = 0	
		while index < len(start):
			count = 1
			while count + index <= len(start) - 1 and start[index] == start[index + count]:
				count += 1
			else:
				new += str(count) + start[index]	#add the number of occurence of the current digit to the new string
				index += count
		else:
			run += 1
			start = new

	print "The length of the final number is %d digits. " %(len(start))
main()
