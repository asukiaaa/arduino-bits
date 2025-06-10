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
  int16_t valInt = round(val * pow(10, numDigitUnderPoint));
  assignUint16ToBytes(bytes, (uint16_t)valInt, order);
}

float readUint16FromBytesAsFloatWithDigitUnderPoint(
    const uint8_t *bytes, int numDigitUnderPoint,
    ArrOrder order = ArrOrder::HighFirst) {
  return ((int16_t)readUint16FromBytes(bytes, order)) /
         pow(10, numDigitUnderPoint);
}

void assignUint32ToBytes(uint8_t *bytes, uint32_t u32,
                         ArrOrder order = ArrOrder::HighFirst) {
  if (order == ArrOrder::LowFirst) {
    bytes[0] = u32 & 0xff;
    bytes[1] = (u32 >> (8 * 1)) & 0xff;
    bytes[2] = (u32 >> (8 * 2)) & 0xff;
    bytes[3] = (u32 >> (8 * 3)) & 0xff;
  } else {
    bytes[3] = u32 & 0xff;
    bytes[2] = (u32 >> (8 * 1)) & 0xff;
    bytes[1] = (u32 >> (8 * 2)) & 0xff;
    bytes[0] = (u32 >> (8 * 3)) & 0xff;
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

void assignUint64ToBytes(uint8_t *bytes, uint64_t u64,
                         ArrOrder order = ArrOrder::HighFirst) {
  for (uint8_t i = 0; i < 8; ++i) {
    uint8_t indexToShift = order == ArrOrder::LowFirst ? i : 7 - i;
    bytes[i] = u64 >> (8 * indexToShift) & 0xff;
  }
}

uint64_t readUint64FromBytes(const uint8_t *bytes,
                             ArrOrder order = ArrOrder::HighFirst) {
  uint64_t val = 0;
  for (uint8_t i = 0; i < 8; ++i) {
    uint8_t indexToShift = order == ArrOrder::LowFirst ? i : 7 - i;
    val |= (uint64_t)bytes[i] << (8 * indexToShift);
  }
  return val;
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

typedef union {
  double value;
  byte bytes[8];
} UnionDouble;

void assignUint64ToBytesFromDouble(uint8_t *bytes, double val) {
  UnionDouble ud;
  ud.value = val;
  memcpy(bytes, ud.bytes, 8);
}

double readUint64FromBytesAsDouble(const uint8_t *bytes) {
  UnionDouble ud;
  memcpy(ud.bytes, bytes, 8);
  return ud.value;
}

bool isBitTrue(uint8_t byteData, size_t position) {
  return ((byteData >> position) & 0b1) == 1;
}

template <class BitType>
bool isBitTrue(BitType byteData, size_t position) {
  return ((byteData >> position) & 0b1) == 1;
}

void setBitTrue(uint8_t *byteData, size_t position) {
  *byteData |= 1 << position;
}

template <class BitType>
void setBitTrue(BitType *byteData, size_t position) {
  *byteData |= (BitType)1 << position;
}

void setBitU16True(uint16_t *byteData, size_t position) {
  *byteData |= (uint16_t)1 << position;
}

class InfoNullableBase {
 public:
  bool isUsed() const { return used; }
  bool isNull() const { return !used; }
  bool isNotNull() const { return used; }

 protected:
  bool used;
};

class InfoInt16Nullable : public InfoNullableBase {
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
    if (this != &other) {
      this->used = other.isUsed();
      this->val = other.getVal(-1);
    }
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

  void copyValue(const InfoFloatNullableBase &other) {
    used = other.isUsed();
    val = other.getVal(-1);
  }

  String toStr(String labelWhenBlank) const {
    if (used) {
      return String(val, digitUnderPoint);
    } else {
      return labelWhenBlank;
    }
  }

  InfoFloatNullable3Bytes &operator=(const InfoFloatNullableBase &other) {
    if (this != &other) {
      this->used = other.isUsed();
      this->val = other.getVal(-1);
    }
    return *this;
  }

  static const size_t lenBytes = 3;
  const size_t digitUnderPoint;
};

template <typename T>
class NullableValTemplate {
 public:
  NullableValTemplate() {}
  NullableValTemplate(T v) { setVal(v); }
  T getVal(T valIfNotAvairable) const {
    return avairable ? val : valIfNotAvairable;
  }
  bool isAvairable() const { return avairable; }
  bool isNull() const { return !avairable; }
  bool isNotNull() const { return avairable; }
  void clear() { avairable = false; }
  void setVal(T v) {
    val = v;
    avairable = true;
  }
  void setVal(const NullableValTemplate<T> &other) {
    if (other.isAvairable()) {
      val = other.getVal(0);
    } else {
      this->clear();
    }
  }
  void setValue(T v) { setVal(v); }
  void setValue(const NullableValTemplate<T> &other) { setVal(other); }
  void writeValIfAvairable(T *v) const {
    if (avairable) {
      *v = val;
    }
  }
  void writeValIfAvairable(NullableValTemplate<T> *other) const {
    if (avairable) {
      other->setVal(val);
    }
  }
  NullableValTemplate<T> &operator=(T v) {
    setVal(v);
    return *this;
  }
  NullableValTemplate<T> &operator=(const NullableValTemplate<T> &v) {
    setVal(v);
    return *this;
  }
  bool operator==(T v) { return avairable && v == val; }
  bool operator!=(T v) { return avairable && v != val; }
  bool operator==(const NullableValTemplate<T> &other) {
    return other.avairable == avairable && (!avairable || other.val == val);
  }
  bool operator!=(const NullableValTemplate<T> &other) {
    return !(this == other);
  }
  void overWriteIfAvairable(const NullableValTemplate<T> &other) {
    if (other.avairable) {
      setVal(other.val);
    }
  }
  String toStr(String labelWhenBlank) const {
    if (avairable) {
      return String(val);
    } else {
      return labelWhenBlank;
    }
  }

 protected:
  T val;
  bool avairable = false;
};

}  // namespace bits_asukiaaa
