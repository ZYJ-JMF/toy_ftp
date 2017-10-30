from ftpDebug import FTP

test_client = FTP()
if not test_client.connect('127.0.0.1', 6789).startswith('220'):
    print('You missed response 220')
if not test_client.login().startswith('230'):
      print('You missed response 230')
print("success")