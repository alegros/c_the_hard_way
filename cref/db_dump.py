# This script helps me debug cref.c
from struct import *
datafile = open("cref.dat", "rb")
# The file contains 5 integers at the beginning
# 5 integers are 20 bytes long so we read 20 bytes
data = datafile.read(20)
maxrows, ntl_size, args_size, sdesc_size, desc_size = \
unpack('iiiii', data)
print("Maxrows : %s" % maxrows)
print("ntl_size : %s" % ntl_size)
print("args_size : %s" % args_size)
print("sdesc_size : %s" % sdesc_size)
print("desc_size : %s " % desc_size)
record_length = 8 + ntl_size*3 + args_size + sdesc_size + desc_size
# Now let's read the records
for i in range(maxrows):
    data = datafile.read(record_length)
    _id, _set, library, _type, name, args, sdesc, desc\
            = unpack('iipppppp', data)
