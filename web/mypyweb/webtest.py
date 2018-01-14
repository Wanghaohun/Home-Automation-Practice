import os, os.path
import random
import string
import json
import operator
import cherrypy
import pymysql
import pyDes
import binascii
import socket



class StringGenerator(object):
    @cherrypy.expose
    def index(self):
        return open('index.html')       
        
    @cherrypy.expose
    def userinterface(self):
        return open('userinterface.html')
    
    @cherrypy.expose
    def backstage(self):  
        cookie = cherrypy.request.cookie
        cookieraw=str(cookie)
        print(cookieraw+'#')
        cookieraw=cookieraw.split('cookieName=')
        length=len(cookieraw)
        if length<2:
            cookieraw=None
        else:
            cookieraw=cookieraw[1]
            cookieraw=cookieraw[0:34]
            id=cookieraw[0]
        if cookieraw is None:   
            cookie2 = cherrypy.response.cookie
            cookie2['cookieName'] = None
            cookie2['cookieName']['path'] = '/'
            cookie2['cookieName']['max-age'] = 108000
            cookie2['cookieName']['version'] = 1            
            return open('userinterface.html')
        else:
            db = pymysql.connect("localhost","root","*******","pyweb" )
            cursor = db.cursor()
            cursor.execute("SELECT cookie from users where id='%s'"%id)
            sqldata = cursor.fetchone()
            db.close()
            if sqldata is None:   
                return open('userinterface.html')
            else:   
                print(cookieraw+'###'+sqldata[0])
                if operator.eq(cookieraw,sqldata[0]):
                    return open('backstage.html')
                else:
                    return open('userinterface.html')
        
    @cherrypy.expose    
    def treeexample(self):
        return open('treeexample.html')
    @cherrypy.expose    
    def aboutus(self):
        return open('aboutus.html')
    @cherrypy.expose    
    def test(self):
        return open('test.html')
    @cherrypy.expose
    def error(self):
        return open('error.html')
    @cherrypy.expose
    def datarev(self,sid,d1,d2,d3,d4):
        data_path=os.getcwd()+os.sep+'dataddd'
        if sid=="":
            return "sid null"
        elif operator.eq(sid,"1314159"):
            data=[{'d1':d1,'d2':d2,'d3':d3,'d4':d4}]
            jsonstring = json.dumps(data)
            open(data_path+os.sep+'data_log.txt','a').write(jsonstring+'\n')
            open(data_path+os.sep+'data_log.txt').close()
            return "ok"
        else:
            return "error"
        
        
@cherrypy.expose
class InterfaceVerification(object):
    def __init__(self):
        self.CAPTCHA = ""
    @cherrypy.tools.accept(media='text/plain')
    def GET(self):
        length=4
        some_string = ''.join(random.sample(string.hexdigits, int(length)))
        self.CAPTCHA = some_string
        return some_string

    def POST(self,username,password,captcha):
        if operator.eq(captcha.lower(),self.CAPTCHA.lower()):
            db = pymysql.connect("localhost","root","********","pyweb" )
            cursor = db.cursor()
            cursor.execute("SELECT * from users where username='%s'"%username)
            sqldata = cursor.fetchone()
            db.close()
            if sqldata is None:
                data=[{'mode':'deny'}]
                jsonstring = json.dumps(data)       
                return jsonstring
            else:   
                if operator.eq(password,sqldata[2]):
                    encryptdata = sqldata[1]+sqldata[2]
                    k = pyDes.des("DESCRYPT", pyDes.CBC, "\0\0\0\0\0\0\0\0", pad=None, padmode=pyDes.PAD_PKCS5)
                    d = k.encrypt(encryptdata)
                    cookie=str(sqldata[0])+str(binascii.hexlify(d)).replace("\'","")
                    data=[{'mode':'success','cookie':cookie}]                   
                    jsonstring = json.dumps(data)   
                    cookie2 = cherrypy.response.cookie
                    cookie2['cookieName'] = cookie
                    cookie2['cookieName']['path'] = '/'
                    cookie2['cookieName']['max-age'] = 108000
                    cookie2['cookieName']['version'] = 1            
                    return jsonstring 
                else:
                    data=[{'mode':'deny'}]
                    jsonstring = json.dumps(data)       
                    return jsonstring
        else:
            data=[{'mode':'wrong'}]
            jsonstring = json.dumps(data)       
            return jsonstring
            
@cherrypy.expose            
class BackStageService(object):
    @cherrypy.tools.accept(media='text/plain')
    def REFRESH(self):
        data_path=os.getcwd()+os.sep+'dataddd'
        with open(data_path+os.sep+'data_log.txt', 'r') as f:
            lines = f.readlines()
            last_line = lines[-1]
        return last_line
    def POST(self,id,time,d1,d2,d3,d4):
        if int(id)<5:
            data_path=os.getcwd()+os.sep+'dataddd'
            data=[{'d'+id+'_t':time,'d'+id+'_1':d1,'d'+id+'_2':d2,'d'+id+'_3':d3,'d'+id+'_4':d4}]
            jsonstring = json.dumps(data)
            open(data_path+os.sep+'data_note.txt','a').write(jsonstring+'\n')
            open(data_path+os.sep+'data_note.txt').close()
    @cherrypy.expose    
    def GET(self):
        data_path=os.getcwd()+os.sep+'dataddd'
        with open(data_path+os.sep+'data_note.txt', 'r') as f:
            lines = f.readlines()
            if len(lines)==4:
                jsondata=lines[0].replace("}]",",")+(lines[1].replace("}]","")).replace("[{","")+(lines[2].replace("}]","")).replace("[{",",")+lines[3].replace("[{",",")
            elif len(lines)==3:
                jsondata=lines[0].replace("}]",",")+(lines[1].replace("}]","")).replace("[{","")+lines[2].replace("[{",",")    
            elif len(lines)==2:
                jsondata=lines[0].replace("}]",",")+lines[1].replace("[{","")
            elif len(lines)==1:
                jsondata=lines[0];
        return(jsondata)
    def CLEAN(self):
        data_path=os.getcwd()+os.sep+'dataddd'
        open(data_path+os.sep+'data_note.txt','w').truncate()
        open(data_path+os.sep+'data_note.txt').close()
    def OPEN(self):
        addr=('10.18.31.175',8888)
        s=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
        msg="OPENPLEASE"
        s.sendto(msg.encode('utf-8'),addr)
        s.close()   
    def SHUT(self):
        addr=('10.18.31.175',8888)
        s=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
        msg="SHUTDOWNNOW"
        s.sendto(msg.encode('utf-8'),addr)
        s.close()   
if __name__ == '__main__':
    conf = {
        '/': {
            'tools.sessions.on': True,
            'tools.staticdir.root': os.path.abspath(os.getcwd())
        },
        '/generator': {
            'request.dispatch': cherrypy.dispatch.MethodDispatcher(),
            'tools.response_headers.on': True,
            'tools.response_headers.headers': [('Content-Type', 'text/plain')],
        },
        '/static': {
            'tools.staticdir.on': True,
            'tools.staticdir.dir': './public'
        },
        '/verification': {
            'request.dispatch': cherrypy.dispatch.MethodDispatcher(),
            'tools.response_headers.on': True,
            'tools.response_headers.headers': [('Content-Type', 'text/plain')],
        },
        '/bsverification': {
            'request.dispatch': cherrypy.dispatch.MethodDispatcher(),
            'tools.response_headers.on': True,
            'tools.response_headers.headers': [('Content-Type', 'text/plain')],
        }
    }
    webapp = StringGenerator()
    webapp.verification = InterfaceVerification()
    webapp.bsverification = BackStageService()
    cherrypy.server.socket_host = '10.18.31.167'
    cherrypy.quickstart(webapp, '/', conf)
    d1=1
    d2=1
    d3=1
    d4=1
    sid=1
    
    
    
    
