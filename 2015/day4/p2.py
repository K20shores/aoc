#	p2.py
#
#	Kyle Shores
#	12/22/2015
#
#
#	Day 4, problem 2 for the advent of code.
#
#
#	Find the MD5 hash value for which the input "iwrupvqb" concatenated with the smallest
#	decimal number that produces a hex value with at least six zeros.

import md5


def solve(key):
	num = 0
	m = md5.new()
	m.update(key + str(num))
	while m.hexdigest()[:6] != "000000":
		num += 1
		del m
		m = md5.new()
		m.update(key+str(num))
	else:
		return [num, m.hexdigest()]

def main():
	key = "iwrupvqb"	#input from advent of code
	
	answer = solve(key)	#a list, [decimal value, full hex answer]

	print "The decimal number %d concatenated with %s produces the smallest hex value with at\nleast 5 leading zeros of %s." %(answer[0], key, answer[1])
main()
