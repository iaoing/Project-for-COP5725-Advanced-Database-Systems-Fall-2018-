import matplotlib.pyplot as plt

def func1():
	fd = open("build_index_time_cost.log", "r")
	wfd = open("build_index_time_cost.ana.log", "w")
	n_lines = 0;
	x_r = []
	y_r = []
	for line in fd:
		n_lines += 1
		# if(n_lines > 1 and n_lines < 10246 and (n_lines - 2) % 3 == 0):
		if("size: [" in line and "build time: [" in line):
			a = line.find('size: [') + len('size: [')
			b = line.find(']', a)
			sub1 = (line[a:b])
			c = line.find('build time: [') + len('build time: [')
			d = line.find(' s', c)
			sub2 = (line[c:d])
			# buf = "%5d    %f\n" % (int(sub1), float(sub2))
			# buf = str(sub1) + "    " + str(sub2) + '\n'
			# wfd.write(buf);

			x_r.append(int(sub1))
			y_r.append(float(sub2))


	fd.close()
	# wfd.flush()
	# wfd.close()

	fig = plt.figure()
	ax1 = fig.add_subplot(111)
	ax1.set_title('index build time (DBLP dataset)')
	plt.xlabel('number of nodes associated with keyword')
	plt.ylabel("build time (second)")
	ss1 = ax1.scatter(x_r, y_r, c = 'r', marker = 'x')
	plt.legend([ss1], ['one keyword'], loc='upper left')
	plt.show()



if __name__ == '__main__':
	func1()
