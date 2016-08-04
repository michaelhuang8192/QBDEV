import hashlib


#QBPOS_CLSID = '{0361371E-C40F-4026-8ACD-3F84D9A577E3}'
QBPOS_CLSID = '{33BDA738-B015-406D-8C39-A81345997943}'


def qbpos_hash_passwd(passwd):
    h = hashlib.sha1()
    h.update( QBPOS_CLSID + passwd )
    s = h.hexdigest().upper()
    r = ''
    for i in range(0, 21, 2):
        r += s[i]
    
    return r + s[21:]


def qb_hash_passwd(username, passwd):
    h = hashlib.sha1()
    h.update( username + passwd )
    s = h.hexdigest().lower()
    return '06' + s[:-2] + '00'

if __name__ == '__main__':
    import sys
    
    if len(sys.argv) == 2:
        print qbpos_hash_passwd(sys.argv[1])
    elif len(sys.argv) == 3:
        print qb_hash_passwd(sys.argv[1], sys.argv[2])
        
        