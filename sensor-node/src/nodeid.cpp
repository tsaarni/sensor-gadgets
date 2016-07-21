

#include <Arduino.h>
#include <EEPROM.h>

#include "nodeid.hpp"



NodeId::NodeId()
   : name_()
{
}


const char*
NodeId::get()
{
   // check if node name is already stored in eeprom
   if (eeprom_crc16_(node_name_len_ + 2))
   {
      // it was not: generate random node name
      int i;
      
      for (i=0; i < (node_name_len_); ++i)
      {
         EEPROM[i] = 'a' + random('z'-'a');
      }
      
      uint16_t crc = eeprom_crc16_(node_name_len_);
      EEPROM[i++] = static_cast<uint8_t>(crc>>8);
      EEPROM[i]   = static_cast<uint8_t>(crc & 0xff);
   }

   for (int i=0; i < node_name_len_; ++i)
   {
      name_[i] = EEPROM[i];
   }
   
   return name_;
}


void
NodeId::set(const char* node_id)
{
   int i;
   
   for (i=0; i < node_name_len_; ++i)
   {
      EEPROM[i] = node_id[i];

      if (node_id[i] == 0) break;
   }

   uint16_t crc = eeprom_crc16_(node_name_len_);
   EEPROM[i++] = static_cast<uint8_t>(crc>>8);
   EEPROM[i]   = static_cast<uint8_t>(crc & 0xff);
}


uint16_t
NodeId::eeprom_crc16_(size_t len)
{
   uint16_t crc = 0xffff;
   
   for (size_t i = 0; i < len; ++i)
   {
      crc ^= EEPROM[i] << 8;
      
      for (int bit = 8; bit > 0; --bit)
      {
         if (crc & 0x8000)
         {
            crc = (crc << 1) ^ 0x1021;
         }
         else
         {
            crc = (crc << 1);
         }
      }
   }

   return crc;
}
