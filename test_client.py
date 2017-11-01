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

credit = 40
minor = 3
major = 8

def create_test_file(filename):
    f = open(filename, 'wb')
    for i in range(10000):
        data = struct.pack('d', random.random())
        f.write(data)
    f.close()
def test(port=6789, directory='/Users/pingguo/Desktop/CN1_ftp/server/tmp'):
    ftp = FTP()
    # connect
    if not ftp.connect('127.0.0.1', port).startswith('220'):
      print('You missed response 220')
      credit -= minor
    # login
    if not ftp.login().startswith('230'):
      print('You missed response 230')
      credit -= minor
    # SYST
    if ftp.sendcmd('SYST') != '215 UNIX Type: L8':
      print('Bad response for SYST')
      credit -= minor
    # TYPE
    if ftp.sendcmd('TYPE I') != '200 Type set to I.':
      print('Bad response for TYPE I')
      credit -= minor
    # PORT download
    filename = 'test%d.data' % random.randint(100, 200)
    create_test_file(directory + '/' + filename)
    ftp.set_pasv(False)
    if not ftp.retrbinary('RETR %s' % filename, open(filename, 'wb').write).startswith('226'):
      print('Bad response for RETR')
      credit -= minor
    if not filecmp.cmp(filename, directory + '/' + filename):
      print('Something wrong with RETR')
      credit -= major
    os.remove(directory + '/' + filename)
    os.remove(filename)
    # PASV upload
    ftp2 = FTP()
    ftp2.connect('127.0.0.1', port)
    ftp2.login()
    filename = 'test%d.data' % random.randint(100, 200)
    create_test_file(filename)
    if not ftp2.storbinary('STOR %s' % filename, open(filename, 'rb')).startswith('226'):
      print('Bad response for STOR')
      credit -= minor
    if not filecmp.cmp(filename, directory + '/' + filename):
      print('Something wrong with STOR')
      credit -= major
    os.remove(directory + '/' + filename)
    os.remove(filename)
    # QUIT
    if not ftp.quit().startswith('221'):
      print('Bad response for QUIT')
      credit -= minor
    ftp2.quit()
test()
# Test 2

directory = ''.join(random.choice(string.ascii_letters) for x in range(10))
if os.path.isdir(directory):
  shutil.rmtree(directory)
os.mkdir(directory)
test(6789, directory)
shutil.rmtree(directory)
print("success")