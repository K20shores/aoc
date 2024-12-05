#	p1.py
#
#	Kyle Shores
#	12/24/2015
#
#	Process string literals versus the space taken up by those string literals.
#	What is the difference between string literals in code and decoded string literals?


def main():
	with open("input.txt", "r") as f:
		literal = [i.strip() for i in f.readlines()]
		escaped = [i.decode("string_escape") for i in literal]
		literal = [len(i) for i in literal]
		escaped = [len(i)-2 for i in escaped]	#-2 ensures that we are not counting the quotes around the string
		difference = [literal[i] - escaped[i] for i in range(len(literal))]
		print "The difference in the string literals and their space in memory is %d." %(sum(difference))
main()
