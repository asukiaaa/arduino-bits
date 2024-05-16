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

typedef union {
  float value;
  byte bytes[4];
} UnionFloat;

void assignUint32ToBytesFromFloat(uint8_t *bytes, float val) {
  UnionFloat uf;
  uf.value = val;
  memcpy(bytes, uf.bytes, 4);
}

float readUint32FromBytesAsFloat(const uint8_t *bytes) {
  UnionFloat uf;
  memcpy(uf.bytes, bytes, 4);
  return uf.value;
}

bool isBitTrue(uint8_t byteData, uint8_t position) {
  return ((byteData >> position) & 0b1) == 1;
}

void setBitTrue(uint8_t *byteData, uint8_t position) {
  *byteData |= 1 << position;
}

void setBitU16True(uint16_t *byteData, size_t position) {
  *byteData |= (uint16_t)1 << position;
}

class InfoNullableBase {
 public:
  bool isUsed() const { return used; }

 protected:
  bool used;
};

class InfoIntNullable3Bytes : public InfoNullableBase {
 public:
  void toBytes(uint8_t *bytes) const {
    if (used) setBitTrue(&bytes[0], 0);
    bits_asukiaaa::assignUint16ToBytes(&bytes[1], val);
  }

  void updateFromBytes(const uint8_t *bytes) {
    used = isBitTrue(bytes[0], 0);
    val = bits_asukiaaa::readUint16FromBytes(&bytes[1]);
  }

  void setValue(float val) {
    used = true;
    this->val = val;
  }

  void clear() {
    used = false;
    val = 0;
  }

  float getVal(uint16_t valWhenBlank) const {
    return used ? val : valWhenBlank;
  }

  String toStr(String labelWhenBlank) const {
    if (used) {
      return String(val);
    } else {
      return labelWhenBlank;
    }
  }

  static const size_t lenBytes = 3;

 private:
  uint16_t val;
};

class InfoFloatNullableBase : public InfoNullableBase {
 public:
  void setValue(float val) {
    used = true;
    this->val = val;
  }

  void clear() {
    used = false;
    val = 0;
  }

  float getVal(float valWhenBlank) const { return used ? val : valWhenBlank; }

 protected:
  float val;
};

class InfoFloatNullable : public InfoFloatNullableBase {
 public:
  void toBytes(uint8_t *bytes) const {
    if (used) setBitTrue(&bytes[0], 0);
    bits_asukiaaa::assignUint32ToBytesFromFloat(&bytes[1], val);
  }

  void updateFromBytes(const uint8_t *bytes) {
    used = isBitTrue(bytes[0], 0);
    val = bits_asukiaaa::readUint32FromBytesAsFloat(&bytes[1]);
  }

  InfoFloatNullable &operator=(const InfoFloatNullableBase &other) {
    this->used = other.isUsed();
    this->val = other.getVal(-1);
    return *this;
  }

  String toStr(String labelWhenBlank) const {
    if (used) {
      return String(val);
    } else {
      return labelWhenBlank;
    }
  }

  static const size_t lenBytes = 5;
};

class InfoFloatNullable3Bytes : public InfoFloatNullableBase {
 public:
  InfoFloatNullable3Bytes(size_t numDigitUnderPoint)
      : digitUnderPoint(numDigitUnderPoint) {}

  void toBytes(uint8_t *bytes) const {
    if (used) setBitTrue(&bytes[0], 0);
    bits_asukiaaa::assignUint16ToBytesFromFloatWithDigitUnderPoint(
        &bytes[1], val, digitUnderPoint);
  }

  void updateFromBytes(const uint8_t *bytes) {
    used = isBitTrue(bytes[0], 0);
    val = bits_asukiaaa::readUint16FromBytesAsFloatWithDigitUnderPoint(
        &bytes[1], digitUnderPoint);
  }

  String toStr(String labelWhenBlank) const {
    if (used) {
      return String(val, digitUnderPoint);
    } else {
      return labelWhenBlank;
    }
  }

  InfoFloatNullable3Bytes &operator=(const InfoFloatNullableBase &other) {
    this->used = other.isUsed();
    this->val = other.getVal(-1);
    return *this;
  }

  static const size_t lenBytes = 3;
  const size_t digitUnderPoint;
};

}  // namespace bits_asukiaaa
