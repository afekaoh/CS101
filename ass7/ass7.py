"""
Name: Adam Shay Shapira
ID: 316044809
Group:01
Assignment 07
"""
import csv

HTML_SUFFIX = ".html"
FIND_LINK = "href"


def is_link_line(line):
	if FIND_LINK in line:
		return True
	return False


def is_link(line):
	return str(line).endswith(HTML_SUFFIX)


def read_new_page(link):
	with open(link) as f:
		data = f.readlines()
		name = f.name
		d = list(filter(is_link_line, data))
		return name, d


def find_links(page):
	links = list()
	for line in page:
		line = line.split('"')
		line = list(filter(is_link, line))
		for link in line:
			if link not in links:
				links.append(link)
	return links


def crawler(link, dic):
	file_name, new_data = read_new_page(link)
	links = find_links(new_data)
	dic[file_name] = links
	for link in links:
		if link not in dic:
			crawler(link, dic)


def create_csv(dic):
	with open("result.csv", mode='w', newline='') as file:
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
