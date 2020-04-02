import numpy as np 

f = open("ascii_file.txt", "r")

data_in = f.read().splitlines()
f.close()
array_len = len(data_in)/2
dims = int(np.sqrt(array_len))

a, b = data_in[:array_len], data_in[array_len:]

# print(a)
# print(b)

def convert_i(arr):
	outp =[]
	for i in range(dims):
		row = []
		for j in range(dims):
			row.append(int(arr[(i*dims)+j]))
		outp.append(row)
	return np.array(outp)

def convert_o(arr):
	str_o = ""
	for i in arr:
		for j in i:
			str_o += str(j) + "\n"
	return str_o

fin = np.dot(convert_i(a), convert_i(b))

print(fin)
print(convert_o(fin))

o = open("solution.txt", 'w')
o.write(convert_o(fin))

