import os

board  = raw_input("Enter name of PCB ")
filestring =  "PCB\n\t- :download:`{0}.brd<../../../PCB Files/{0}/{0}.brd>`".format(board)
filestring = filestring + ("\n\t- :download:`{0}.sch<../../../PCB Files/{0}/{0}.sch>`".format(board))
pdfs = ["schematic"]
for i in pdfs:
    filestring = filestring + ("\n\t- :download:`{1}.pdf<../../../PCB Files/{0}/{0} {2}.pdf>`".format(board,i[0].upper()+i[1:],i))

os.system("echo '%s' | pbcopy" % filestring)
print "String with PCB files has been copied to the clipboard"
