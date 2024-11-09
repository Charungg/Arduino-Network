#include "ASSP.h" //Import Networking functions
#include "names.h" //Import array of names over

#define LED_YELLOW 5
#define LED_PINK 6
#define LED_WHITE 9
#define LED_GREEN 10
#define LED_BLUE 11

#define amount_of_port 4
#define trials 50

const int initial_loop_value=0;

bool isHostOnPort(const char name[],int port){
  SerialShield.setMessageSender("ccc");       //Setting the message
  SerialShield.setMessageDestination(name);
  SerialShield.setMessageContent("ping");

  while(true){
    if(SerialShield.sendASSPMessage(port)){  //Sending the message to a specific port

      for (int i=initial_loop_value; i<trials; i++){
        if (SerialShield.fetchASSPMessage()){  //Fetching any message from any port
          return true;
        }
      }

      return false; //Host is not connected to that specific port
    }
  }
}


void task1(){
  Serial.println("Device   Port2?");

  for (int i=initial_loop_value; i<NUMNAMES; i++){ 
    Serial.print(names[i]);

    if (isHostOnPort(names[i],2)){  //Host is connected to port 2
      Serial.println("      YES");
    }

    else{                           //Host is not connected to port 2
      Serial.println("      NO");
    }
  }
}


int getPortForHost(const char name[]){
  for (int i=1; i<=amount_of_port; i++){  
    if (isHostOnPort(name,i)){   //Checks if host is connected to any port
      return i;   //Returns the connected port value
    }
  }
  return false;
}


void task2(){
  int connected_port_value;

  Serial.println("Network Names: ");
  
  for (int i=initial_loop_value; i<NUMNAMES; i++){          

    Serial.print(names[i]);                               
    Serial.print(" connected to Port ");
    connected_port_value=getPortForHost(names[i]); //Get the port value
    Serial.println(connected_port_value);

    if (connected_port_value!=false){   
      blink_network_name_leds(names[i]); //Passes the char name to a fucntion to blink the correct LEDs
      }
    }
  }

void blink_network_name_leds(char network_name[]){
  const int amount_of_leds = 5;
  const int amount_of_letters = 3;
  const int ASCII_to_alphabet_order = 64;
  const int five_bit_binary = 16;
  const int delay_time = 2000;
  
  int character_value;
  int binary_convertor;
  int set_leds[amount_of_leds]={LED_YELLOW,LED_PINK,LED_WHITE,LED_GREEN,LED_BLUE}; //Setting up the LEDS into an array

  for (int i=initial_loop_value; i<amount_of_letters; i++){              //This loop is deisgned to go through each letter of the network name

    character_value=network_name[i]-ASCII_to_alphabet_order;      //A character is selected from the network name which is a number in char and subtract by 64
    Serial.print(network_name[i]);                            //To coresspond a letter from 1-26 in the alphabet
    Serial.print(" = ");
    binary_convertor=five_bit_binary;                //Assign/Reset the binary converter to 16 for the new loop below

    for (int k=initial_loop_value; k<amount_of_leds; k++){

      if (character_value/binary_convertor){   //If the character value is divisalbe by the binary converter by 1 then
        character_value-=binary_convertor;        
        digitalWrite(set_leds[k],HIGH);           //It will lightup a lightbulb within the set_leds array depending on the k value
        Serial.print(1);
      }
      else{
        Serial.print(0);
      }
      binary_convertor/=2;   //convert 16 to 8 then 4 then 2 then 1 every iteration of the loop
                            // which is all to the power of 2 which is what binary use
    }

    Serial.println("");
    delay(delay_time);

    for (int i=initial_loop_value; i<amount_of_leds; i++){  //This will reset all the LED back to LOW meaning no lights
    digitalWrite(set_leds[i],LOW);  //After the LEDs light are on for a short amount of time then it will turn off all LED
    }
  }  
}


long getTimeToHost(const char name[], int port){
  long time_start=millis(); //Store current runtime of code

  if (isHostOnPort(name,port)){
    return (millis()-time_start);  //Returns current runtime of code minus the runtime when initiaiting this function
  }

  else{
    return false; //Returns a runtime value of 0
  }
} 


void task3(){
  int port;
  long time_value;
  long fastest_time;
  long slowest_time;

  Serial.println("Host    Port    Fastest (ms)    Slowest (ms)");

  for (int i=initial_loop_value; i<NUMNAMES; i++){  //Every iteration of loop gives a host
    Serial.print(names[i]);
    port=getPortForHost(names[i]);  //Store the port value assigned to the host

    if (port != false){   //If port value is not value 0
      fastest_time=99999;
      slowest_time=0;

      Serial.print("      ");
      Serial.print(port);
      Serial.print("     ");

      for (int k=initial_loop_value; k<trials; k++){  //Run response gathered from the host has 50 tries
        time_value=getTimeToHost(names[i],port);  //Store the host response time to send and recieve a message
 
        if (time_value<fastest_time && time_value!=0){  //If current host response time value is less than the fastest time then
          fastest_time=time_value;     //It will store/overwrite the fastest time varaible
        }

        if (time_value>slowest_time && time_value!=0){  //If current host response time value is more than the slowest time then
          slowest_time=time_value;     //It will store/overwrite the slowest time varaible
        }
      }

      Serial.print("     ");           //Displays the fastest and slowest times
      Serial.print(fastest_time);
      Serial.print("              ");
      Serial.println(slowest_time);
    }

    else{
      Serial.println("      NC");   //Displays "not connected"
    }   
  }
}

 
void setup(){
  const long instantiation_begin = 57600;
  const int instantiation_network_mode = 6;
  Serial.begin(instantiation_begin);
  SerialShield.setEmuMode(instantiation_network_mode); 
  SerialShield.showASSPmessages = false;  //Used for debugging sending and receiving messages

  pinMode(LED_YELLOW, OUTPUT);  //Sets up the LED to be ready to use
  pinMode(LED_PINK, OUTPUT);
  pinMode(LED_WHITE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);   
  
  task1();
  Serial.println("");
  task2();
  Serial.println("");
  task3();
}


void loop(){
  ;
}

