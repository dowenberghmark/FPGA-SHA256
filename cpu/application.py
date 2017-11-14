
import binascii
import sys



#translates ascii strings to binary strings
#input: textfile
#return: list of passwords in binary string
def ascii_to_bits(text,encoding='utf-8', errors='surrogatepass'):
	binary_passwords = []
	with open(text,'rw+') as un_hashed_passwords:
		passwords = un_hashed_passwords.readlines()

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
		byte_size = bytearray(params[i])
		size_of_byte_size = sys.getsizeof(byte_size)
		k.append(448%512 - size_of_byte_size)

	for i in range(0,len(params)):
		params[i] = params[i] + (k[i]*'0')

	return params
def prepare_passwords():
	
	bin_array = ascii_to_bits('password.txt')
	add_1 = append_one(bin_array)
	print add_1
	append_k(add_1)
def main():


	prepare_passwords()
if __name__ == "__main__":
    main()

