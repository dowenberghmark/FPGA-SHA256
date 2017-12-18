import argparse, subprocess, os, math, sys
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

def head():
	return "\\documentclass{article}\n\\usepackage[utf8]{inputenc}\n\\usepackage{graphicx}\\usepackage{subcaption}\n\\usepackage{rotating}\n\\begin{document}"

def tens(s):
  lst =  []
  for l in s:
   # print l
    if l >= 1:
      lst.append(len(str(l).split('.')[0])-1)
    else:
      aa = str(l)
      aa = aa[2:]
      n = 0
      for char in aa:
        n -=1
        if char != '0':
          lst.append(n)
          break
  return lst

def plotjpg2(ss):
    color = ['b', 'g', 'r', 'c', 'm', 'y', 'k', 'w']
    #print ss[0][1:]
    ss[0] = tens(ss[0][1:])
    print ss[1]
    for x in xrange(1,len(ss)):
       
       plt.plot(ss[x][1:], ss[0], color[x]+"-", label=ss[x][0][:-4])
    plt.legend(loc="lower right")
    plt.ylabel('Amount of hashed data in tenfold (MB)')
    plt.xlabel('Number of hashes per s')
    plt.title('A comparison between hashing on FPGA and CPU')
    plt.grid(True)
    plt.savefig("chartsGraph.pdf")



if __name__ == '__main__':
  argument_parser = argparse.ArgumentParser()
  argument_parser.add_argument('--folder',
                                 help="folder with csv files",
                                 default="csv")
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
      nn += 1
      if line[-1] == "\n":
        line = line[:-1]
      if line[-1] == "\r":
        line = line[:-1]
     # print line
      l = line.split(";")
      if first and mesures == args.mesure-1:
        #print "here " + l[0]
        if l[0] == '0':
          skip = True
        else:
       #   print "nn: " +str(nn)
        #  print "add len " + l[0]
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
  plotjpg2(lst)

  table = "\\begin{table}\n\\centering\n\\begin{tabular}{"+"l"*3+"}\n \hline\n"
  table += "Filesize & Time our& Time FPGA\\\\ \\hline \n"
  # for x in xrange(0,len(lst[0])):
   #  table += str(lst[0][x]) + "&" +str(lst[1][x]) + "&" +str(lst[2][x]) + "\\\\ \n"
   


     #print slist[x-3]#sGuess
   #end = "\\hline\n\\end{tabular}\n\\caption{FIX ME!!!}\n\\label{tbl:oprv}\n\\end{table}\n\\end{document}"
   #f2 = open('table1.tex', 'w')
   #f2.write(head()+table+end)
   #f2.close()

