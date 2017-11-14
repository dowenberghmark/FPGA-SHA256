
import binascii
import sys



#translates ascii strings to binary strings
#input: textfile
#return: list of passwords in binary string
def ascii_to_bits(passwords,encoding='utf-8', errors='surrogatepass'):

	binary_passwords = []
	for line in passwords:
		bits = bin(int(binascii.hexlify(line.encode(encoding, errors)), 16))[2:]
		binary_passwords.append(bits.zfill(8 * ((len(bits) + 7) // 8)))

	print 'this is binary passwords: ', type(binary_passwords)
	return binary_passwords


def append_one(params):

	for i in range(0,len(params)):
		params[i] = params[i] + '1'
	return params

def append_k(params):
	k = []
	for i in range(0,len(params)):
		k.append(448%512 - len(params[i]))

	for i in range(0,len(params)):
		params[i] = params[i] + (k[i]*'0')


	return params

def append_64(params, passwords):

	for i in range (0,len(params)):
		params[i] = params[i]+ '{0:064b}'.format((8*len(passwords[i])))

	return params

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

