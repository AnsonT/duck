#ifdef todo

// Pseudo Code ideas
void sendQuacks(String deviceID, String messageID, String payload)
{
  LoRa.beginPacket();
  couple(quacket_B, deviceID);
  couple(quacket_B, messageID);
  couple(quacket_B, payload);
  LoRa.endPacket();
}

sendQuacks(deviceID, messageID,payload);

#endif
