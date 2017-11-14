import sha2

if __name__ == '__main__':
	print sha2.sha256('Can you keep a secret?').hexdigest()
