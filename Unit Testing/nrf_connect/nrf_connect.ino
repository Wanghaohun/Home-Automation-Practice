#include <iBoardRF24Network.h>
#include <iBoardRF24.h>
#include <digitalWriteFast.h>
#include <SPI.h>
#include <Ethernet.h>


uint16_t sid=001;//设置sid（设备号）
 
//NRF24l01发送接收节点
const uint16_t this_node = 0; 
const uint16_t other_node = 1;
 
//NRF24l01收发数据
struct payload_t
{
  uint32_t ms;
  uint32_t data;
  char sendchar[20];
};

//NRF24l01脚针设置
iBoardRF24 radioSend(3,8,5,6,7,2);
iBoardRF24Network networkSend(radioSend);

bool ispayload=false;//NRF24l01收发判断
uint16_t sendChannel=1;//NRF24l01发送频道
uint32_t sendCount=0;//NRF24l01发送次数

char incomingData[20]={0};//NRF24l01接收到的字符（其它设置发过来）





//设置MAC地址，如果有二个请改为不同(随意)
static byte mymac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  
EthernetClient client;//声明网关类

char server[] = "api.znck007.com";//声明API网址
//char urlBuf[] = "/index.php";

static long timer;//用于规定时间间隔
unsigned long last_http;//记录http时间，用于http超时判断，目前20秒无响应则重新GET。
bool ishttpget=false;//标示http是否已经结束



bool isUpdate=false;//成功则更新服务器数据为正常
char updateData[20]={0};//更新的字符（服务器通过字符分析指定某个设备）

//串口通信处理(主要用于zigbee通信)
unsigned long serialnowlast;
char serialbuff[80]={0};
char serialData;
int seriali=0;


 
void setup(void)
{
  //设置串口波特率
  Serial.begin(115200);
  Serial.println("IBOARD_GATEWAY_ZIGBEE/RF24Network");
  
  //NRF24l01初始化
  SPI.begin();
  radioSend.begin();
  
  //获取IP DNS等
/*  Serial.println("\n[getDHCPandDNS]"); 
  if (Ethernet.begin(mymac) == 0)
  {
        Serial.print(F("Failed to configure Ethernet using DHCP\n"));
        while(1);
   }
   else
   {
        Serial.print("My IP address: ");
        for (byte thisByte = 0; thisByte < 4; thisByte++) {
       // print the value of each byte of the IP address:
        Serial.print(Ethernet.localIP()[thisByte], DEC);
        Serial.print("."); }
  Serial.println("Ethernet configuration OK");
  }

  //判断web服务器是否正常
  timer=millis();
  while ((!client.available()) && (millis()-timer)<2000);

  if (client.available()) {
    Serial.println("Right");
    client.stop();
  }
  timer = millis();  
 
  //完成setup
  Serial.println("setup success");    
  */
}

void loop() {
   //NRF24l01检测接收数处理
  //如果使用NRF24l01,请启动这里（并删除ZIGBEE检测接收数据处理）
  networkSend.begin(/*channel*/ sid, /*node address*/ this_node);
  // Pump the network regularly
  networkSend.update();
  // Is there anything ready for us?
  timer=millis();
  //while ((!networkSend.available())  && (millis()-timer)<2000);
  while(networkSend.available())
  {
    // If so, grab it and print it out
    RF24NetworkHeader header;
    payload_t payload;
    memset(payload.sendchar, 0, 20);
    networkSend.read(header,&payload,sizeof(payload));
    Serial.print("Received packet #");
    Serial.print(payload.data);
    Serial.print(" at ");
    Serial.print(payload.ms);
    Serial.print(" getchar ");
    Serial.println(payload.sendchar); 

    ispayload = true;

    Serial.println(incomingData);
  }
  
  if(!ishttpget)
  {
  if(ispayload)
  {
    ispayload = false;

      if(strstr(incomingData,"{ck") && strstr(incomingData,"}") )
        {
          last_http = millis();          
          ishttpget=true;                      
          Serial.println("\n>>> POST");
          static char buf[20]={0};
          sprintf(buf,"%s",incomingData);
          return;
        }   
  
  }
  
  
  }


}
