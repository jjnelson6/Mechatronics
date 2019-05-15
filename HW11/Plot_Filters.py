from pylab import *


Filename = "data_6.txt"

f= open (Filename, 'r')
Data= f.readlines()

Rawdata = []
MAF= []
IIR = []
FIR= []
index=[]


for n in range (len(Data)):
	Spdata= Data[n].split("     ")
	FIR_data= Spdata[4].split('\n')
	index.append(int(Spdata[0]))	
	Rawdata.append(float(Spdata[1]))
	MAF.append(float(Spdata[2]))
	IIR.append(float(Spdata[3]))
	FIR.append(float(FIR_data[0]))
	

figure()
title('DSP: '+Filename)
plot(index, Rawdata, 'ro-', label='Rawdata',)
plot(index, MAF, 'g>-',label='MAF' )
plot(index, IIR, 'bx-',label= 'IIR')
plot(index, FIR, 'k<-',label= 'FIR')

ylabel('Zaccl')
xlabel('index')
legend()


show()


