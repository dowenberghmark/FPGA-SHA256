import io
from numpy import genfromtxt
import matplotlib.pyplot as plt
import csv
import pylab
import plotly

def chart_plot():
	data=genfromtxt('output.csv',delimiter=';',skip_header=1,names=['time', 'filesize',])
	plt.xlabel ('Time - s')
	plt.ylabel ('Size - MB')
	plt.title('SHA-256 Results')
	plt.plot(data)
	plt.show()

def plotly():
	print(plotly.__version__)  # version >1.9.4 required
	from plotly.graph_objs import Scatter, Layout
	plotly.offline.plot({
	"data": [
	Scatter(x=[1, 2, 3, 4], y=[4, 1, 3, 7])
	],
	"layout": Layout(
	    title="hello world"
	)
	})
def main():
	plotly()

if __name__== "__main__":
	main()