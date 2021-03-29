import os
import itertools

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

test_idx = 1

def test(path, pattern, options):

	global test_idx

	mygrep_command = "./mygrep"
	grep_command = "grep"
	pattern_path = " {} {}".format(pattern, path)
	options_str = ""
	for opt in options:
		options_str += " " + opt

	log = YELLOW + "[%4d] " %test_idx + NC + mygrep_command + options_str + pattern_path
	log = log.ljust(75)

	mygrep_command += options_str + pattern_path + " > mygrep_output"
	grep_command += options_str + pattern_path + " > grep_output"
	
	mygrep_status = os.system(mygrep_command)
	if (mygrep_status != 0):
		log += RED + " [mygrep fail {}]".format(mygrep_status) + NC
		print(log)
		return -1

	grep_status = os.system(grep_command)
	if (grep_status == 2):
		log += RED + " [grep fail {}]".format(grep_status) + NC
		print(log)
		return -1

	cmp_command = "cmp -s mygrep_output grep_output"
	cmp_status = os.system(cmp_command)
	if (cmp_status != 0):
		log += RED + " [fail]" + NC
		print(log)
		return -1

	os.system("rm mygrep_output")
	os.system("rm grep_output")
	log += GREEN + " [pass]" + NC
	print(log)
	test_idx += 1

	return 0


file0 = "file0"
file1 = "file1"

def permutations_combinations(alist):
	result = []
	for i in range(len(alist)):
		temp_list = list(itertools.combinations(alist, i))
		for item in temp_list:
			result.append(item)
	return result


options_arr = ["-A 2", "-b", "-c", "-i", "-n", "-v", "-x"]
strs = ["dontfind", "chaim", "CHAIM", "\"is chaim\""]
opts = permutations_combinations(options_arr)

test(file0, "chaim", [])
test(file1, "chaim", [])
test(file1, "\"is chaim\"", [])
test(file1, "chaim", ["-c"])
test(file1, "\"m. name\"", ["-E"])
test(file1, "\"now\{ ad\}ding\"", ["-E"])
test(file1, "\"\( some\)\"", ["-E"])
test(file1, "\"\\\\text \"", ["-E"])
test(file1, "\"for\.\"", ["-E"])
test(file1, "\"\[\]\"", ["-E"])
test(file1, "\"escaping \| chars\"", ["-E"])
test(file1, "\"m[a-z] name\"", ["-E"])
test(file1, "\"m[A-Z] name\"", ["-E"])
test(file1, "\"m[A-Z] name\"", ["-E", "-i"])
test(file1, "\"m[A-Z] name is (chaim|moshe)\"", ["-E", "-i"])


for optper in opts:
	for astr in strs:
		result = test(file0, astr, optper)
		if result < 0:
			exit()
