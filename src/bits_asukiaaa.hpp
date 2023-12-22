#pragma once

namespace bits_asukiaaa {

enum class ArrOrder {
  LowFirst,
  HighFirst,
};

void assignUint16ToBytes(uint8_t *bytes, uint16_t u16,
                         ArrOrder order = ArrOrder::HighFirst) {
  if (order == ArrOrder::LowFirst) {
    bytes[0] = lowByte(u16);
    bytes[1] = highByte(u16);
  } else {
    bytes[1] = lowByte(u16);
    bytes[0] = highByte(u16);
  }
}

uint16_t readUint16FromBytes(const uint8_t *bytes,
                             ArrOrder order = ArrOrder::HighFirst) {
  if (order == ArrOrder::LowFirst) {
    return (uint16_t)bytes[1] << 8 | bytes[0];
  } else {
    return (uint16_t)bytes[0] << 8 | bytes[1];
  }
}

void assignUint16ToBytesFromFloatWithDigitUnderPoint(
    uint8_t *bytes, float val, int numDigitUnderPoint,
    ArrOrder order = ArrOrder::HighFirst) {
  assignUint16ToBytes(bytes, round(val * pow(10, numDigitUnderPoint)), order);
}

float readUint16FromBytesAsFloatWithDigitUnderPoint(
    const uint8_t *bytes, int numDigitUnderPoint,
    ArrOrder order = ArrOrder::HighFirst) {
  return readUint16FromBytes(bytes, order) / pow(10, numDigitUnderPoint);
}

void assignUint32ToBytes(uint8_t *bytes, uint32_t u32,
                         ArrOrder order = ArrOrder::HighFirst) {
  if (order == ArrOrder::LowFirst) {
    bytes[0] = u32 & 0xff;
    bytes[1] = (u32 >> 8 * 1) & 0xff;
    bytes[2] = (u32 >> 8 * 2) & 0xff;
    bytes[3] = (u32 >> 8 * 3) & 0xff;
  } else {
    bytes[3] = u32 & 0xff;
    bytes[2] = (u32 >> 8 * 1) & 0xff;
    bytes[1] = (u32 >> 8 * 2) & 0xff;
    bytes[0] = (u32 >> 8 * 3) & 0xff;
  }
}

uint32_t readUint32FromBytes(const uint8_t *bytes,
                             ArrOrder order = ArrOrder::HighFirst) {
  if (order == ArrOrder::LowFirst) {
    return (uint32_t)bytes[3] << (8 * 3) | (uint32_t)bytes[2] << (8 * 2) |
           (uint32_t)bytes[1] << (8 * 1) | bytes[0];
  } else {
    return (uint32_t)bytes[0] << (8 * 3) | (uint32_t)bytes[1] << (8 * 2) |
           (uint32_t)bytes[2] << (8 * 1) | bytes[3];
  }
}

void assignUint32ToBytesFromFloatWithDigitUnderPoint(
    uint8_t *bytes, float val, int numDigitUnderPoint,
    ArrOrder order = ArrOrder::HighFirst) {
  assignUint32ToBytes(bytes, round(val * pow(10, numDigitUnderPoint)), order);
}

float readUint32FromBytesAsFloatWithDigitUnderPoint(
    const uint8_t *bytes, int numDigitUnderPoint,
    ArrOrder order = ArrOrder::HighFirst) {
  return readUint32FromBytes(bytes, order) / pow(10, numDigitUnderPoint);
}

bool isBitTrue(uint8_t byteData, uint8_t position) {
  return ((byteData >> position) & 0b1) == 1;
}

void setBitTrue(uint8_t *byteData, uint8_t position) {
  *byteData |= 1 << position;
}

}  // namespace bits_asukiaaa
