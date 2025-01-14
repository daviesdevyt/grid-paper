void set_straight(){
  servoMotor.write(95); // straight
  delay(500);
}

void set_left(){
  servoMotor.write(140); // left
  delay(500); 
}

void set_right(){
  servoMotor.write(45); // right
  delay(500);
}

float get_distance(){
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // measure duration of pulse from ECHO pin
  duration_us = pulseIn(ECHO_PIN, HIGH, 30000); //30ms

  // calculate the distance
  distance_cm = 0.017 * duration_us;

  // print the value to Serial Monitor
  // Serial.print("distance: ");
  // Serial.print(distance_cm);
  // Serial.println(" cm");
  return distance_cm;
}

String search_best_dir(){
  set_left();
  float left_distance = get_distance();
  if (left_distance > large_distance_threshold){
    return "spin_left";
  }
  set_right();
  float right_distance = get_distance();
  if (right_distance > large_distance_threshold){
    return "spin_right";
  }
  if (left_distance > right_distance && left_distance > distance_threshold){
    return "spin_left";
  }
  if (right_distance > left_distance && right_distance > distance_threshold){
    return "spin_right";
  }
  return "back";
}

void usfunction(){
  float dis = get_distance();
  if (dis < 40.0){
    stop();
    String dir = search_best_dir();
    Serial.print("Distance: ");
    Serial.println(dis);
    Serial.print("best dir: ");
    Serial.println(dir);
    set_straight();
    if (dir== "spin_left"){
      left();
      front(500);
      right();
      front(1000);
      right();
      front(500);
      left();
    }
    else if (dir== "spin_right"){
      right();
      front(500);
      left();
      front(1000);
      left();
      front(500);
      right();
    }
  }
}
