#include <Arduino.h>

#include <bits_asukiaaa.hpp>

void setup() { Serial.begin(9600); }

void printU64(Stream* serial, uint64_t val) {
  const uint8_t lenVals = 20;
  uint8_t vals[lenVals];
  for (uint8_t i = 0; i < lenVals; ++i) {
    vals[i] = val % 10;
    val /= 10;
  }
  for (uint8_t i = 0; i < lenVals; ++i) {
    auto indexPrint = lenVals - i - 1;
    serial->print(vals[indexPrint]);
  }
}

void printlnU64(Stream* serial, uint64_t val) {
  printU64(serial, val);
  serial->println();
}

void convertUint64() {
  uint64_t val = 10000000000000000000 + millis();
  // val = millis(); // debug
  Serial.print("write ");
  printlnU64(&Serial, val);
  uint8_t data[8];
  bits_asukiaaa::assignUint64ToBytes(data, val);
  for (int i = 0; i < 8; ++i) {
    if (i != 0) {
      Serial.print(" ");
    }
    Serial.print(data[i]);
  }
  Serial.println();
  auto valReceive = bits_asukiaaa::readUint64FromBytes(data);
  Serial.print("read ");
  printlnU64(&Serial, valReceive);
}

void convertDouble() {
  double val = 100000.01 + millis();
  Serial.println("write " + String(val));
  uint8_t data[8];
  bits_asukiaaa::assignUint64ToBytesFromDouble(data, val);
  for (int i = 0; i < 8; ++i) {
    if (i != 0) {
      Serial.print(" ");
    }
    Serial.print(data[i]);
  }
  Serial.println();
  auto valReceive = bits_asukiaaa::readUint64FromBytesAsDouble(data);
  Serial.println("read " + String(valReceive));
}

void testNullable() {
  bits_asukiaaa::NFloat valFloat0;
  bits_asukiaaa::NFloat valFloat1;
  bits_asukiaaa::NullableValTemplate<float> valFloat2;
  bits_asukiaaa::NFloat valNone;

  valFloat0 = 3.0;
  valFloat1 = valFloat0;
  valFloat2 = valFloat0;
  Serial.println("valFloat0 " + valFloat0.toStr("none"));
  Serial.println("valFloat1 " + valFloat1.toStr("none"));
  Serial.println("valFloat3 " + valFloat1.toStr("none"));
  Serial.println("val 0 and 1 is same? " +
                 String(valFloat0 == valFloat1 ? "yes" : "no"));
  Serial.println("val 0 and none is same? " +
                 String(valFloat0 == valNone ? "yes" : "no"));
}

void loop() {
  convertUint64();
  convertDouble();
  testNullable();
  delay(2000);
}
