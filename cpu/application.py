
import binascii
import sys


"""
translates ascii strings to bit strings in list
Args:
	ascii_array (list): string list
Returns:
	list: password list in bits
"""
def ascii_to_bits(ascii_array,encoding='utf-8', errors='surrogatepass'):

	binary_passwords = []
	for line in string_array:
		bits = bin(int(binascii.hexlify(line.encode(encoding, errors)), 16))[2:]
		binary_passwords.append(bits.zfill(8 * ((len(bits) + 7) // 8)))
	print 'this is binary passwords: ', type(binary_passwords)
	return binary_passwords


"""
append '1' to the end of the string in each element in the list
Args:
	bin_array (list): list of strings in binary format
Returns:
	list: '1' appeneded to each element in the list
"""
def append_one(bin_array):

	for i in range(0,len(bin_array)):
		bin_array[i] = bin_array[i] + '1'
	return bin_array

"""
append k-times '0' to end of string in each element in the list
Args:
	bin_array (list): list of strings in binary format
Returns:
	list: list of elements appended '0' until length of each element equals 448
"""
def append_k(bin_array):
	k = []
	for i in range(0,len(bin_array)):
		k.append(448%512 - len(bin_array[i]))

	for i in range(0,len(bin_array)):
		bin_array[i] = bin_array[i] + (k[i]*'0')
	return bin_array

"""
appends 64-bit format length of the ascii-format string to each element in bin_array list
Args:
	bin_array (list): string list
	ascii_array (list): string list
Returns:
	list: list of elements with 64-bit format of length of ascii string appended to end of each element 
"""
def append_64(bin_array, ascii_array):

	for i in range (0,len(bin_array)):
		bin_array[i] = bin_array[i] + '{0:064b}'.format((8*len(ascii_array[i])))

	return bin_array

"""
prepares array of passwords
Returns:
	list: list of prepared passwords according to SHA-256
"""
def prepare_passwords():
	with open('password.txt', 'rw+') as un_hashed_passwords:
		passwords = un_hashed_passwords.readlines()
	
	bin_array = ascii_to_bits(passwords)
	add_1 = append_one(bin_array)
	print add_1
	add_2 = append_k(add_1)
	append_64(add_2, passwords)


def main():

	prepare_passwords()
if __name__ == "__main__":
	main()

