import argparse, subprocess, os, math, sys
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt



def plotjpg2(ss, out, header):
    color = ['b', 'g', 'r', 'c', 'm', 'y', 'k', 'w']
    for x in xrange(1,len(ss)):
      for c in xrange(1,len(ss[x])):
       ss[x][c] = ss[x][c]/ss[0][c]
      print ss[x]
      print ss[0]
      plt.plot(ss[0][1:], ss[x][1:], color[x]+"-", label=ss[x][0][:-4])
    plt.gca().set_xscale('log')
    plt.legend(loc="upper right")
    plt.xticks(ss[0][1:])
    plt.xlabel('Amount of hashed data (MB)')
    plt.ylabel('Throughput (MB/s)')
    plt.title(header)
    plt.grid(True)
    plt.savefig(out+".pdf")



if __name__ == '__main__':
  argument_parser = argparse.ArgumentParser()
  argument_parser.add_argument('--folder',
                                 help="folder with csv files",
                                 default="csv")
  argument_parser.add_argument('--header',
                                 help="The header of the graph",
                                 default="A comparison between hashing on FPGA and CPU")
  argument_parser.add_argument('--out',
                                 help="The filename of the graph, .pdf will be added",
                                 default="chartsGraph")
  argument_parser.add_argument('--mesure',
                                 help="mesure per size",
                                 default=10,
                                 type=int)

  args = argument_parser.parse_args()
  print args.folder
  lst = [["Datas"]]
  for (dirpath, dirnames, filenames) in os.walk(os.getcwd()+"/"+args.folder):
    for filename in filenames:
      if ".csv" in filename:
        print filename
        lst.append([filename])
  first = True
  skip = False
  for x in xrange(1,len(lst)):
    mesures = args.mesure-1
    sum = 0
    nn = 0
    for line in open(os.getcwd()+"/"+args.folder +"/"+ lst[x][0]):
      #print line
      nn += 1
      if line[-1] == "\n":
        line = line[:-1]
      if line[-1] == "\r":
        line = line[:-1]
      #print line
      l = line.split(";")
      if first and mesures == args.mesure-1:
        #print "here " + l[0]
        if l[0] == '0':
          skip = True
        else:
       #   print "nn: " +str(nn)
         # print "add len " + l[0]
          lst[0].append(float(l[0]))
          sum += float(l[1])
      elif mesures == 0:
        lst[x].append(sum/args.mesure)
    #    print lst[0]
     #   print lst[x]
      #  print "nn: " +str(nn)
        sum =0
        mesures = args.mesure
      else:
       # print "sum: " + str(sum)

        sum += float(l[1])
      mesures -= 1
    first = False

  #print lst
 # for l in lst:
   # print len(l)
  plotjpg2(lst, args.out, args.header)


