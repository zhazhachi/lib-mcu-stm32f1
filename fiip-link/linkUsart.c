#include "./linkUsart.h"
static void linkUsart_start(LinkServerStruct* self);
static void linkUsart_send(LinkServerStruct* self,
                    uint8_t* data,
                    uint16_t dataLen,
                    LinkCfgStruct* link);
static void linkUsart_recv(uint32_t status);

void startUsart(uint8_t* dev, uint32_t speed) {
  LinkServerStruct linkServer;
  LinkCfgStruct link;
  mem_set(&linkServer, 0x00, sizeof(LinkServerStruct));
  mem_set(&link, 0x00, sizeof(LinkCfgStruct));

  link.type = linkType_usart;
  link.address = dev;
  link.port = speed;

  linkServer.link = &link;
  linkServer.start = linkUsart_start;
  linkServer.send = linkUsart_send;
  linkServer.recv = fiip_recv;
  linkServer.fd = malloc(sizeof(int));

  fiip_addServer(linkType_usart, &linkServer);
  linkServer.start(fiip_getServer(linkType_usart));
}

void linkUsart_start(LinkServerStruct* self) {
  if(self->link->address[0] == '1')
    it_addListener(usart1.IRQn ,linkUsart_recv);
  if(self->link->address[0] == '2')
    it_addListener(usart2.IRQn ,linkUsart_recv);
  if(self->link->address[0] == '3')
    it_addListener(usart3.IRQn ,linkUsart_recv);
}

void linkUsart_send(LinkServerStruct* self,
                    uint8_t* data,
                    uint16_t dataLen,
                    LinkCfgStruct* dstLink) {
  if (dstLink->address[0] == '1') {
    usart1.send((char*)data, dataLen);
  } else if (dstLink->address[0] == '2') {
    usart2.send((char*)data, dataLen);
  } else if (dstLink->address[0] == '3') {
    usart3.send((char*)data, dataLen);
  }
}

void linkUsart_recv(uint32_t status){
  LinkServerStruct* linkServer = fiip_getServer(linkType_usart);
  
  LinkCfgStruct srcLink;
  srcLink.type = linkType_usart;
  srcLink.address = (uint8_t*)malloc(8);
  srcLink.address[0] = linkServer->link->address[0];
  srcLink.port = linkServer->link->port;
  if(linkServer->link->address[0] == '1')
    linkServer->recv(usart1.rxBuf, usart1.rxNum, &srcLink);
  if(linkServer->link->address[0] == '2')
    linkServer->recv(usart2.rxBuf, usart2.rxNum, &srcLink);
  if(linkServer->link->address[0] == '3')
    linkServer->recv(usart3.rxBuf, usart3.rxNum, &srcLink);
}
