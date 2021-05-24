#script to generate various distributions with a number of events for comparison

import argparse
import numpy as np
import time

#parser
parser = argparse.ArgumentParser(description="Make a sample distribution")
parser.add_argument('distro', 
                    choices=['gaussian', 'flat', 'poisson', 'slow_gaussian'],
                    help='type of distribution to generate')
parser.add_argument('events', 
                    type=int,
                    help='number of events to generate')
args = parser.parse_args()

distro = args.distro
events = args.events

#generate various distributions within bounds 0-12
if distro == 'slow_gaussian':
    data = np.random.normal(5, 2 ,events)
    time.sleep(300)
if distro == 'gaussian':
    data = np.random.normal(5, 2 ,events)
if distro == 'poisson':
    data = np.random.poisson(5, events)
if distro == 'flat':
    data = np.random.uniform(0,12, events)

#save these to a local file
np.savetxt(f"data_{distro}_{events}.txt",data)


