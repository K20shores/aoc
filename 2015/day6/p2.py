#	p2.py
#
#	Kyle Shores
#	12/23/2015
#
#	Lights, lights, lights!	What is the total brightness of all of the lights

import re

def turn_on(lights, regex):
	start_i, start_j = regex.group(1).split(",")
	end_i, end_j = regex.group(2).split(",")

	for i in range( int(start_i), int(end_i) + 1 ):
		for j in range( int(start_j), int(end_j) + 1 ):
			lights[(i,j)] += 1


def turn_off(lights, regex):
	start_i, start_j = regex.group(1).split(",")
	end_i, end_j = regex.group(2).split(",")

	for i in range( int(start_i), int(end_i) + 1 ):
		for j in range( int(start_j), int(end_j) + 1 ):
			if lights[(i,j)] > 0:
				lights[(i,j)] -= 1


def toggle_lights(lights, regex):
	start_i, start_j = regex.group(1).split(",")
	end_i, end_j = regex.group(2).split(",")

	for i in range( int(start_i), int(end_i) + 1 ):
		for j in range( int(start_j), int(end_j) + 1 ):
			lights[(i,j)] += 2


def main():
	# -1 means off, 0 means on. This will be useful when the command for toggle is presented
	lights = { (i,j):0 for i in range(0,1000) for j in range(0,1000) }
	on = re.compile(r"turn on (\d*,\d*) through (\d*,\d*)")
	off = re.compile(r"turn off (\d*,\d*) through (\d*,\d*)")
	toggle = re.compile(r"toggle (\d*,\d*) through (\d*,\d*)")

	with open("input.txt", "r") as f:
		fcontents = f.readlines()
		for i in fcontents:
			i = i.strip()
			on_match = on.match(i)
			off_match = off.match(i)
			toggle_match = toggle.match(i)
			if on_match:
				turn_on(lights, on_match)
			if off_match:
				turn_off(lights, off_match)
			if toggle_match:
				toggle_lights(lights, toggle_match)

	print "The total brightness of the lights left on is %d." %(sum(lights.values()))


main()
