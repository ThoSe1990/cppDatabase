#!/usr/bin/python3.8

import sys

import pythonDatabase


pythonDatabase.Connect()

cmdLineArguments = sys.argv
print ("adding key ", cmdLineArguments[1],  "  with value: " , cmdLineArguments[2])

pythonDatabase.Put(cmdLineArguments[1], cmdLineArguments[2])


pythonDatabase.Disconnect()
