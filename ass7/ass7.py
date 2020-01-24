"""
Name: Adam Shay Shapira
ID: 316044809
Group:01
Assignment: 07
"""
import csv

LINK_SUFFIX = ".html"
LINK_TAG = "href"


def find_links(line_list):
	"""
	:param line_list: a list of lines
	:return: links: a list of links end with a link suffix
	"""
	links = set()
	for line in line_list:
		line = line.split('"')
		line = filter(lambda word: word.endswith(LINK_SUFFIX), line)
		for link in line:
			links.add(link)
	return list(links)


def parse_page(page):
	"""
	:param page: a page to search links in
	:return: name: the name of the file
	:return: link_list: a list of of all the lines with link tag in them
	"""
	with open(page) as file:
		lines = file.readlines()
		link_list = list(filter(lambda line: LINK_TAG in line, lines))
		links = find_links(link_list)
		return links


def crawler(link, dic):
	"""
	:param link: string with a name of a file to open
	:param dic: dictionary to write to
	:return: the function open the file and adds in a recursive way all the links inside of it
	"""
	if link in dic:
		return
	links = parse_page(link)
	dic[link] = links
	for link in dic[link]:
		if link not in dic:
			crawler(link, dic)


def create_csv(dic):
	"""
	:param dic: a dictionary object
	:return: the function takes the dictionary and create a results.csv file with the dictionary content
	"""
	with open("results.csv", mode='w', newline='') as file:
		writer = csv.writer(file)
		for key in dic:
			line = [key, ]
			line.extend(dic[key])
			writer.writerow(line)


dictionary = {}
new_link = input("enter source file:\n")
crawler(new_link, dictionary)
create_csv(dictionary)
new_link = input("enter file name:\n")
dictionary[new_link].sort()
print(dictionary[new_link])
