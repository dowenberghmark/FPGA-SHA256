import io
from numpy import genfromtxt
import matplotlib.pyplot as plt
import csv

def chart_plot():
	data=genfromtxt('output.csv',delimiter=',')
	plt.xlabel ('Time / s')
	plt.ylabel ('Size / GB')
	plt.title('SHA-256 Results')
	plt.plot(data)
	plt.show()

def main():
	chart_plot()

if __name__== "__main__":
	main()