#	p1.py
#
#	Kyle Shores
#	12/23/2015
#
#	Day5, problem 1 of advent of code. Identify "nice" strings from the given input with the following specifications:
#	-must contain at least three vowels
#	-must contain at least 1 letter that appears twice in a row
#	-must not containt the strings "ab, cd, pq, or xy"



#should return true if the string meets the specifications for vowels
def vowels(item):
	vowels = {x:0 for x in "aeiou"}
	for i in item:
		if i in vowels.keys():	#is the letter a vowel? (yes, increase its count by one)
			vowels[i] += 1
	if sum(vowels.values()) >= 3:	#did at least three vowels appear (yes, this may be a nice string)
		return True
	else:
		return False


#should return True if the string meets the specifications of repeats
def repeats(item):
	for index, i in enumerate(item):
		if index != len(item) - 1 and i == item[index + 1]:
			return True
	return False



#return True if the string is legal
def illegal(item):
	nope = ["ab", "cd", "pq", "xy"]
	for i in nope:
		if i in item:
			return False
	return True



def main():
	count = 0
	with open("input.txt") as f:
		fcontents = f.readlines()
		for i in fcontents:
			i = i.strip()
			if vowels(i) & repeats(i) & illegal(i):
				count += 1
	print "The number of nice strings in the input is %d" %(count)

main()
