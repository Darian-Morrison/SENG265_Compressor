#!/usr/bin/env python3

import sys
import struct

magicNumbers = "\xab\xba\xbe\xef"
EOT = chr(3)




#right shift takes st as parameter and preforms a right shift to it
def left_shift(st):
	li=list(st)
	temp=st[0]
	for i in range(len(st)):
		if i==(len(st)-1):
			li[i]=temp
		else:
			li[i]=li[i+1]
	return "".join(li)

#function that returns ph1 string
def transformForward(block):
	#build list of lists
	li=[]
	for i in range(len(block)):
		if i==0:
			li.append(block)
		else:
			li.append(left_shift(li[i-1]))
	li.sort()
	return "".join([i[len(i)-1] for i in li])

def transformBackward(block):
	#make initial array with block characters in a list
	array=list(block)
	while(len(array[1])<len(array)):
		#copy into temp array
		temp = array[:]
		#sort temp
		temp.sort()
		#store sorted val in array
		for i in range(len(array)):
			array[i] = array[i] + temp[i][len(temp[i])-1]
	for st in array:
		if st.endswith(EOT):
			return st[:-1]
	
		
			
def forward(infile,outfile,size):
	#write magic nums to file
	outfileptr = open(outfile, encoding="latin-1", mode="w")
	outfileptr.write(magicNumbers)
	#convert blocksize of type int to writable chr and write to file
	bytes = struct.pack("I", size)
	chars = [chr(c) for c in bytes]
	s = "".join(chars)	
	outfileptr.write(s)
	#read infile block size by blocksize
	infileptr = open(infile, "r")
	while True:
		block =infileptr.read(size)
		if not block:
			break
		block = block +EOT
		outfileptr.write(transformForward(block))
	#close in and out file 
	outfileptr.close()
	infileptr.close()


def backward(infile,outfile):
	#read magic nums from file, error message if can't be read or incorrect
	infileptr = open(infile, encoding="latin-1", mode= "r")
	if infileptr.read(4)!=magicNumbers:
		print("Wrong file type for transformation")
		return
	#read next 4 bytes and convert to int
	s = infileptr.read(4)
	chars = list(s)
	ints = [ord(c) for c in chars]
	bytes = bytearray(ints)
	result =struct.unpack("I",bytes)
	size=result[0]
	outfileptr = open(outfile, encoding="latin-1", mode= "w")
	while True:
		block =infileptr.read(size+1)
		if not block:
			break
		outfileptr.write(transformBackward(block))
	

	infileptr.close()
	
def main():
	#Declarations, op decides wether forwards or backwards is executed
	op =0
	infile =""
	outfile =""
	blocksize =0
	for i,arg in enumerate(sys.argv):
		if arg == '--infile':
			infile = sys.argv[i+1]
			continue
		elif arg == '--outfile':
			outfile = sys.argv[i+1]
			continue
		elif arg == '--blocksize':
			blocksize = sys.argv[i+1]
			continue
		elif arg == '--forward':
			op = 1
		elif arg == '--backward':
			op = 2
	if op == 1:
		forward(infile,outfile,int(blocksize))
	if op == 2:
		backward(infile,outfile)
		

if __name__ == "__main__":
	main()
