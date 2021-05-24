import numpy as np
import matplotlib.pyplot as plt
import glob

# Defining a simple histogram function for the task
def plot_pdf(data, *args,**kwargs):
    counts, bins, _ = plt.hist(data, bins =20,
                                histtype='step', density=1,**kwargs)
    plt.xlabel('density')
    plt.xlim(0,12)

plt.figure()

with open('__GangaInputData.txt__') as f:
    lines = [line.rstrip() for line in f]

for i in lines:
    data = np.loadtxt(i[7:]) #stripping off the 'file:// for the path'
    plot_pdf(data)

#save the resulting histogram
plt.savefig("output.png")

