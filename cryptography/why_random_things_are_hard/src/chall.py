from Crypto.Util.number import *
from random import *
import json

class RNG1:
    def __init__(self, mod, seed):
        self.a = randint(2, mod)
        self.b = randint(2, mod)
        self.m = mod
        self.state = seed
        self.state = (self.rand() * self.rand()) % self.m

    def rand(self):
        self.state = (self.a * self.state + self.b) % self.m
        return self.state >> 99

    def encrypt(self,msg):
        return long_to_bytes(self.rand() ^ int(msg, 16)).hex()

class RNG2:
    def __init__(self, m, n):
        self.m = m
        self.n = n

    def rand(self, a, b):
        return self.mult((((self.m - a) * (self.m - a)) + ((self.n - b) * (self.n - b))), 2024) * (((self.m - a)*(self.m - a)) + ((self.n - b) * (self.n - b)))
    
    def encrypt(self, a, b):
        return self.rand(a, b)

    def mult(self, x, a):
        return ~(x * a) & ((1 << (x * a).bit_length()) - 1)

def encrypt(pt, x, y, mode):
    if mode == 1:
        ct = []
        rand2 = RNG2(bytes_to_long(pt[:len(pt)//2]), bytes_to_long(pt[len(pt)//2:]))
        res = [rand2.encrypt(0, x)]
        res.append(rand2.encrypt(y, 0))
        res.append(rand2.encrypt(y, x))
        for i in res:
            ct.append(rand1.encrypt(hex(i)[2:]))
    elif mode == 2:
        ct_val = bytes.fromhex(rand1.encrypt(pt))
        rand2 = RNG2(bytes_to_long(ct_val[:len(ct_val)//2]), bytes_to_long(ct_val[len(ct_val)//2:]))
        ct = [rand2.encrypt(0, x)]
        ct.append(rand2.encrypt(y, 0))
        ct.append(rand2.encrypt(y, x))
    else:
        return 0 
    return ct

flag = b"ARA5{uhh_seems_like_it_is_not_random_number_anymore_b431d0abe4}"

x = 2**60
m = getPrime(512)
seed = randint(1, m)
rand1 = RNG1(m, seed)
data = { "m": rand1.m , "a": rand1.a, "b": rand1.b}

for _ in range(3):
    try:
        inp = json.loads(input("Input : "))
        if("option" in inp):
            if(inp["option"] == "help_me"):
                print(json.dumps(data))
            
            elif(inp["option"] == "flag"):
                y = randint(0, x)
                ct = encrypt(flag, x, y, 1)
                result = {}
                result["ct_flag"] = ct
                print(json.dumps(result))

            elif(inp["option"] == "encrypt" and "plaintext" in inp):
                pt = inp["plaintext"].encode().hex()
                if(len(pt) >= 20):
                    ct_msg = []
                    for i in pt:
                        y = randint(0, x)
                        ct_msg.append(encrypt(i, x, y, 2))
                    shuffle(ct_msg)
                    shuffle(ct_msg)
                    result = {}
                    result["ct_msg"] = ct_msg
                    print(json.dumps(result))
                else:
                    print(json.dumps({"error":"Something wrong"}))
            else:
                print(json.dumps({"error":"Something wrong"}))
    except Exception as e:
        print(json.dumps({"error":"Something wrong"}))
        exit()