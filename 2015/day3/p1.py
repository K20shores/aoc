#       p1.py
#       
#       Kyle Shores
#       12/8/2015 (around then)
#
#       Last Edited: 12/28/2015
#
#	^, go up one block
#	v, go down one block
#	>, go right one block
#	<, go left one block
#	each time santa visits a block, he delivers a present, as well as to his starting location
#	how many total presents are delivered?

def main():
	with open("input.txt", "r") as f:
		visited = dict()
		location = [0,0]
		visited[tuple(location)] = 1
		contents = f.readlines()[0].strip()

		for i in contents:
			if i == "^":
				location[1] += 1
			if i.lower() == "v":
				location[1] -= 1
			if i == ">":
				location[0] += 1
			if i == "<":
				location[0] -= 1
			visited[tuple(location)] = 1

		print len(visited.keys())
main()
