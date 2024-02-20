import winreg
import pyfiglet
import os


def clr_scr():
    os.system('cls' if os.name == 'nt' else 'clear')
def art_c():
    clr_scr()
    text = "PIEAS PROXY"
    art = pyfiglet.figlet_format(text)
    print(art)
    text = "festerduck"
    art = pyfiglet.figlet_format(text, font='digital')
    print(art)

def get_proxy_settings():
    internet_settings = winreg.OpenKey(winreg.HKEY_CURRENT_USER,
        r'Software\Microsoft\Windows\CurrentVersion\Internet Settings')

    proxy_enable = winreg.QueryValueEx(internet_settings, 'ProxyEnable')[0]
    proxy_server = winreg.QueryValueEx(internet_settings, 'ProxyServer')[0] if proxy_enable else None

    winreg.CloseKey(internet_settings)

    return proxy_enable, proxy_server

proxy_enable, proxy_server = get_proxy_settings()

def set_proxy(enable, proxy_server):
    internet_settings = winreg.OpenKey(winreg.HKEY_CURRENT_USER,
        r'Software\Microsoft\Windows\CurrentVersion\Internet Settings',
        0, winreg.KEY_ALL_ACCESS)

    # enable or disable the proxy
    winreg.SetValueEx(internet_settings, 'ProxyEnable', 0, winreg.REG_DWORD, 1 if enable else 0)

    # set the proxy server
    if enable:
        winreg.SetValueEx(internet_settings, 'ProxyServer', 0, winreg.REG_SZ, proxy_server)

    # close the key when you're done
    winreg.CloseKey(internet_settings)

# enable the proxy
#set_proxy(True, 'http://172.30.10.10:3128')

# disable the proxy
#set_proxy(False, '')

art_c()

if proxy_enable:
    print("Current proxy is\n\n\t[", proxy_server, "]\n")
else:
    print("\nNo proxy is set..\n")

#print("Want to set the proxy? [Default: 172.30.10.10:3128]?")
opt = input("[1] Enable Proxy\t[2] Disable Proxy\n")

if opt == '1':
    set_proxy(True, 'http://172.30.10.10:3128')
elif opt == '2':
    set_proxy(False, '')
else:
    print("Invalid input!")

proxy_enable, proxy_server = get_proxy_settings()
if proxy_enable:
    print("Proxy has been enabled. [", proxy_server, "]")
elif proxy_enable == 0:
    print("Proxy has been disabled")
else:
    print("error")
print("Press any key to exit...")
input()
