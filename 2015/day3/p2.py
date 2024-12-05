#       p2.py
#       
#       Kyle Shores
#       12/8/2015 (around then)
#
#       Last Edited: 12/28/2015
#
#       ^, go up one block
#       v, go down one block
#       >, go right one block
#       <, go left one block
#       each time santa visits a block, he delivers a present, as well as to his starting location
#       how many total presents are delivered?
#	this time a robot also delivers presents
#	how many places receive mulitple presents?
#	santa moves, then the robot



def main():
	with open("input.txt", "r") as f:
		svisited = dict()
		rvisited = dict()
		slocation = [0,0]
		rlocation = [0,0]
		svisited[tuple(rlocation)] = 1
		rvisited[tuple(slocation)] = 1

		contents = f.readlines()[0].strip()

		for s,i in enumerate(contents):
			if s % 2 == 0:
				if i == "^":
					slocation[1] += 1
				if i.lower() == "v":
					slocation[1] -= 1
				if i == ">":
					slocation[0] += 1
				if i == "<":
					slocation[0] -= 1
				svisited[tuple(slocation)] = 1
			else:
				if i == "^":
					rlocation[1] += 1
				if i.lower() == "v":
					rlocation[1] -= 1
				if i == ">":
					rlocation[0] += 1
				if i == "<":
					rlocation[0] -= 1
				rvisited[tuple(rlocation)] = 1

		print len(set(svisited.keys()).union(set(rvisited.keys())))
main()
