#!/usr/bin/python3.8

import random, string
import time
import pythonDatabase

def randomword(length):
   letters = string.ascii_lowercase
   return ''.join(random.choice(letters) for i in range(length))


pythonDatabase.Connect()


for i in range(0,100) :
    value = randomword(10)
    pythonDatabase.Put(value, value)
    time.sleep(0.01)


pythonDatabase.List()


pythonDatabase.Disconnect()
