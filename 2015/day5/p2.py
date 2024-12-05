#	p2.py
#
#	Kyle Shores
#	12/23/2015
#
#	Day5, problem 2 of advent of code. Identify "nice" strings from the given input with the following specifications:
#	-must contain a pair of two letters that appear at least twice that do not overlap "aabaa" is fine but "aaa" is not fine, "aa" overlaps in the second example
#	-must contain at least 1 letter that repeats, but is sepearated by another letter eg. "efe" and "aaa" are both fine

def double_letter_sandwhich(item):
	check = [x for x in item if item.count(x) >= 2]
	#find all elements which appear at least 4 times
	for letter in check:
		index = 0
		while index < len(check) - 1:	#step through all of the letters that appeared at least twice, 
						#see if the current letter and any other letter that appeared at least twice
						#appears as a pair at least twice
			if item.count(letter + check[index]) >= 2:
				return True
			else:
				index += 1
	return False
	



def single_letter_sandwhich(item):
	for index, i in enumerate(item):
		if (index <= len(item) - 3) and i == item[index + 2]:
			return True
	return False




def main():
	count = 0
	with open("input.txt") as f:
		fcontents = f.readlines()
		for i in fcontents:
			i = i.strip()
			if single_letter_sandwhich(i) & double_letter_sandwhich(i):
				count += 1
	print "The number of nice strings in the input is %d" %(count)

main()
