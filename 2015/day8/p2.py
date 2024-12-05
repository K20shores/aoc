#	p1.py
#
#	Kyle Shores
#	12/24/2015
#
#	Process string literals versus the space taken up by those string literals.
#	What is the difference between encoded string literals with all possible escape charachters and the literals without all possible escape characters?


def main():
	with open("input.txt", "r") as f:
		literal = [i.strip() for i in f.readlines()]
		encoded = ["\""+i.encode("string_escape").replace("\"", "\\\"")+"\"" for i in literal]
		literal = [len(i) for i in literal]
		encoded = [len(i) for i in encoded]
		difference = [encoded[i] - literal[i] for i in range(len(literal))]
		print "The difference in the encoded string literals and their respective string literals is %d." %(sum(difference))
main()
