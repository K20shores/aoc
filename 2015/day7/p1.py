#	p1.py	
#
#	Kyle Shores
#	12/24/2015
#
#	Last Edited: 3/26/2016
#
#
#	On the eve of Christmas, I attempt to model a circuit in a python script.
#	I honestly have no idea how i'm going to do this. So, this will be fun.
#
#	Ok, that didn't happen.^^^ I didn't know where to start. So I thought about it for 2 weeks.
#
#	^^and then had to go back to college. Begin easter weekend attempt
import re




def fill(instructions, symbol_vals):
	instruct = re.compile(r"(\w* \w* \w*) -> (\w*)")
	init_vals = re.compile(r"(\w*) -> (\w*)")
	with open("input.txt", "r") as f:
		fcontents = [i.strip() for i in f.readlines()]
		for i in fcontents:
			m = instruct.match(i)	#find all operations that set a symbol value
			if m:
				instructions[m.group(2)] = instructions.get(m.group(2), []) + [m.group(1)]
			else:
				print "match not found for\n" + i
			v = init_vals.match(i)	#find any initial values
			if v:
				symbol_vals[v.group(2)] = v.group(1)
			

def has_value(symbol):
	"""Returns true if symbol is any type of number"""
	try:
		int(symbol)
		return True
	except ValueError:
		return False


def process(instructions, symbol_vals):
	"""Model the circuit and set all of the values"""

	NOT = re.compile(r"NOT (\w*)")
	AND = re.compile(r"(\w*) AND (\w*)")
	OR = re.compile(r"(\w*) OR (\w*)")
	LSHIFT = re.compile(r"(\w*) LSHIFT (\w*)")
	RSHIFT = re.compile(r"(\w*) RSHIFT (\w*)")

	for i in symbol_vals:
		if has_value(symbol_vals[i]) and i != 0:
			symbol_vals[i] = int(symbol_vals[i])
#		print i, symbol_vals[i]
	
#	for i in instructions:
#		print i, instructions[i]	


def main():
	""" Model a circuit."""
	
	instructions = {}
	symbol_vals = {}

	#fill a dictionary with all instructions for a variable as it's values, the variable as the key
	#initialize the symbol list
	fill(instructions, symbol_vals)

	process(instructions, symbol_vals)


#	for i in symbol_vals.keys():
#		print i, symbol_vals[i]



main()
