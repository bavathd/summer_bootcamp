#include <senorConx.h>
#include<pezm.h>

pezm energyMeter(1, 9600, D7, D6); //  (nodeid, baudrate,  rx,  tx)


senorConx connc("D+W");

void setup() {
    connc.ssid = "Quest_Lab_Weather";
    connc.pass = "Questlab123";
    connc.token = "17876494-9075-4547-84f8-68c12f7ddee1";
    connc.initialize((void*(*)())start);
    
}

void loop()
{
    connc.looper((void*(*)())repeat);
}

void start() {
  Serial.begin(115200);
  energyMeter.begin();

}

void repeat()
{
energyMeter.getEnergy();
    
  connc.setPayload("voltage",energyMeter.energyMeter.voltage,0);
    
  connc.setPayload("current",energyMeter.energyMeter.current,1);
    
  connc.setPayload("freq",energyMeter.energyMeter.frequency,2);
    
  connc.setPayload("power",energyMeter.energyMeter.power,3);
    
  connc.setPayload("energy", energyMeter.energyMeter.energy,4);
    
  connc.setPayload("pf",energyMeter.energyMeter.powerFactor,5);
    
  delay(1000); // Read every second


 
}
