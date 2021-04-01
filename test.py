#!/usr/bin/python3

import os
import itertools

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

test_idx = 1

# run single test
def test(path, pattern, options):

	global test_idx

	# build mygrep and grep commands
	mygrep_command = "./my_grep"
	grep_command = "grep"
	pattern_path = " {} {}".format(pattern, path)
	options_str = ""
	for opt in options:
		options_str += " " + opt

	# print log
	log = YELLOW + "[%4d] " %test_idx + NC + mygrep_command + options_str + pattern_path
	log = log.ljust(75)
	print(log, end=' ')

	# direct to an output file
	mygrep_command += options_str + pattern_path + " > mygrep_output"
	grep_command += options_str + pattern_path + " > grep_output"
	
	# run my_grep
	mygrep_status = os.system(mygrep_command)
	if (mygrep_status != 0):
		print(RED + " [mygrep fail {}]".format(mygrep_status) + NC)
		exit()

	# run grep
	grep_status = os.system(grep_command) >> 8
	if (grep_status == 2):
		print(RED + " [grep fail {}]".format(grep_status) + NC)
		exit()

	# comapre results - exit if no match
	cmp_command = "cmp -s mygrep_output grep_output"
	cmp_status = os.system(cmp_command)
	if (cmp_status != 0):
		print(RED + " [fail]" + NC)
		exit()

	# test passed - remove output files
	os.system("rm mygrep_output")
	os.system("rm grep_output")
	print(GREEN + " [pass]" + NC)
	test_idx += 1

	return 0

# create permutations of options for grep/mygrep
def permutations_combinations(alist):
	result = []
	for i in range(len(alist)):
		temp_list = list(itertools.combinations(alist, i))
		for item in temp_list:
			result.append(item)
	return result


file0 = "file0"
file1 = "file1"
options_arr = ["-A 3", "-b", "-c", "-i", "-n", "-v", "-x"]
strs = ["dontfind", "chaim", "CHAIM", "\"is chaim\""]
regex_strs = ["\"(my name is moshe|moshe)[h-m]\"", "\"\\\\text \"", "\"escaping \| chars\"", "\"(name is moshe|moshe)\"", "\"m[A-Z] name\"", "\"(CHAIM|moshe)\"", "\"m[a-z] na.e is (CHaim|moshe)\"", "\"m[a-z] na.e is \[]\"", "\"\[\]\"", "\"for\.\"", "\"\( some\)\""]
opts = permutations_combinations(options_arr)

# test an empty file (sanity)
test(file0, "chaim", [])

# run multiple tests different patterns and options
for optper in opts:
	for astr in strs:
		result = test(file0, astr, optper)
	for rstr in regex_strs:
		result = test(file0, rstr, optper + ('-E',))
