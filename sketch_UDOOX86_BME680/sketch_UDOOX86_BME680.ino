#include "DFRobot_BME680_I2C.h"
#include "Wire.h"
#include <ArduinoJson.h>
int written;
char jsonOut[256];
DFRobot_BME680_I2C bme(0x76);  //0x77 I2C address

void setup()
{
  Serial.begin(115200);
  while(!Serial);
  Serial.print("=============== init ===============");
  delay(1000);
  Serial.println();
  Serial.print(bme.begin());
}

void loop(){
  bme.startConvert();
  delay(1000);
  bme.update();
  StaticJsonBuffer<200> responseJsonBuffer;
  JsonObject& response = responseJsonBuffer.createObject();
  
  response["temperature"] = bme.readTempture();
  response["humidity"] = bme.readHumidity();
  response["pressure"] = bme.readPressure();
  uint16_t gas = bme.readGas();
  response["gas"] = gas;
  response["iaq"] = getIAQClassification(gas);
  response["gas_resistance"] = bme.readGasResistance();
  response["altitude"] = bme.readAltitude();
  written = response.printTo(jsonOut, 255);
  
  jsonOut[written] = '\n';
  written++;
  jsonOut[written] = '\0';
  Serial.print(((String)jsonOut).c_str());  
}

char* getIAQClassification(int gas){
    if(gas <= 50){
      return "good";
    }else if( gas <= 100){
      return "average";
    }else if( gas <= 150){
      return "little bad";
    }else if( gas <= 200){
      return "bad";
    }else if( gas <= 300){
      return "worse";
    }else {
      return "very bad";
    }
}
