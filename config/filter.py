# -*- coding: utf-8 -*-
"""
Created on Mon May 16 09:46:59 2016

@author: leendert
"""

import numpy as np
import sys
import argparse

def printProgress (iteration, total, prefix = '', suffix = '', decimals = 1, barLength = 100):
    """
    Call in a loop to create terminal progress bar
    @params:
        iteration   - Required  : current iteration (Int)
        total       - Required  : total iterations (Int)
        prefix      - Optional  : prefix string (Str)
        suffix      - Optional  : suffix string (Str)
    """
    filledLength    = int(round(barLength * iteration / float(total)))
    percents        = round(100.00 * (iteration / float(total)), decimals)
    bar             = '#' * filledLength + '-' * (barLength - filledLength)
    sys.stdout.write('%s [%s] %s%s %s\r' % (prefix, bar, percents, '%', suffix)),
    sys.stdout.flush()
    if iteration == total:
        print("\n")

def analyseFile(args):
    print('Loading file %s' % args.input)
    inputData = np.genfromtxt(args.input, dtype=None)
    
    names = np.array([i[2] for i in inputData])
    data = np.array([list(i)[:2] + list(i)[3:] for i in inputData])
    data[np.isnan(data)]=0.
    
    maxEvent = int(data[-1,0])+1
    
    if args.verbose:
        print('Nr of events: %d' % maxEvent)
        print('Registered particles:')
        print(set(names))
    printProgress(0, maxEvent, prefix = 'Progress:', suffix = 'Complete', barLength = 50)
    with open(args.output, 'a') as f:
        lastIndex = 0
        for i in range(maxEvent):
            dt = data[lastIndex:lastIndex+30]
            ns = names[lastIndex:lastIndex+30]
            lastIndex += np.where(dt[:,0] == i)[0][0]
            d = dt[dt[:,0] == i]
            n = ns[dt[:,0] == i]
            if args.number:
                f.write('%d\t%d\n' % (i, len(d)))
            
            if args.divide:
                subEventIndex = 0
                tI = 0
                for time in sorted(set(d[:, 1])):
                    subEvent = d[d[:,1] == time]
                    f.write('%d\t%d\t%d\n' % (i, subEventIndex, len(subEvent)))
                    for e in subEvent:
                        f.write('%d\t%f\t%s\t%f\t%f\t%f\t%f\t%f\t%f\n' % (e[0],\
                        e[1], n[tI], e[2], e[3], e[4],\
                        e[5], e[6], e[7]))
                        tI+=1
                    subEventIndex+=1
            if (100.*i/maxEvent)%0.1 < 0.01 or i == maxEvent-1:
                printProgress(i+1, maxEvent, prefix = 'Progress:', suffix = 'Complete', barLength = 50)
            

if __name__ == '__main__':
    
    print('Starting...')
    parser = argparse.ArgumentParser(prog='Filter', description='Process the output of CRADLE++ into a desired output format.')
    parser.add_argument('input', help='Input file for conversion, output file from CRADLE++.')
    parser.add_argument('-o', '--output', help='Output file name', default='output.txt')
    parser.add_argument('-N', '--number', help='Check to precede each event with its size', action='store_true')
    parser.add_argument('-d', '--divide', help='Divide event into subevents with same timing', action='store_true')
    parser.add_argument('-p', '--priority', help='Make a particle a priority and group events based on that, overriding divide')
    parser.add_argument('-v', '--verbose', action='store_true', help='Turn on verbosity')
    parser.add_argument('--version', action='version', version='%(prog)s 0.1')
    
    args = parser.parse_args()
    
    analyseFile(args)
    