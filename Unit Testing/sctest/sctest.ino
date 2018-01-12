#include <Ethernet.h>
#define uint unsigned int
#define uchar unsigned char

// 输入MAC地址，及要访问的域名
byte mac[] = {  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
char serverName[] = "10.18.31.167";
String res="" ,query="",path="";
int i=0;
// 初始化客户端功能
EthernetClient client;
 void setup() {
 // 初始化串口通信
  Serial.begin(9600);
  delay(1);
 Serial.println("[getDHCPandDNS]"); 
  //开始Ethernet连接
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
// 如果DHCP方式获取IP失败,则使用自定义IP
  }
  else
  {
    Serial.println("My IP address: ");
        for (byte thisByte = 0; thisByte < 4; thisByte++) {
       // print the value of each byte of the IP address:
        Serial.print(Ethernet.localIP()[thisByte], DEC);
        Serial.print("."); }
  Serial.println("Ethernet configuration OK");
    }
  // 等待1秒钟用于Ethernet扩展板或W5100芯片完成初始化
  delay(1000);
Serial.println("connecting web server...");
client.connect(serverName, 8080);
  // 如果连接成功，通过串口输出返回数据
        if (client.connected())
        {
        Serial.println("Sending data");
        // 发送HTTP请求:
         client.print("GET /datarev?sid=1314159");
        client.print("&d1="); 
        client.print("11"); 
        client.print("&d2="); 
        client.print("11"); 
        client.print("&d3="); 
        client.print("11"); 
        client.print("&d4="); 
        client.print("11"); 
        client.println(" HTTP/1.1");
        client.println("Host: 10.18.31.167:8080");
        client.println();
        delay(6000);
         }  

}
 
void loop()
{    
                
    while (client.available() > 0) 
    {
      char c = client.read();

      if (c != '\r' ||i==7)
      { 
          res += c;
          if(i==7)
          {
            c = client.read();
            res += c;
            c = client.read();
            res += c;
            Serial.print(res);
            i=0;
          }
        } 
        else 
        {
          i++;
          Serial.print(res);
          res="";         
          delay(3000);
        }
      
        
    }

  
}
