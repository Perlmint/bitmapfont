import _winreg

target_version = {
    "major": 7,
    "minor": 0,
    "patch": 0,
    'meta': ''
}

def parseVersion(version):
    elements = version.split(".", 2)
    patch = elements[2].split("-", 1)
    if len(patch) == 1:
        patch.append('')

    return {
        "major": int(elements[0]),
        "minor": int(elements[1]),
        "patch": int(patch[0]),
        "meta": patch[1]
    }

def formatVersion(version):
    ret = "{0}.{1}.{2}".format(version['major'], version['minor'], version['patch'])
    if len(version['meta']) > 0:
        ret = ret + "-" + version['meta']

    return ret

def get_regvalue(regkey, regvalue):
    value, type = _winreg.QueryValueEx(regkey, regvalue)

    return value

def open_key(parent, key):
    return _winreg.OpenKey(
        parent,
        key,
        0,
        _winreg.KEY_READ | _winreg.KEY_WOW64_64KEY
    )

def enum_all_keys(key):
    idx = 0
    ret = []
    
    try :
        while True:
            sub_key = _winreg.EnumKey(key, idx)
            idx = idx + 1
            ret.append(sub_key)
    except:
        return ret

found = False

imagemagick_root_key = open_key(_winreg.HKEY_LOCAL_MACHINE, 'SOFTWARE\\ImageMagick')
versions = filter(lambda v: v != 'Current', enum_all_keys(imagemagick_root_key))

for version in versions:
    parsed = parseVersion(version)
    if parsed['major'] != target_version['major']:
        continue
    if parsed['minor'] < target_version['minor']:
        continue
    if parsed['patch'] < target_version['patch']:
        continue
    target_version['minor'] = parsed['minor']
    target_version['patch'] = parsed['patch']
    target_version['meta'] = parsed['meta']
    found = True

if found:
    try:
        key = open_key(imagemagick_root_key, formatVersion(target_version) + "\\Q:16")
    except:
        try:
            key = open_key(imagemagick_root_key, formatVersion(target_version) + "\\Q:8")
        except Error as e:
            raise e
    print get_regvalue(key, "LibPath")
else:
    raise Exception("Available version is not found")
