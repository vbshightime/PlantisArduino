#define THERMISTOR_PIN A0 
#define SOIL_PIN A1 

int RelayMode  = 2;
const int SAMPLE_NUMBER = 10;
const int BALANCE_RESISTOR = 10000; 
const double ROOM_TEMP = 298.15;
const double RESISTOR_ROOM_TEMP = 10000.0;
double currentTempC =0;
double resistTherm =0;
int output_value = 0;
String currentMoist ;
char data ;

const double BETA = 3799.42;


void setup() {
  // put your setup code here, to run once:
  pinMode(RelayMode, OUTPUT);

  Serial.begin(9600);

}

void loop() {

  digitalWrite(RelayMode,LOW); 
  // put your main code here, to run repeatedly:
  currentTempC = readThermister();
  String TempC = (String) currentTempC;
  currentMoist = readMoisture();
  String Moist = (String) currentMoist;


  if (Serial.available()){
        
       data=Serial.read();
       String Btdata = (String)data;

       if(Btdata.length()>0){

       switch(data){
            case 0:  
                     digitalWrite(RelayMode,LOW);  
                     break; //end message
            case 1: 
                    digitalWrite(RelayMode,HIGH);  
                    break;
            case 2:
                    Serial.print(Moist + "#");
                    break;
            case 3: 
                    Serial.print(TempC + "*");
                    break;
                   
            default:
                   digitalWrite(RelayMode,LOW); 
            
            }
                  
      }
  }
  
}

double readThermister()
{
  double adcAverage  = 0;            // Holds the average voltage measurement
  int adcSamples[SAMPLE_NUMBER];
  double rTherm =0 ;            // Holds thermistor resistance value
  double tKelvin =0 ;            // Holds calculated temperature
  double tCelsius =0; // Hold temperature in celsius
  

  for(int i=0; i<SAMPLE_NUMBER; i++){
    adcSamples[i] = analogRead(THERMISTOR_PIN);  // read from pin and store
    delay(100);        // wait 10 milliseconds
    }

    for(int i=0; i<SAMPLE_NUMBER; i++){
        adcAverage += adcSamples[i];      // add all samples up . . .
     }

     adcAverage /= SAMPLE_NUMBER;
     
     //Now find the resistance of thermistor by applying voltage devider rule 
     rTherm = BALANCE_RESISTOR * ((1023/adcAverage) - 1);
     tKelvin = (BETA * ROOM_TEMP) / (BETA + (ROOM_TEMP * log(rTherm / RESISTOR_ROOM_TEMP)));
     tCelsius = tKelvin - 273.15;
     return tCelsius;
}

int readMoisture(){
  
  output_value = analogRead(SOIL_PIN);
  int mapped_value = map(output_value, 1023, 470, 0,100);
  return mapped_value;
}

