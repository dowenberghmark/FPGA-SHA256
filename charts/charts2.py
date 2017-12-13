import argparse, subprocess, os, math, sys
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

def head():
	return "\\documentclass{article}\n\\usepackage[utf8]{inputenc}\n\\usepackage{graphicx}\\usepackage{subcaption}\n\\usepackage{rotating}\n\\begin{document}"



def plotjpg2(ss):
    plt.plot(ss[0], ss[1], "r-", label="our SHA")
    plt.plot(ss[0], ss[2], "g-", label="reference SHA")
    plt.legend(loc="upper left")
    plt.savefig("chartsGraph.pdf")



if __name__ == '__main__':
   argument_parser = argparse.ArgumentParser()
   argument_parser.add_argument('--our',
                                 help="Our csv states",
                                 default="output.csv")
   argument_parser.add_argument('--ref',
                                 help="reference csv",
                                 default="reference.csv")
   args = argument_parser.parse_args()
   print args.our
   print args.ref
   lst = [[],[],[]]
   skip = True
   for line in open(os.getcwd()+"/"+args.our):
    if line[-1] == "\n":
      line = line[:-1]
    if line[-1] == "\r":
      line = line[:-1]
      #print line
    l = line.split(";")
    if skip:
      skip = False
    else:
      lst[0].append(int(l[0]))
      lst[1].append(int(l[1]))
  
   skip=True
   for line in open(os.getcwd()+"/"+args.ref):
    if line[-1] == "\n":
      line = line[:-1]
    if line[-1] == "\r":
      line = line[:-1]
      #print line
    l = line.split(";")
    if skip:
      skip = False
    else:
      lst[2].append(int(l[1]))
   print lst
   plotjpg2(lst)

   table = "\\begin{table}\n\\centering\n\\begin{tabular}{"+"l"*3+"}\n \hline\n"
   table += "Filesize & Time our& Time FPGA\\\\ \\hline \n"
   for x in xrange(0,len(lst[0])):
     table += str(lst[0][x]) + "&" +str(lst[1][x]) + "&" +str(lst[2][x]) + "\\\\ \n"
   


     #print slist[x-3]#sGuess
   end = "\\hline\n\\end{tabular}\n\\caption{FIX ME!!!}\n\\label{tbl:oprv}\n\\end{table}\n\\end{document}"
   f2 = open('table1.tex', 'w')
   f2.write(head()+table+end)
   f2.close()

