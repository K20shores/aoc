#	p1.py
#
#	Kyle Shores
#	12/28/2015
#
#	Last Edited: 12/28/2015
#
#	Find the shortest path between a set number of cities (an Euler path).
#	
#	Definitely my least elegant solution, but it works. 


import re

def fill(city_connections, distances):
	with open("input.txt", "r") as f:
		fcontents =[i.strip() for i in f.readlines()]
		
		path = re.compile(r"(\w*) to (\w*) = (\d*)")
		for i in fcontents:
			match = path.match(i)
			city_connections[match.group(1)] = city_connections.get(match.group(1), []) + [match.group(2)]
			city_connections[match.group(2)] = city_connections.get(match.group(2), []) + [match.group(1)]
			distances[match.group(1)+'-'+match.group(2)] = int(match.group(3))
			distances[match.group(2)+'-'+match.group(1)] = int(match.group(3))




		
def find_distance(distances, route):
	total = 0
	for i in range(len(route) -1):
		total += distances[route[i]+"-"+route[i+1]]
	return total


def visited_all(city_connections, path):
	k = city_connections.keys()
	for i in k:
		if i not in path:
			return False
	return True

def find_city(path, location):
	for i in range(-1, -1 * len(path), -1):
		if path[i] == location:
			return i
	


def find_path(city_connections, start, routes, path = []):
	path += [start]
	if visited_all(city_connections, path):
		return

	
	for location in city_connections[start]:
		if location in path:
			continue
		else:	
			find_path(city_connections, location, routes, path)
			if visited_all(city_connections, path):
				routes[tuple(path)] = 0
				to_here = find_city(path, location)
				path = path[:to_here]
	return

def main():
	distances = {}		#will hold distances
	city_connections = {}	#will hold which cities connect to which
	fill(city_connections, distances)
	
	routes = {}
	for start in city_connections.keys():
		find_path(city_connections, start, routes, [])
	
	for i in routes:
		routes[i] = find_distance(distances, i)
	print "The distance of the longest route is %d." %(max(routes.values()))

main()
