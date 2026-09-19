# 📁 Description

This steganography challenge presents participants with a corrupted JPEG file.

At first glance, the file appears unreadable and cannot be opened using typical image viewers or identified correctly using standard forensic tools.

However, careful byte-level analysis reveals that the image was deliberately obfuscated using several reversible transformations inspired by the Force... and the wisdom of Master Yoda.

---

# 🧩 Technical Breakdown

The obfuscation process consists of three reversible transformations.

## 1. Byte Pair Swapping

Every pair of adjacent bytes is swapped:

~~~text
[x y] → [y x]
~~~

For example:

~~~text
[A B C D] → [B A D C]
~~~

This immediately disrupts the original binary structure of the JPEG.

---

## 2. 4-Byte Block Reordering

The file is then processed in blocks of 4 bytes.

Inside each block, the first two bytes are swapped with the last two:

~~~text
[A B C D] → [C D A B]
~~~

This further scrambles recognizable structures such as JPEG headers and metadata.

---

## 3. XOR Encryption

Finally, every byte is XORed with the repeating 4-byte key:

~~~text
Yoda
~~~

The operation can be represented as:

~~~text
encrypted[i] = data[i] XOR key[i mod 4]
~~~

This hides recognizable byte sequences such as JPEG magic bytes:

~~~text
FF D8 FF
~~~

After this final transformation, the file no longer resembles a valid JPEG.

---

# 🔓 Decryption Strategy

To recover the original image, the transformations must be reversed in the opposite order.

## 1. XOR with `"Yoda"`

Since XOR is symmetrical, applying the same key again restores the bytes from before the XOR step:

~~~text
(data XOR key) XOR key = data
~~~

The repeating key is:

~~~text
Yoda
~~~

---

## 2. Undo the 4-Byte Reordering

Each 4-byte block must be transformed back:

~~~text
[C D A B] → [A B C D]
~~~

---

## 3. Undo the Byte Pair Swapping

Finally, every adjacent pair of bytes is swapped again:

~~~text
[y x] → [x y]
~~~

After all three transformations are reversed, the original JPEG structure is restored and the image can be opened normally.

---

# 🧠 Summary

The complete obfuscation process is:

~~~text
Original JPEG
    ↓
Swap adjacent byte pairs
    ↓
Reorder 4-byte blocks
    ↓
XOR with "Yoda"
    ↓
Corrupted file
~~~

The decryption process is therefore:

~~~text
Corrupted file
    ↓
XOR with "Yoda"
    ↓
Undo 4-byte block reordering
    ↓
Undo adjacent byte swaps
    ↓
Original JPEG
~~~

Standard tools such as `file`, `binwalk`, and regular image viewers will fail until the binary structure has been restored.
