#!/usr/bin/env python
#*- coding:utf-8 -*-
# File iot_demo.py

import urllib
import urllib2
import json
import time
import re

import sys
from collections import deque
import datetime
import getopt
import os
import subprocess
import requests
from requests.auth import HTTPBasicAuth
import json

global hum
tem = "N/A"  #device 1
#------------------------------------------------------------

#------------------------------------------------------------
#Use your own deviceID and apikey
deviceID="xxxxxxxxxx"
apikey = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"

#------------------------------------------------------------

#------------------------------------------------------------
#header packet information
#------------------------------------------------------------

HEADER_SIZE=4
PKT_TYPE_DATA=0x10
PKT_TYPE_ACK=0x20

PKT_FLAG_ACK_REQ=0x08
PKT_FLAG_DATA_ENCRYPTED=0x04
PKT_FLAG_DATA_WAPPKEY=0x02
PKT_FLAG_DATA_ISBINARY=0x01

#------------------------------------------------------------
#last pkt information
#------------------------------------------------------------
dst=0
ptype=0
ptypestr="N/A"
src=0
seq=0
datalen=0
SNR=0
RSSI=0
bw=0
cr=0
sf=0

#------------------------------------------------------------
#with fiware support?
#------------------------------------------------------------
_fiware=0
#------------------------------------------------------------

#------------------------------------------------------------
#log gateway message?
#------------------------------------------------------------
_logGateway=0
#gateway id/addr, just for giving a different name to various log files
_gwaddr=1
#------------------------------------------------------------

#------------------------------------------------------------
#raw output from gateway?
#------------------------------------------------------------
_rawFormat=0
#------------------------------------------------------------

#------------------------------------------------------------
#check for app key?
#------------------------------------------------------------
_wappkey=0
#------------------------------------------------------------
the_app_key = [5,6,7,8]

#valid app key? by default we do not check for the app key
_validappkey=1
#------------------------------------------------------------
#add here app keys that you want to allow for your gateway
app_key_list = [
	#change here your application key
	[1,2,3,4],
	[5,6,7,8]
]
#------------------------------------------------------------

#------------------------------------------------------------
#for handling images
#------------------------------------------------------------
#list of active nodes
nodeL = deque([])
#association to get the file handler
fileH = {}
#global image seq number
rcvImg=0
#------------------------------------------------------------

def setup():
	time.sleep(0.1)

def http_post(data):
	try:
		url = 'http://www.linksprite.io/api/http'
		jdata = json.dumps(data)
		req = urllib2.Request(url, jdata)
		req.add_header('Content-Type','application/json')
		response = urllib2.urlopen(req)
		return response.read()
	except urllib2.URLError:
		print "connect failed"
		return "connect failed"
		pass


def main():
	while True:
		sys.stdout.flush()
		ch=sys.stdin.read(1)
		if (ch=='^'):
			now = datetime.datetime.now()
			ch=sys.stdin.read(1)
			if (ch=='p'):
				data = sys.stdin.readline()
				print now.isoformat()
				arr = map(int,data.split(','))
				print "splitted in: ",
				print arr
				dst=arr[0]
				ptype=arr[1]
				ptypestr="N/A"
				if ((ptype & 0xF0)==PKT_TYPE_DATA):
					ptypestr="DATA"
					if (ptype & PKT_FLAG_DATA_ISBINARY)==PKT_FLAG_DATA_ISBINARY:
						ptypestr = ptypestr + " IS_BINARY"
					if (ptype & PKT_FLAG_DATA_WAPPKEY)==PKT_FLAG_DATA_WAPPKEY:
						ptypestr = ptypestr + " WAPPKEY"
					if (ptype & PKT_FLAG_DATA_ENCRYPTED)==PKT_FLAG_DATA_ENCRYPTED:
						ptypestr = ptypestr + " ENCRYPTED"
					if (ptype & PKT_FLAG_ACK_REQ)==PKT_FLAG_ACK_REQ:
						ptypestr = ptypestr + " ACK_REQ"
				if ((ptype & 0xF0)==PKT_TYPE_ACK):
					ptypestr="ACK"
				src_r=arr[2]
				seq=arr[3]
				datalen=arr[4]
				SNR=arr[5]
				RSSI=arr[6]
				if (_rawFormat==0):
					info_str="(dst=%d type=0x%.2X(%s) src=%d seq=%d len=%d SNR=%d RSSI=%d)" % (dst,ptype,ptypestr,src,seq,datalen,SNR,RSSI)
				else:
					info_str="rawFormat(len=%d SNR=%d RSSI=%d)" % (datalen,SNR,RSSI)
				print info_str
			if (ch=='r'):
				data = sys.stdin.readline()
				arr = map(int,data.split(','))
				bw=arr[0]
				cr=arr[1]
				sf=arr[2]
				info_str="(BW=%d CR=%d SF=%d)" % (bw,cr,sf)
				#print info_str
			if (ch=='l'):
				print 'not implemented yet'
			if (ch=='$' and _logGateway==1):
				data = sys.stdin.readline()
				#print "rcv gw output to log (^$): "+data,
				f=open(os.path.expanduser(_gwlog_filename),"a")
				now = datetime.datetime.now()
				f.write(now.isoformat()+'> ')
				f.write(data)
				f.close()
			continue
		if (ch=='\\'):
			now = datetime.datetime.now()
			if _validappkey==1:
				ch=sys.stdin.read(1)
				if (ch=='!'): #log on ThingSpeak
					ldata = sys.stdin.readline()
					nsharp = ldata.count('#')
					if nsharp==0:
						data=['','','1']
						data[2]=ldata
					elif nsharp==1:
						data = ldata.split('#')
						if len(data[0])>1:
							data.insert(1,'');
						else:
							data.insert(0,'');
					else:
						data = ldata.split('#')
					data[2] = data[2].replace('\n', '')
					data[2] = data[2].replace('\0', '')
					tem = data
					hum = "ok"
					post_data ={
					"action":"update",
					"apikey":apikey,
					"deviceid":deviceID,
					"params":
					{
						"dst":dst,
						"type":ptype,
						"src":src,
						"seq":seq,
						"len":datalen,
						"SNR":SNR,
						"RSSI":RSSI,
						"Temp":data[2]
					}}
					http_post(post_data)
					print "\n----------------------------------------------------"
					print "recieve value is:"+ data[2]
					print "------------------------------------------------------"
				else: # not a known data logging prefix
					print 'unrecognized data logging prefix'
					sys.stdin.readline()
			else:
				print 'invalid app key: discard data'
				sys.stdin.readline()
			continue
		if (ch == '?' and _ignoreComment==1):
			sys.stdin.readline()
			continue
		sys.stdout.write(ch)

if __name__=='__main__':
	try:
			main()
	except KeyboardInterrupt:
			time.sleep(1)


