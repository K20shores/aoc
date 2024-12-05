#	p1.py
#
#	Kyle Shores
#	1/3/2016
#
#	Last edited: 1/4/2016
#
#	What is Santa's next password?
#	-must contain a run of three letters, eg. "abc", "bcd", "cde", ...
#	-must not contain the letters "i", "l", or "o"
#	-must conatin at least two pairs which cannot overlap
#	-is incremented, eg. "aa" becomes "ab", and "az" becomes "aa"



#return the next character in the alphabet, loop around from "z" to "a"
def nxt(c):
	if ord(c) == ord("z"):
		return "a"
	else:
		return chr(ord(c) + 1)





#check for at least two pairs of letters
def pairs(s):
	count = 0
	index = 0
	while index < len(s) - 1:
		if s[index] == s[index + 1]:
			count += 1
			index += 2
		else:
			index += 1
	if count >= 2:
		return True
	return False




#check to see if there are any consecutive letters
def run(s):
	for index, c in enumerate(s):
		if index < len(s) - 3:#make sure we can access all characters of the string up to the last character
			if nxt(c) == s[index + 1] and nxt(s[index + 1]) == s[index + 2]:
				return True
	return False 





#return False if any illegal characters are present
def illegal_chars(s):
	if "i" in s or "l" in s or "o" in s:
		return False
	return True


def increment(s):
	edit_this = -1
	while not pairs(s) or not run(s) or not illegal_chars(s):
		if -1 * edit_this == len(s) and s[edit_this] == "z":
			s[edit_this] = nxt(s[edit_this])	
			edit_this = -1
			continue
		if s[edit_this] == "z":
			s[edit_this] = nxt(s[edit_this])
			edit_this -= 1
		else:		
			s[edit_this] = nxt(s[edit_this])

def main():
	start = [x for x in "cqjxjnds"]

	increment(start)
	
	print "The next password should be %s" %(''.join(start))
main()
