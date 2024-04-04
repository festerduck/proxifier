import winreg

def get_proxy_settings():
    internet_settings = winreg.OpenKey(winreg.HKEY_CURRENT_USER,
        r'Software\Microsoft\Windows\CurrentVersion\Internet Settings')

    proxy_enable = winreg.QueryValueEx(internet_settings, 'ProxyEnable')[0]
    proxy_server = winreg.QueryValueEx(internet_settings, 'ProxyServer')[0] if proxy_enable else None

    winreg.CloseKey(internet_settings)

    return proxy_enable, proxy_server

proxy_enable, proxy_server = get_proxy_settings()
print('Proxy Enable:', proxy_enable)
print('Proxy Server:', proxy_server)
