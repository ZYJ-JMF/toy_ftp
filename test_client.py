from ftplib import FTP

test_client = FTP()
if not test_client.connect('192.241.151.16', 8487).startswith('220'):
    print('You missed response 220')