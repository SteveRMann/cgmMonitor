/************ showRssi() ************/
void showRssi() {
  // Shows the RSSI value on boot.

  long rssi = WiFi.RSSI();
  char rssiStr[16];
  itoa(rssi, rssiStr, 10);

  Serial.print(F("RSSI = '"));
  Serial.print(rssi);
  Serial.println(F("'"));

  Serial.print(F("rssiStr= '"));
  Serial.print (rssiStr);
  Serial.println(F("'"));


  timeDisplay.writeDigitRaw(0, letterR);
  timeDisplay.writeDigitRaw(1, letterS);
  timeDisplay.writeDigitRaw(2, letterS);
  timeDisplay.writeDigitRaw(3, letterI);
  timeDisplay.writeDisplay();

  //Since the first char is always "-", and we will never see a three-digit RSSI,
  //we only need the second and third characters.

  //Show the "-" in position 0
  bgDisplay.writeDigitAscii(0, '-');
  

  //Show the first character in the second position
  char buf[2];   //Temporary buffer
  strncpy_P(buf, rssiStr + 1, 1);
  buf[1] = 0;
  
  int i = atoi(buf);
  switch (i) {
    case 0:
      bgDisplay.writeDigitRaw(1, num0);
      break;
    case 1:
      bgDisplay.writeDigitRaw(1, num1);
      break;
    case 2:
      bgDisplay.writeDigitRaw(1, num2);
      break;
    case 3:
      bgDisplay.writeDigitRaw(1, num3);
      break;
    case 4:
      bgDisplay.writeDigitRaw(1, num4);
      break;
    case 5:
      bgDisplay.writeDigitRaw(1, num5);
      break;
    case 6:
      bgDisplay.writeDigitRaw(1, num6);
      break;
    case 7:
      bgDisplay.writeDigitRaw(1, num7);
      break;
    case 8:
      bgDisplay.writeDigitRaw(1, num8);
      break;
    case 9:
      bgDisplay.writeDigitRaw(1, num9);
      break;
    default:
      bgDisplay.writeDigitRaw(1, letterX);
  }


  //Show the second character
  strncpy_P(buf, rssiStr + 2, 1);
  buf[1] = 0;
  i = atoi(buf);
  switch (i) {
    case 0:
      bgDisplay.writeDigitRaw(2, num0);
      break;
    case 1:
      bgDisplay.writeDigitRaw(2, num1);
      break;
    case 2:
      bgDisplay.writeDigitRaw(2, num2);
      break;
    case 3:
      bgDisplay.writeDigitRaw(2, num3);
      break;
    case 4:
      bgDisplay.writeDigitRaw(2, num4);
      break;
    case 5:
      bgDisplay.writeDigitRaw(2, num5);
      break;
    case 6:
      bgDisplay.writeDigitRaw(2, num6);
      break;
    case 7:
      bgDisplay.writeDigitRaw(2, num7);
      break;
    case 8:
      bgDisplay.writeDigitRaw(2, num8);
      break;
    case 9:
      bgDisplay.writeDigitRaw(2, num9);
      break;
    default:
      bgDisplay.writeDigitRaw(2, letterX);
  }

  //Clear the third display caracter
  bgDisplay.writeDigitRaw(3, 0x0);
  bgDisplay.writeDisplay();

  Serial.println();
  delay(1500);
}
