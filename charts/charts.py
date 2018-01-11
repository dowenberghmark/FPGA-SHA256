import argparse
import os
import math
from matplotlib import pyplot as plt
import matplotlib


def plot(data, filename, title):
    color = ['bD', 'go', 'rh', 'cx', 'mp', 'y<', 'k', 'w']
    for x in xrange(1, len(data)):
        for c in xrange(1, len(data[x])):
            data[x][c] = data[0][c]/data[x][c]
        plt.plot(data[0][1:], data[x][1:], color[x-1]+"-", label=data[x][0][:-4])
    plt.gca().set_xscale('log')
    plt.legend(loc="upper left")
    plt.xlabel('Amount of hashed data (MB)')
    plt.ylabel('Throughput (MB/s)')
    plt.title(title)
    plt.grid(True)
    plt.savefig(filename+".pdf")


if __name__ == '__main__':
    argument_parser = argparse.ArgumentParser()
    argument_parser.add_argument('--folder',
                                 help="folder with csv files",
                                 default="")
    argument_parser.add_argument('--title',
                                 help="The title of the graph",
                                 default="A comparison between hashing on FPGA and CPU")
    argument_parser.add_argument('--filename',
                                 help="The filename of the graph, .pdf will be added",
                                 default="chartsGraph")
    argument_parser.add_argument('--MPP',
                                 help="measurements per point",
                                 default=10,
                                 type=int)

    args = argument_parser.parse_args()
    if len(args.folder) == 0:
        print "No input folder set, use --folder "
        exit()
    data = [["Data sizes"]]
    for (dirpath, dirnames, filenames) in os.walk(os.getcwd()+"/"+args.folder):
        for filename in filenames:
            if ".csv" in filename:
                data.append([filename])
    first = True
    for x in xrange(1, len(data)):
        measuresCount = args.MPP-1
        sum = 0
        for line in open(os.getcwd() + "/" + args.folder + "/" + data[x][0]):
            if line[-1] == "\n":
                line = line[:-1]
            if line[-1] == "\r":
                line = line[:-1]
            linesplit = line.split(";")
            if first and measuresCount == args.MPP-1:
                if linesplit[0] == '0':
                    skip = True
                else:
                    data[0].append(float(linesplit[0]))
                sum += float(linesplit[1])
            elif measuresCount == 0:
                data[x].append(sum/args.MPP)
                sum = 0
                measuresCount = args.MPP
            else:
                sum += float(linesplit[1])
            measuresCount -= 1
        first = False
    plot(data, args.filename, args.title)
