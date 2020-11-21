import sys
import random

num_dec_mb = int(sys.argv[1])
f = open("/dev/urandom", "rb")
data = b""

for i in range(num_dec_mb):
    data+=f.read(10000000)
    print("Read %d MB" % (i*10))