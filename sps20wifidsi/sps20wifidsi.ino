#include <senorConx.h>
#include <sps30.h>



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
int16_t ret;
  uint8_t auto_clean_days = 4;
  uint32_t auto_clean;

  Serial.begin(9600);
  delay(2000);

  sensirion_i2c_init();

  while (sps30_probe() != 0) {
    Serial.print("SPS sensor probing failed\n");
    delay(500);
  }

#ifndef PLOTTER_FORMAT
  Serial.print("SPS sensor probing successful\n");
#endif /* PLOTTER_FORMAT */

  ret = sps30_set_fan_auto_cleaning_interval_days(auto_clean_days);
  if (ret) {
    Serial.print("error setting the auto-clean interval: ");
    Serial.println(ret);
  }

  ret = sps30_start_measurement();
  if (ret < 0) {
    Serial.print("error starting measurement\n");
  }

#ifndef PLOTTER_FORMAT
  Serial.print("measurements started\n");
#endif /* PLOTTER_FORMAT */

#ifdef SPS30_LIMITED_I2C_BUFFER_SIZE
  Serial.print("Your Arduino hardware has a limitation that only\n");
  Serial.print("  allows reading the mass concentrations. For more\n");
  Serial.print("  information, please check\n");
  Serial.print("  https://github.com/Sensirion/arduino-sps#esp8266-partial-legacy-support\n");
  Serial.print("\n");
  delay(2000);
#endif

  delay(1000);

}

void repeat()
{   
 struct sps30_measurement m;
  char serial[SPS30_MAX_SERIAL_LEN];
  uint16_t data_ready;
  int16_t ret;

  do {
    ret = sps30_read_data_ready(&data_ready);
    if (ret < 0) {
      Serial.print("error reading data-ready flag: ");
      Serial.println(ret);
    } else if (!data_ready)
      Serial.print("data not ready, no new measurement available\n");
    else
      break;
    delay(100); /* retry in 100ms */
  } while (1);

  ret = sps30_read_measurement(&m);
  if (ret < 0) {
    Serial.print("error reading measurement\n");
  } else {


  connc.setPayload("pm1p0",m.mc_1p0,0);
 
    connc.setPayload("pm2p5",m.mc_2p5,1);

    connc.setPayload("pm4p0",m.mc_4p0,2);

    connc.setPayload("pm10p0",m.mc_10p0,3);


    
    connc.setPayload("nc0p5",m.nc_0p5,4);
    
    connc.setPayload("nc1p0",m.nc_1p0,5);
    
    connc.setPayload("nc2p5",m.nc_2p5,6);
    
    connc.setPayload("nc2p5",m.nc_4p0,7);
  
    connc.setPayload("nc10p0",m.nc_10p0,8);
    connc.setPayload("typ", m.typical_particle_size,9);

  }









 
}
