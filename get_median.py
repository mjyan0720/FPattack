#!/usr/bin/python

import numpy as np
import sys


if __name__ == "__main__":

    input_file = sys.argv[1]
    median_vals = []
    with open(input_file) as f:
        for line in f:
            items = line.strip()[:-1].split(',')
            median_vals.append(np.median([int(x) for x in items]))

    print(median_vals)
    print(np.median(median_vals))
