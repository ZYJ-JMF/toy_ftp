from ftpDebug import FTP
import subprocess
import random
import time
import filecmp
import struct
import os
import shutil
import string
ftp = FTP()

def create_test_file(filename):
    f = open(filename, 'wb')
    for i in range(10000):
        data = struct.pack('d', random.random())
        f.write(data)
    f.close()

if not ftp.connect('127.0.0.1', 6789).startswith('220'):
    print('You missed response 220')
if not ftp.login().startswith('230'):
    print('You missed response 230')
if ftp.sendcmd('SYST') != '215 UNIX Type: L8.':
    print('Bad response for SYST')
if ftp.sendcmd('TYPE I') != '200 Type set to I.':
    print('Bad response for TYPE I')

port=6789
directory='/Users/pingguo/Desktop/CN1_ftp/server/tmp'
filename = 'test%d.data' % random.randint(100, 200)
create_test_file(directory + '/' + filename)

ftp.set_pasv(False)

if not ftp.retrbinary('RETR %s' % filename, open(filename, 'wb').write).startswith('226'):
    print('Bad response for RETR')
print("success")