#!/usr/bin/env python3

import sys
import struct
import argparse

#Global Vars
magicNumbers = "\xab\xba\xbe\xef"
magicNumbers2 = "\xda\xaa\xaa\xad"
EOT = chr(3)

#move_to_front_encode(), takes string as parameter and returns list
def MtF(string):
	li=[]
	index=1
	stack=[]
	for char in string:
		if char not in li:
			li.append(index)
			li.append(char)
			stack.append(char)
			index=index+1
		elif char in stack:
			li.append(stack.index(char)+1)
			stack.remove(char)
			stack.insert(0,char)
	return li
#run_length_encode(), takes list as parameter and returns list
def RLE(li):
	out=[]
	count = 0
	for ele in li:
		if ele == 1:
			count=count+1
		else:
			if count == 1:
				out.append(1)
				count=0
			if count ==2:
				out.append(1)
				out.append(1)
				count=0
			elif count > 2:
				out.append(0)
				out.append(count)
				count=0
			out.append(ele)
	if count == 1:
		out.append(1)
		count=0
	elif count > 1:
		out.append(0)
		out.append(count)
		count=0
	return out
#turns an element into a ascii formating for ph2
def ascii(ele):
	if isinstance(ele,int):
		ele=chr(ele+128)
	return ele

#to_ascii(), takes list as parameter and converts int in list to acii and chars to binary
def to_ascii(li):
	
	bytes=[ascii(ele) for ele in li]
	return bytes

def readBytesToInt(infileptr, size):
	#read next 4 bytes and convert to int
	s = infileptr.read(size)
	chars = list(s)
	ints = [ord(c) for c in chars]
	bytes = bytearray(ints)
	result =struct.unpack("I",bytes)
	return result[0]

#Move to Front Transformation
def encode(infilename,outfilename):
	#open infile in latin-1 encoding
	infile=open(infilename, encoding="latin-1", mode="r")
	#Read magic numbers, print mesage and return if wrong numbers or no numbers
	if infile.read(4)!=magicNumbers:
		print("Wrong file type for transformation")
		return
	#read 4 bytes to determine block size
	blockSize = readBytesToInt(infile, 4)
	#read rest of infile adn close
	data=infile.read()
	infile.close()
	#open outfile and write magic numbers
	outfile=open(outfilename, encoding="latin-1", mode="w")
	outfile.write(magicNumbers2)
	#write blocksize to file in hex numbers
	bytes = struct.pack("I", blockSize)
	chars = [chr(c) for c in bytes]
	s = "".join(chars)
	outfile.write(s)
	#PREFORM MTF
	mtf_list=MtF(data)
	#PREFORM RL ENCODE
	rl_list=RLE(mtf_list)
	#PREFORM INTO_ASCII
	ascii_list=to_ascii(rl_list)
	#WRITE FINAL PRODUCT TO OUTFILE AND CLOSE
	outfile.write("".join(ascii_list))
	outfile.close()

def bytes_to_ints(ele):
	if ele >= '\x80':
		ele=ord(ele)-128
	return ele

def from_ascii(st):
	li=list(st)
	RLE=[bytes_to_ints(ele) for ele in li]
	return RLE

def i_RLE(li):
	count=0
	i=0
	for ele in li:
		if ele == 0:
			i=li.index(ele)
			count=li[i+1]
			li.pop(i+1)
			li.remove(ele)
			for j in range(count):
				li.insert(i,1)
		count=0
	return li	

def unindexing(i,stack,li):
	#if new to stack
	if len(stack)>=i:	
		c=stack.pop(i-1)
		stack.insert(0,c)
		return c
	#if in stack
	else:
		c=li[(li.index(i)+1)]
		li.remove(c)
		stack.append(c)
		return c
	
def i_MtF(li):
	stack=[]
	og_li=[unindexing(i,stack,li) for i in li]	
	return og_li

def decode(infilename,outfilename):
	#open infile in latin-1 encoding
	infile=open(infilename, encoding="latin-1", mode="r")
	#Read magic numbers, print mesage and return if wrong numbers or no numbers
	if infile.read(4)!=magicNumbers2:
		print("Wrong file type for transformation")
		return
	#read 4 bytes to determine block size
	blockSize = readBytesToInt(infile, 4)
	#read rest of infile and close
	data=infile.read()
	infile.close()
	#open outfile and write magic numbers
	outfile=open(outfilename, encoding="latin-1", mode="w")
	outfile.write(magicNumbers)
	#write blocksize to file in hex numbers
	bytes = struct.pack("I", blockSize)
	chars = [chr(c) for c in bytes]
	s = "".join(chars)
	outfile.write(s)
	#PREFORM i_into_ASCII
	rl_list=from_ascii(data)
	#PREFORM i_run_length
	mtf_list = i_RLE(rl_list)
	#PREFORM i_MtF
	s=i_MtF(mtf_list)
	#WRITE FINAL PRODUCT TO OUTFILE AND CLOSE
	outfile.write("".join(s))
	outfile.close()

def main():
	op =0
	infile =""
	outfile =""
	for i,arg in enumerate(sys.argv[1:]):
		if arg == infile:
			continue
		elif arg == outfile:
			continue
		elif arg == '--infile':
			infile = sys.argv[i+2]
			continue
		elif arg == '--outfile':
			outfile = sys.argv[i+2]
			continue
		elif arg == '--encode':
			op = 1
			continue
		elif arg == '--decode':
			op = 2
	if op == 1:
		encode(infile,outfile,)
	if op == 2:
		decode(infile,outfile)
	

if __name__ == "__main__":
	main()
