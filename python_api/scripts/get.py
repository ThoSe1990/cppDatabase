#!/usr/bin/python3.8

import sys

import pythonDatabase


pythonDatabase.Connect()

cmdLineArguments = sys.argv
print ("requesting value: " , cmdLineArguments[1])

value = pythonDatabase.Get(cmdLineArguments[1])

print("received value: " , value)

pythonDatabase.Disconnect()
